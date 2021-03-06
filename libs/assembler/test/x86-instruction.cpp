#include <boost/test/unit_test.hpp>

#include <ytl/base/config.hpp>
#include <ytl/assembler/arch/x86/instruction.hpp>


#define EXPAND_ARGS(...) __VA_ARGS__


#if defined(YTL_HAS_CONSTEXPR)

# define CE_SHOW_EXP( inst, ... ) \
    std::cout << "static_assert   :: " << #inst << " " << #__VA_ARGS__ << " => ";

# define CE_INST_EXP( inst, ... ) \
    instruction:: inst <mode_tag>( __VA_ARGS__ )

# define CE_OP_EQ( prm, res ) \
    { \
        CE_SHOW_EXP prm \
        YTL_CONSTEXPR auto const b = CE_INST_EXP prm; \
        std::cout << b << std::endl; \
        static_assert( b == ytl::make_short_buffer( EXPAND_ARGS res ), "" ); \
    }

#else

# define CE_OP_EQ(...) /*nothing*/

#endif


# define SHOW_EXP( inst, ... ) \
    std::cout << "assert(dynamic) :: " << #inst << " " << #__VA_ARGS__ << " => ";

#define INST_EXP(inst, ...) \
    instruction:: inst <mode_tag>( __VA_ARGS__ );

#define OP_EQ(prm, res) \
    CE_OP_EQ( prm, res ) \
    { \
        SHOW_EXP prm \
        auto const b = INST_EXP prm; \
        std::cout << b << std::endl; \
        BOOST_CHECK_EQUAL( b, ytl::make_short_buffer( EXPAND_ARGS res ) ); \
    }


//
// binary expression is checked by nasm
//

////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_instruction_OR_32 )
{
    using namespace ytl::assembler::x86;
    typedef mode::bits_32_tag       mode_tag;

    //////
    // OR DL, DL
    OP_EQ( ( OR, reg::dl, reg::dl ), ( 0x08, 0xd2 ) )
    
    //////
    // OR CX, CX
    OP_EQ( ( OR, reg::cx, reg::cx ), ( 0x66, 0x09, 0xc9 ) )

    //////
    // OR [EAX], DL
    OP_EQ( ( OR, ptr[reg::eax], reg::dl ), ( 0x08, 0x10 ) )
    // OR [EDI], EAX
    OP_EQ( ( OR, ptr[reg::edi], reg::eax ), ( 0x09, 0x07 ) )
    // OR [EDI], CX
    OP_EQ( ( OR, ptr[reg::edi], reg::cx ), ( 0x66, 0x09, 0x0f ) )
    // OR [SI], EDX
    OP_EQ( ( OR, ptr[reg::si], reg::edx ), ( 0x67, 0x09, 0x14 ) )

    //////
    // OR DL, [EAX]
    OP_EQ( ( OR, reg::dl, ptr[reg::eax] ), ( 0x0a, 0x10 ) )
    // OR EAX, [EDI]
    OP_EQ( ( OR, reg::eax, ptr[reg::edi] ), ( 0x0b, 0x07 ) )
    // OR CX, [EDI]
    OP_EQ( ( OR, reg::cx, ptr[reg::edi] ), ( 0x66, 0x0b, 0x0f ) )
    // OR EDX, [SI]
    OP_EQ( ( OR, reg::edx, ptr[reg::si] ), ( 0x67, 0x0b, 0x14 ) )

    ///////
    // OR AL, 42
    OP_EQ( ( OR, reg::al, 42 ), ( 0x0c, 0x2a ) )
    // OR AX, 42
    OP_EQ( ( OR, reg::ax, byte[42] ), ( 0x66, 0x0d, 0x2a, 0x00 ) )
    // OR AX, 42
    OP_EQ( ( OR, reg::ax, word[42] ), ( 0x66, 0x0d, 0x2a, 0x00 ) );
    // OR AX, 42
    OP_EQ( ( OR, reg::ax, dword[42] ), ( 0x66, 0x0d, 0x2a, 0x00, 0x00, 0x00 ) )
    // OR EAX, 42
    OP_EQ( ( OR, reg::eax, byte[42] ), ( 0x0d, 0x2a, 0x00 ) )
    // OR EAX, 42
    OP_EQ( ( OR, reg::eax, word[42] ), ( 0x0d, 0x2a, 0x00 ) )
    // OR EAX, 42
    OP_EQ( ( OR, reg::eax, dword[42] ), ( 0x0d, 0x2a, 0x00, 0x00, 0x00 ) )/**/



    //
    OP_EQ( ( ARPL, reg::ax, reg::ax ), ( 0x63, 0xc0 ) )/**/
    
}


////////////////////////////////////////////////////////////////////////////////
//
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::framework::master_test_suite().p_name.value = "ytl.assembler x86 instruction test";

    return nullptr;
}