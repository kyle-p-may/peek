#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "peeklib/storage/logentry.h"

TEST(BasicLogEntryTest, WillConstruct)
{
  LogEntry l; 
}

TEST(BasicLogEntryTest, ModifyCorrect)
{
  LogEntry l(LogEntry::Type::Delete, 0, 32);

  ASSERT_TRUE(!l.isEnd());
  ASSERT_TRUE(!l.isBegin());
  ASSERT_TRUE(!l.hasFinished());
  ASSERT_TRUE(!l.hasCommitted());

  l.markBegin();
  ASSERT_TRUE(l.isBegin());
  ASSERT_FALSE(l.isEnd());
  l.markEnd();
  ASSERT_TRUE(l.isBegin());
  ASSERT_TRUE(l.isEnd());
}

TEST(BasicLogEntryTest, ReadAndWrite)
{
  const char* filename = "/peekdata/Log_ReadAndWritetest.data";
  {
    std::ofstream creator(filename, std::ios::out | std::ios::binary);
    creator.close();
  }

  std::fstream stream(filename, std::ios::out | std::ios::in | std::ios::binary);
  EXPECT_TRUE(stream.is_open());
  std::streampos pos = 0;

  LogEntry l(LogEntry::Type::Delete, 1, 512);
  l.markBegin();

  auto size = l.write(stream, pos);
  ASSERT_TRUE(size > 0);

  LogEntry nextL(stream, pos);

  ASSERT_TRUE(nextL.isBegin());
  ASSERT_FALSE(nextL.isEnd());
  ASSERT_FALSE(nextL.hasFinished());
  ASSERT_FALSE(nextL.hasCommitted());

  nextL.finish(stream);

  LogEntry finalL(stream, pos);
  ASSERT_TRUE(nextL.isBegin());
  ASSERT_FALSE(nextL.isEnd());
  ASSERT_TRUE(nextL.hasFinished());
  ASSERT_FALSE(nextL.hasCommitted());

  stream.close();
}