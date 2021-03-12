#include <ios>
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>

#include "gtest/gtest.h"
#include "peeklib/storage/file.h"
#include "peeklib/storage/filemanager.h"
#include "peeklib/storage/streammanager.h"
#include "peeklib/util/debug.h"

TEST(BasicFileNameGeneratorTest, ConstructsAndCorrectOutput)
{
  peek::storage::FilenameGenerator g {"/peekdata/example-"};

  std::string expected_1 {"/peekdata/example-1"};
  std::string expected_2 {"/peekdata/example-6"};
  std::string output_1 = g(1);
  std::string output_2 = g(6);

  ASSERT_TRUE(output_1 == expected_1);
  ASSERT_TRUE(output_2 == expected_2);
}

TEST(FilenameManagerTests, ConstructsWithoutAnyValid) {
  const std::vector<FileId> valid_ids {};
  peek::storage::FilenameGenerator g{"/peekdata/filemanager_tests-"};
  FileManager fm {valid_ids, g};

  // this should create a new file
  auto sm = fm.getPermission();

  int x = 15;
  // write out integer to file
  // read out integer from the file
  sm->UseResource([x](auto r) {
    r->perform([x](std::fstream& out){
      out.seekp(0);
      out.write(reinterpret_cast<const char*>(&x), sizeof(x));
      out.flush();
    });
  });

  // read a single integer from the file
  auto result = sm->UseResource([](auto r) {
    return r->perform([](std::fstream& in) {
      auto x = int{};
      in.seekg(0);
      in.read(reinterpret_cast<char*>(&x), sizeof(x));
      return x;
    });
  });

  ASSERT_EQ(x, result);
}


TEST(FilenameManagerTests, MultipleUserThreads) {
  const std::vector<FileId> valid_ids {};
  peek::storage::FilenameGenerator g{"/peekdata/filemanager_tests_mt-"};
  FileManager fm {valid_ids, g};

  // this allows us to check to see if anything bad happened
  std::mutex m;
  std::vector<bool> results;

  // create a few different functions that threads could do
  // these will be doing "array operations" on the
  // file to simulate multiple threads working on the file
  auto read_l = [&](FileManager* fm, int offset, int expected) {
    auto result = int{};
    {
      auto sm = fm->getPermission();
      result = sm->UseResource([offset](auto r) {
        return r->perform([offset](std::fstream& in) {
          auto x = int{};
          in.seekg(offset * sizeof(x));
          in.read(reinterpret_cast<char*>(&x), sizeof(x));
          return x;
        });
      });
    }

    {
      std::lock_guard<std::mutex> g(m);
      results.push_back(expected == result);
    }
  };

  auto write_l = [&](FileManager* fm, int offset, int x) {
    {
      auto sm = fm->getPermission();
      sm->UseResource([offset, x](auto r) {
        r->perform([offset, x](std::fstream& out) {
          out.seekp(offset * sizeof(x));
          out.write(reinterpret_cast<const char*>(&x), sizeof(x));
        });
      });
    }
  };

  Debug::log([](auto& out) {out << "before write" << std::endl; });

  // now, lets make a bunch of threads to write to a file
  std::vector<std::thread> writer_threads {};
  int num_to_start_at = 4;
  int num_writer_threads = 10;
  for (int i = 0; i < num_writer_threads; ++i) {
    std::thread t(write_l, &fm, i, i+num_to_start_at);
    writer_threads.push_back(std::move(t));
  }
  Debug::log([](auto& out) {out << "write working" << std::endl; });

  for (auto& t : writer_threads) {
    t.join();
  }

  Debug::log([](auto& out) {out << "write done" << std::endl; });


  // now, lets make a bunch of threads to read the file and make sure its what we expect
  std::vector<std::thread> reader_threads {};
  for (int i = 0; i < num_writer_threads; ++i) {
    std::thread t(read_l, &fm, i, i+num_to_start_at);
    reader_threads.push_back(std::move(t));
  }

  Debug::log([](auto& out) {out << "read working" << std::endl; });

  for (auto& t : reader_threads) {
    t.join();
  }

  Debug::log([](auto& out) {out << "read done" << std::endl; });

  // now, check to make sure that no results were bad

  std::for_each(results.begin(), results.end(), [](bool a) {
    ASSERT_TRUE(a);
  });
}