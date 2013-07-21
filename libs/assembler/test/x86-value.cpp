#include <boost/test/unit_test.hpp>

#include <ytl/base/config.hpp>
#include <ytl/assembler/arch/x86/value.hpp>



#define EXPAND_ARGS(...) __VA_ARGS__


#if defined(YTL_HAS_CONSTEXPR)

# define CE_SHOW_EXP( ... ) \
    std::cout << "static_assert   :: " << "generate_mod_rm_sib_disp " << #__VA_ARGS__ << " => ";

# define CE_OP_EQ( prm, res ) \
    { \
        using namespace ytl::assembler::x86; \
        CE_SHOW_EXP prm \
        YTL_CONSTEXPR auto const b = generate_mod_rm_sib_disp prm; \
        std::cout << b << std::endl; \
        static_assert( b == ytl::make_short_buffer( EXPAND_ARGS res ), "" ); \
    }

#else

# define CE_OP_EQ(...) /*nothing*/

#endif


# define SHOW_EXP( ... ) \
    std::cout << "assert(dynamic) :: " << "generate_mod_rm_sib_disp " << #__VA_ARGS__ << " => ";

#define OP_EQ(prm, res) \
    CE_OP_EQ( prm, res ) \
    { \
        using namespace ytl::assembler::x86; \
        SHOW_EXP prm \
        auto const b = generate_mod_rm_sib_disp prm; \
        std::cout << b << std::endl; \
        BOOST_CHECK_EQUAL( b, ytl::make_short_buffer( EXPAND_ARGS res ) ); \
    }


//
// binary expression is checked by nasm
//

////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_modrm_sib_disp_reg_mem16 )
{
    /*
        Mod 0b00
    */
    // EAX, [BX+SI]
    OP_EQ( ( reg::eax, ptr[reg::bx + reg::si] ), ( 0x00 ) );

    // ECX, [BX+DI]
    OP_EQ( ( reg::ecx, ptr[reg::bx + reg::di] ), ( 0x09 ) );

    // EDX, [BP+SI]
    OP_EQ( ( reg::edx, ptr[reg::bp + reg::si] ), ( 0x12 ) );

    // EBX, [BP+DI]
    OP_EQ( ( reg::ebx, ptr[reg::bp + reg::di] ), ( 0x1b ) );

    // SP, [SI]
    OP_EQ( ( reg::sp, ptr[reg::si] ), ( 0x24 ) );

    // BL, [DI]
    OP_EQ( ( reg::bl, ptr[reg::di] ), ( 0x1d ) );

    // BL, [disp16]
    OP_EQ( ( reg::bl, ptr[word[8]] ), ( 0x1e, 0x08, 0x00 ) );

    // SP, [BX]
    OP_EQ( ( reg::sp, ptr[reg::bx] ), ( 0x27 ) );


    /*
        Mod 0b01
    */
    // EAX, [BX+SI+72]
    OP_EQ( ( reg::eax, ptr[reg::bx + reg::si + byte[72]] ), ( 0x40, 0x48 ) );

    // ECX, [BX+DI+72]
    OP_EQ( ( reg::ecx, ptr[reg::bx + reg::di + byte[72]] ), ( 0x49, 0x48 ) );

    // EDX, [BP+SI+72]
    OP_EQ( ( reg::edx, ptr[reg::bp + reg::si + byte[72]] ), ( 0x52, 0x48 ) );

    // EBX, [BP+DI+72]
    OP_EQ( ( reg::ebx, ptr[reg::bp + reg::di + byte[72]] ), ( 0x5b, 0x48 ) );

    // SP, [SI+72]
    OP_EQ( ( reg::sp, ptr[reg::si + byte[72]] ), ( 0x64, 0x48 ) );

    // BL, [DI+72]
    OP_EQ( ( reg::bl, ptr[reg::di + byte[72]] ), ( 0x5d, 0x48 ) );

    // BL, [BP+72]
    OP_EQ( ( reg::bl, ptr[reg::bp + byte[72]] ), ( 0x5e, 0x48 ) );

    // SP, [BX+72]
    OP_EQ( ( reg::sp, ptr[reg::bx + byte[72]] ), ( 0x67, 0x48 ) );


    /*
        Mod 0b10
    */
    // EAX, [BX+SI+456]
    OP_EQ( ( reg::eax, ptr[reg::bx + reg::si + word[456]] ), ( 0x80, 0xc8, 0x01 ) );

    // ECX, [BX+DI+456]
    OP_EQ( ( reg::ecx, ptr[reg::bx + reg::di + word[456]] ), ( 0x89, 0xc8, 0x01 ) );

    // EDX, [BP+SI+456]
    OP_EQ( ( reg::edx, ptr[reg::bp + reg::si + word[456]] ), ( 0x92, 0xc8, 0x01 ) );

    // EBX, [BP+DI+456]
    OP_EQ( ( reg::ebx, ptr[reg::bp + reg::di + word[456]] ), ( 0x9b, 0xc8, 0x01 ) );

    // SP, [SI+456]
    OP_EQ( ( reg::sp, ptr[reg::si + word[456]] ), ( 0xa4, 0xc8, 0x01 ) );

    // BL, [DI+456]
    OP_EQ( ( reg::bl, ptr[reg::di + word[456]] ), ( 0x9d, 0xc8, 0x01 ) );

    // BL, [BP+456]
    OP_EQ( ( reg::bl, ptr[reg::bp + word[456]] ), ( 0x9e, 0xc8, 0x01 ) );

    // SP, [BX+456]
    OP_EQ( ( reg::sp, ptr[reg::bx + word[456]] ), ( 0xa7, 0xc8, 0x01 ) );


    /*
        Mod 0b11
    */
    OP_EQ( ( reg::ax, reg::ax ), ( 0xc0 ) );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_modrm_sib_disp_reg_mem32 )
{
    /*
        Mod 0b00
    */
    // CX, [EAX]
    OP_EQ( ( reg::cx, ptr[reg::eax] ), ( 0x08 ) );

    // EDX, [ESP]
    OP_EQ( ( reg::edx, ptr[reg::esp] ), ( 0x14, 0x24 ) );

    // ECX, [EDI * 1]
    // !! Different from NASM -> ECX, [EDI]
    OP_EQ( ( reg::ecx, ptr[reg::edi*1] ), ( 0x0c, 0x3d, 0x00, 0x00, 0x00, 0x00 ) );

    // ECX, [EDI]
    OP_EQ( ( reg::ecx, ptr[reg::edi] ), ( 0x0f ) );

    // EDX, [EDI * 2]
    // !! Different from NASM -> EDX, [EDI + EDI]
    OP_EQ( ( reg::edx, ptr[reg::edi*2] ), ( 0x14, 0x7d, 0x00, 0x00, 0x00, 0x00 ) );

    // EDX, [EDI + EDI]
    OP_EQ( ( reg::edx, ptr[reg::edi + reg::edi] ), ( 0x14, 0x3f ) );

    // EAX, [EDI * 4]
    OP_EQ( ( reg::eax, ptr[reg::edi*4] ), ( 0x04, 0xbd, 0x00, 0x00, 0x00, 0x00 ) );

    // EBX, [EDI * 8]
    OP_EQ( ( reg::ebx, ptr[reg::edi*8] ), ( 0x1c, 0xfd, 0x00, 0x00, 0x00, 0x00 ) );

    // BX, [100]
    OP_EQ( ( reg::bx, ptr[100] ), ( 0x1d, 0x64, 0x00, 0x00, 0x00 ) );


    /*
        Mod 0b01
    */
    // ESI, [EBP]
    OP_EQ( ( reg::esi, ptr[reg::ebp] ), ( 0x75, 0x00 ) );

    // EBX, [EAX+12]
    OP_EQ( ( reg::bx, ptr[reg::eax + byte[12]] ), ( 0x58, 0x0c ) );

    // [EAX+12], EBX
    OP_EQ( ( ptr[reg::eax + byte[12]], reg::bx ), ( 0x58, 0x0c ) );

    // [ESP+12], EDI
    OP_EQ( ( ptr[reg::esp + byte[12]], reg::edi ), ( 0x7c, 0x24, 0x0c ) );

    // EAX, [EAX + EBX * 1 + 32]
    OP_EQ( ( reg::eax, ptr[reg::eax + reg::ebx * 1 + byte[32]] ), ( 0x44, 0x18, 0x20 ) );

    // EAX, [EAX + EBX * 1 + 32]
    OP_EQ( ( ptr[reg::eax + reg::ebx * 1 + byte[32]], reg::eax ), ( 0x44, 0x18, 0x20 ) );

    // EAX, [EAX + EBX * 8 + 32]
    OP_EQ( ( ptr[reg::eax + reg::ebx * 8 + byte[32]], reg::eax ), ( 0x44, 0xd8, 0x20 ) );


    /*
        Mod 0b10
    */
    // EDI, [EAX + 128]
    OP_EQ( ( reg::edi, ptr[reg::eax + 128] ), ( 0xb8, 0x80, 0x00, 0x00, 0x00 ) );

    // ESI, [ESP + 128]
    OP_EQ( ( reg::esi, ptr[reg::esp + 128] ), ( 0xb4, 0x24, 0x80, 0x00, 0x00, 0x00 ) );

    // [ESP + ESI * 1 + 128], EBP
    OP_EQ( ( ptr[reg::esp + reg::esi * 1 + 128], reg::ebp ), ( 0xac, 0x34, 0x80, 0x00, 0x00, 0x00 ) );


    /*
        Mod 0b11
    */
    // EAX, EAX
    OP_EQ( ( reg::eax, reg::eax ), ( 0xc0 ) );

    // EAX, EBX
    OP_EQ( ( reg::eax, reg::ebx ), ( 0xd8 ) );
}


////////////////////////////////////////////////////////////////////////////////
//
BOOST_AUTO_TEST_CASE( asm_x86_modrm_sib_disp_reg_mem_from_opcode )
{
    OP_EQ( ( 2, reg::dx ), ( 0xd2 ) );
}


////////////////////////////////////////////////////////////////////////////////
//
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::framework::master_test_suite().p_name.value = "ytl.assembler x86 value test";

    return nullptr;
}