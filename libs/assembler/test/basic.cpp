#include "gtest/gtest.h"

#include <type_traits>

#include <ytl/assembler/exprimental/x86/basic.hpp>
#include <ytl/assembler/exprimental/x86/value.hpp>


TEST( Assembler, AsmX86BasicAddressMultiply32 )
{
    using namespace ytl::assembler::x86;

    {
        typedef decltype( reg::eax * 1 )    expr_t;
        typedef detail::sib_value<detail::register_none_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax * 2 )    expr_t;
        typedef detail::sib_value<detail::register_none_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax * 4 )    expr_t;
        typedef detail::sib_value<detail::register_none_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax * 8 )    expr_t;
        typedef detail::sib_value<detail::register_none_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax * 8 )    expr_t;
        typedef detail::sib_value<detail::register_none_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }
}

TEST( Assembler, AsmX86BasicAddress_Base_Multiply )
{
    using namespace ytl::assembler::x86;

    {
        typedef decltype( reg::eax + reg::eax * 1 )    expr_t;
        typedef detail::sib_value<reg::types::eax_t, reg::types::eax_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + reg::ebx )    expr_t;
        typedef detail::sib_value<reg::types::eax_t, reg::types::ebx_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }
}

TEST( Assembler, AsmX86BasicAddress_Disp )
{
    using namespace ytl::assembler::x86;

    {
        typedef decltype( reg::eax + byte[32] )    expr_t;
        typedef detail::reg_disp_value<reg::types::eax_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::ebx + byte[32] )    expr_t;
        typedef detail::reg_disp_value<reg::types::eax_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_FALSE( ret );
    }

    {
        typedef decltype( reg::eax + 32 )    expr_t;
        typedef detail::reg_disp_value<reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + dword[32] )    expr_t;
        typedef detail::reg_disp_value<reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }
}

TEST( Assembler, AsmX86BasicAddress_Multiply_Disp )
{
    using namespace ytl::assembler::x86;

    {
        typedef decltype( reg::eax * 1 + byte[32] )    expr_t;
        typedef detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::ebx * 1 + byte[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::eax_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_FALSE( ret );
    }

    {
        typedef decltype( reg::eax * 1 + 32 )    expr_t;
        typedef detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax * 1 + dword[32] )    expr_t;
        typedef detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }
}

TEST( Assembler, AsmX86BasicAddress_Base_Multiply_Disp )
{
    using namespace ytl::assembler::x86;

    {
        typedef decltype( reg::eax + reg::ebx * 1 + byte[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + reg::ebx * 1 + byte[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::eax_t, byte_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_FALSE( ret );
    }

    {
        typedef decltype( reg::eax + reg::ebx * 1 + 32 )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + reg::ebx * 1 + dword[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + reg::ebx + dword[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::eax + 1 * reg::ebx + dword[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::ebx + reg::eax + dword[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::ebx_t, reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }

    {
        typedef decltype( reg::ebx + reg::eax * 1 - dword[32] )    expr_t;
        typedef detail::sib_disp_value<reg::types::ebx_t, reg::types::eax_t, dword_value_t> expect_t;

        auto const ret = std::is_same<expr_t, expect_t>::value;
        EXPECT_TRUE( ret );
    }
}