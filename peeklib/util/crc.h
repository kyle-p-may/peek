#ifndef PEEKLIB_UTIL_CRC_H
#define PEEKLIB_UTIL_CRC_H

#include <cassert>
#include <cstdint>

#include <algorithm>
#include <array>
#include <numeric>

using Checksum_t = uint_fast32_t;

/**
 * This code was adapted from the rosettacode.org C++ implementation
 * of the CRC-32 algorithm 
 */

class CRC32 {
    public:
        // Calculates the CRC for any sequence of values. (You could use type traits and a
        // static assert to ensure the values can be converted to 8 bits.)
        template <typename InputIterator>
        static Checksum_t Calculate(InputIterator first, InputIterator last)
        {
        // Generate lookup table only on first use then cache it - this is thread-safe.
        static auto const table = generate_crc_lookup_table();
        
        // Calculate the checksum - make sure to clip to 32 bits, for systems that don't
        // have a true (fast) 32-bit type.
        return std::uint_fast32_t{0xFFFFFFFFuL} &
            ~std::accumulate(first, last,
            ~std::uint_fast32_t{0} & std::uint_fast32_t{0xFFFFFFFFuL},
                [](std::uint_fast32_t checksum, std::uint_fast8_t value) 
                { return table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8); });
        }

    private:
        // Generates a lookup table for the checksums of all 8-bit values.
        static std::array<std::uint_fast32_t, 256> generate_crc_lookup_table() noexcept
        {
            auto const reversed_polynomial = std::uint_fast32_t{0xEDB88320uL};
            
            // This is a function object that calculates the checksum for a value,
            // then increments the value, starting from zero.
            struct byte_checksum
            {
                std::uint_fast32_t operator()() noexcept
                {
                auto checksum = static_cast<std::uint_fast32_t>(n++);
            
                for (auto i = 0; i < 8; ++i)
                    checksum = (checksum >> 1) ^ ((checksum & 0x1u) ? reversed_polynomial : 0);
            
                return checksum;
                }
            
                unsigned n = 0;
            };
            
            auto table = std::array<std::uint_fast32_t, 256>{};
            std::generate(table.begin(), table.end(), byte_checksum{});
            
            return table;
        }
};


 



#endif // PEEKLIB_UTIL_CRC_H