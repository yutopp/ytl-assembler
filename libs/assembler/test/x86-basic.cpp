#include <boost/test/unit_test.hpp>

#include <type_traits>

#include <ytl/base/config.hpp>
#include <ytl/assembler/arch/x86/basic.hpp>

////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_multiply_type )
{
    using namespace ytl::assembler::x86;

    static_assert( std::is_same<
                        decltype( reg::eax * 1 ),
                        detail::sib_value<detail::register_none_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax * 2 ),
                        detail::sib_value<detail::register_none_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax * 4 ),
                        detail::sib_value<detail::register_none_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax * 8 ),
                        detail::sib_value<detail::register_none_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 1 * reg::eax ),
                        detail::sib_value<detail::register_none_t, reg::types::eax_t>
                   >::value, "" );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_multiply_value )
{
    using namespace ytl::assembler::x86;

#if defined(YTL_HAS_CONSTEXPR)

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 1 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 2 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 2 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 4 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 4 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 8 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 8 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( 1 * reg::eax );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

#endif

    {
        auto const rhs = ( reg::eax * 1 );
        auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 );
        BOOST_CHECK(
            rhs.scaling_factor == lhs.scaling_factor
        );
    }

    {
        auto const rhs = ( reg::eax * 2 );
        auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 2 );
        BOOST_CHECK(
            rhs.scaling_factor == lhs.scaling_factor
        );
    }

    {
        auto const rhs = ( reg::eax * 4 );
        auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 4 );
        BOOST_CHECK(
            rhs.scaling_factor == lhs.scaling_factor
        );
    }

    {
        auto const rhs = ( reg::eax * 8 );
        auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 8 );
        BOOST_CHECK(
            rhs.scaling_factor == lhs.scaling_factor
        );
    }

    {
        auto const rhs = ( 1 * reg::eax );
        auto const lhs = detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 );
        BOOST_CHECK(
            rhs.scaling_factor == lhs.scaling_factor
        );
    }
}




////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_base_multiply_type )
{
    using namespace ytl::assembler::x86;

    static_assert( std::is_same<
                        decltype( reg::eax + reg::eax * 1 ),
                        detail::sib_value<reg::types::eax_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax * 1 + reg::eax ),
                        detail::sib_value<reg::types::eax_t, reg::types::eax_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax + reg::ebx ),
                        detail::sib_value<reg::types::eax_t, reg::types::ebx_t>
                   >::value, "" );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_base_multiply_value )
{
    using namespace ytl::assembler::x86;

#if defined(YTL_HAS_CONSTEXPR)

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + reg::eax * 1 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::eax_t>( 1 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 1 + reg::eax );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::eax_t>( 1 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + reg::ebx );
        static YTL_CONSTEXPR auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 );
        static_assert( rhs.scaling_factor == lhs.scaling_factor, "" );
    }

#endif

    {
        auto const rhs = ( reg::eax + reg::eax * 1 );
        auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::eax_t>( 1 );
        BOOST_CHECK( rhs.scaling_factor == lhs.scaling_factor );
    }

    {
        auto const rhs = ( reg::eax * 1 + reg::eax );
        auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::eax_t>( 1 );
        BOOST_CHECK( rhs.scaling_factor == lhs.scaling_factor );
    }

    {
        auto const rhs = ( reg::eax + reg::ebx );
        auto const lhs = detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 );
        BOOST_CHECK( rhs.scaling_factor == lhs.scaling_factor );
    }
}




////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_disp_type )
{
    using namespace ytl::assembler::x86;

    // disp 8
    static_assert( std::is_same<
                        decltype( reg::eax + byte_value_t(32) ),
                        detail::reg_disp_value<reg::types::eax_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::ebx - byte_value_t(32) ),
                        detail::reg_disp_value<reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( byte_value_t(32) + reg::ebx ),
                        detail::reg_disp_value<reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    // disp 32
    static_assert( std::is_same<
                        decltype( reg::eax + 72 ),
                        detail::reg_disp_value<reg::types::eax_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::ebx - 72 ),
                        detail::reg_disp_value<reg::types::ebx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 72 + reg::ebx ),
                        detail::reg_disp_value<reg::types::ebx_t, dword_value_t>
                   >::value, "" );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_disp_value )
{
    using namespace ytl::assembler::x86;

#if defined(YTL_HAS_CONSTEXPR)

    // disp 8
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::eax_t, byte_value_t>( 32 );
        static_assert( rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::ebx - byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::ebx_t, byte_value_t>( -32 );
        static_assert( rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( byte_value_t(32) + reg::ebx );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::ebx_t, byte_value_t>( 32 );
        static_assert( rhs.value == lhs.value, "" );
    }

    // disp 32
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + 72 );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::eax_t, dword_value_t>( 72 );
        static_assert( rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::ebx - 72 );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::ebx_t, dword_value_t>( -72 );
        static_assert( rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( 72 + reg::ebx );
        static YTL_CONSTEXPR auto const lhs = detail::reg_disp_value<reg::types::ebx_t, dword_value_t>( 72 );
        static_assert( rhs.value == lhs.value, "" );
    }

#endif

    // disp 8
    {
        static auto const rhs = ( reg::eax + byte_value_t(32) );
        static auto const lhs = detail::reg_disp_value<reg::types::eax_t, byte_value_t>( 32 );
        BOOST_CHECK( rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::ebx - byte_value_t(32) );
        static auto const lhs = detail::reg_disp_value<reg::types::ebx_t, byte_value_t>( -32 );
        BOOST_CHECK( rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( byte_value_t(32) + reg::ebx );
        static auto const lhs = detail::reg_disp_value<reg::types::ebx_t, byte_value_t>( 32 );
        BOOST_CHECK( rhs.value == lhs.value );
    }

    // disp 32
    {
        static auto const rhs = ( reg::eax + 72 );
        static auto const lhs = detail::reg_disp_value<reg::types::eax_t, dword_value_t>( 72 );
        BOOST_CHECK( rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::ebx - 72 );
        static auto const lhs = detail::reg_disp_value<reg::types::ebx_t, dword_value_t>( -72 );
        BOOST_CHECK( rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( 72 + reg::ebx );
        static auto const lhs = detail::reg_disp_value<reg::types::ebx_t, dword_value_t>( 72 );
        BOOST_CHECK( rhs.value == lhs.value );
    }
}




////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_multiply_disp_type )
{
    using namespace ytl::assembler::x86;

    // disp 8
    static_assert( std::is_same<
                        decltype( reg::eax * 1 + byte_value_t(32) ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::ebx * 1 - byte_value_t(32) ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( byte_value_t(-32) + reg::ebx * 1 ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( byte_value_t(72) + 8 * reg::ebx ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    // disp 32
    static_assert( std::is_same<
                        decltype( reg::eax * 1 + 32 ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::ebx * 1 - 32 ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( -32 + reg::ebx * 1 ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 72 + 8 * reg::ebx ),
                        detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                   >::value, "" );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_multiply_disp_value )
{
    using namespace ytl::assembler::x86;

#if defined(YTL_HAS_CONSTEXPR)

    // disp 8
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 1 + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::ebx * 1 - byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( byte_value_t(-32) + reg::ebx * 1 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( byte_value_t(72) + 8 * reg::ebx );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 8 ), 72 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    // disp 32
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax * 1 + 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::ebx * 1 - 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( -32 + reg::ebx * 1 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( 72 + 8 * reg::ebx );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 8 ), 72 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

#endif

    // disp 8
    {
        static auto const rhs = ( reg::eax * 1 + byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::ebx * 1 - byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( byte_value_t(-32) + reg::ebx * 1 );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( byte_value_t(72) + 8 * reg::ebx );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 8 ), 72 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    // disp 32
    {
        static auto const rhs = ( reg::eax * 1 + 32 );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::eax_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::eax_t>( 1 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::ebx * 1 - 32 );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( -32 + reg::ebx * 1 );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 1 ), -32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( 72 + 8 * reg::ebx );
        static auto const lhs = detail::sib_disp_value<detail::register_none_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<detail::register_none_t, reg::types::ebx_t>( 8 ), 72 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }
}





////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_base_multiply_disp_type )
{
    using namespace ytl::assembler::x86;

    // disp 8
    static_assert( std::is_same<
                        decltype( reg::eax + reg::ebx * 1 + byte_value_t(32) ),
                        detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::edx * 2 + reg::ecx + byte_value_t(32) ),
                        detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( reg::eax + 4 * reg::ebx + byte_value_t(32) ),
                        detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 8 * reg::edx + reg::ecx + byte_value_t(32) ),
                        detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                   >::value, "" );

    // disp 32
    static_assert( std::is_same<
                        decltype( reg::eax + reg::ebx * 1 + 32 ),
                        detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 8 * reg::edx + reg::ecx + 32 ),
                        detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 8 * reg::edx + reg::ecx + 32 ),
                        detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                   >::value, "" );

    static_assert( std::is_same<
                        decltype( 8 * reg::edx + reg::ecx + 32 ),
                        detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                   >::value, "" );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_basic_address32_base_multiply_disp_value )
{
    using namespace ytl::assembler::x86;

#if defined(YTL_HAS_CONSTEXPR)

    // disp 8
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + reg::ebx * 1 + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::edx * 2 + reg::ecx + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 2 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + 4 * reg::ebx + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 4 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( 8 * reg::edx + reg::ecx + byte_value_t(32) );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 8 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    // disp 32
    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + reg::ebx * 1 + 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::edx * 2 + reg::ecx + 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 2 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( reg::eax + 4 * reg::ebx + 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 4 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

    {
        static YTL_CONSTEXPR auto const rhs = ( 8 * reg::edx + reg::ecx + 32 );
        static YTL_CONSTEXPR auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 8 ), 32 );
        static_assert( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value, "" );
    }

#endif

    // disp 8
    {
        static auto const rhs = ( reg::eax + reg::ebx * 1 + byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::edx * 2 + reg::ecx + byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 2 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::eax + 4 * reg::ebx + byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 4 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( 8 * reg::edx + reg::ecx + byte_value_t(32) );
        static auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, byte_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 8 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    // disp 32
    {
        static auto const rhs = ( reg::eax + reg::ebx * 1 + 32 );
        static auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 1 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::edx * 2 + reg::ecx + 32 );
        static auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 2 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( reg::eax + 4 * reg::ebx + 32 );
        static auto const lhs = detail::sib_disp_value<reg::types::eax_t, reg::types::ebx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::eax_t, reg::types::ebx_t>( 4 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }

    {
        static auto const rhs = ( 8 * reg::edx + reg::ecx + 32 );
        static auto const lhs = detail::sib_disp_value<reg::types::ecx_t, reg::types::edx_t, dword_value_t>
                                                ( detail::sib_value<reg::types::ecx_t, reg::types::edx_t>( 8 ), 32 );
        BOOST_CHECK( rhs.sib.scaling_factor == lhs.sib.scaling_factor && rhs.value == lhs.value );
    }
}




////////////////////////////////////////////////////////////////////////////////
//
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::framework::master_test_suite().p_name.value = "ytl.assembler x86 basic test";

    return nullptr;
}