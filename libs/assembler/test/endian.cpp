#include "gtest/gtest.h"

#include <ytl/assembler/endian.hpp>

TEST( Assembler, LittleEndianWriter )
{
    ytl::byte_t buf[sizeof(int)];
    union {
        ytl::byte_t b[sizeof(int)];
        int n;
    } u = { 0x12, 0x34, 0x56, 0x78 };

    ytl::assembler::endian::little::write( buf, &u.n, sizeof(int) );

    EXPECT_EQ( buf[0], 0x12 );
    EXPECT_EQ( buf[1], 0x34 );
    EXPECT_EQ( buf[2], 0x56 );
    EXPECT_EQ( buf[3], 0x78 );
}