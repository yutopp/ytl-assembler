#include <boost/test/unit_test.hpp>

#include <ytl/base/config.hpp>

#include "gen.hpp"


////////////////////////////////////////////////////////////////////////////////
//
void asm_bin_util__b()
{
    using namespace ytl::assembler;

#if defined(YTL_HAS_CONSTEXPR)

    static_assert( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ).size() == 6, "" );
    static_assert( _b( 1, 2, 3, 4, 5, 6 ).size() == 6, "" );

    static_assert( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ), "" );
    static_assert( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 1, 2, 3, 4, 5, 6 ), "" );
    static_assert( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) != _b( 1, 2 ), "" );
    static_assert( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 2, 1 ), "" );

    static_assert( x86::detail::instruction_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ), "" );

#endif

    BOOST_CHECK( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ).size() == 6 );
    BOOST_CHECK( _b( 1, 2, 3, 4, 5, 6 ).size() == 6 );

    BOOST_CHECK( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ) );
    BOOST_CHECK( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 1, 2, 3, 4, 5, 6 ) );
    BOOST_CHECK( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) != _b( 1, 2 ) );
    BOOST_CHECK( x86::detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 2, 1 ) );

    BOOST_CHECK( x86::detail::instruction_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ) );
}

////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::test_suite* const test = BOOST_TEST_SUITE( "ytl.assembler _bin_util test" );

    test->add( BOOST_TEST_CASE( &asm_bin_util__b ) );

    return test;
}