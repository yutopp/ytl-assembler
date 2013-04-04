#include <boost/test/unit_test.hpp>

#include "gen.hpp"


////////////////////////////////////////////////////////////////////////////////
//
void asm_bin_util__b()
{
    using namespace ytl::assembler::x86;

    BOOST_CHECK( detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ) );
    BOOST_CHECK( detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 1, 2, 3, 4, 5, 6 ) );
    BOOST_CHECK( detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) != _b( 1, 2 ) );
    BOOST_CHECK( detail::mod_rm_sib_disp_buffer( detail::buffer_construct, 1, 2 ) != _b( 2, 1 ) );

    BOOST_CHECK( detail::instruction_buffer( detail::buffer_construct, 1, 2, 3, 4, 5, 6 ) == _b( 1, 2, 3, 4, 5, 6 ) );
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