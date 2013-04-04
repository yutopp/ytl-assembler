#include <boost/test/unit_test.hpp>

#include <ytl/assembler/arch/x86/value.hpp>
#include <ytl/assembler/arch/x86/show.hpp>

#include "_bin_util/gen.hpp"

//
// binary expression is checked by nasm
//

////////////////////////////////////////////////////////////////////////////////
//
void asm_x86_modrm_sib_disp_reg_mem16()
{
    using namespace ytl::assembler::x86;

    /*
        Mod 0b00
    */
    // EAX, [BX+SI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, ptr[reg::bx + reg::si] ),
        _b( 0x00 )
        );

    // ECX, [BX+DI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ecx, ptr[reg::bx + reg::di] ),
        _b( 0x09 )
        );

    // EDX, [BP+SI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::bp + reg::si] ),
        _b( 0x12 )
        );

    // EBX, [BP+DI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ebx, ptr[reg::bp + reg::di] ),
        _b( 0x1b )
        );

    // SP, [SI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::si] ),
        _b( 0x24 )
        );

    // BL, [DI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[reg::di] ),
        _b( 0x1d )
        );

    // BL, [disp16]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[word[8]] ),
        _b( 0x1e, 0x08, 0x00 )
        );

    // SP, [BX]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::bx] ),
        _b( 0x27 )
        );

    /*
        Mod 0b01
    */
    // EAX, [BX+SI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, ptr[reg::bx + reg::si + byte[72]] ),
        _b( 0x40, 0x48 )
        );

    // ECX, [BX+DI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ecx, ptr[reg::bx + reg::di + byte[72]] ),
        _b( 0x49, 0x48 )
        );

    // EDX, [BP+SI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::bp + reg::si + byte[72]] ),
        _b( 0x52, 0x48 )
        );

    // EBX, [BP+DI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ebx, ptr[reg::bp + reg::di + byte[72]] ),
        _b( 0x5b, 0x48 )
        );

    // SP, [SI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::si + byte[72]] ),
        _b( 0x64, 0x48 )
        );

    // BL, [DI+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[reg::di + byte[72]] ),
        _b( 0x5d, 0x48 )
        );

    // BL, [BP+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[reg::bp + byte[72]] ),
        _b( 0x5e, 0x48 )
        );

    // SP, [BX+72]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::bx + byte[72]] ),
        _b( 0x67, 0x48 )
        );


    /*
        Mod 0b10
    */
    // EAX, [BX+SI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, ptr[reg::bx + reg::si + word[456]] ),
        _b( 0x80, 0xc8, 0x01 )
        );

    // ECX, [BX+DI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ecx, ptr[reg::bx + reg::di + word[456]] ),
        _b( 0x89, 0xc8, 0x01 )
        );

    // EDX, [BP+SI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::bp + reg::si + word[456]] ),
        _b( 0x92, 0xc8, 0x01 )
        );

    // EBX, [BP+DI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ebx, ptr[reg::bp + reg::di + word[456]] ),
        _b( 0x9b, 0xc8, 0x01 )
        );

    // SP, [SI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::si + word[456]] ),
        _b( 0xa4, 0xc8, 0x01 )
        );

    // BL, [DI+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[reg::di + word[456]] ),
        _b( 0x9d, 0xc8, 0x01 )
        );

    // BL, [BP+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bl, ptr[reg::bp + word[456]] ),
        _b( 0x9e, 0xc8, 0x01 )
        );

    // SP, [BX+456]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::sp, ptr[reg::bx + word[456]] ),
        _b( 0xa7, 0xc8, 0x01 )
        );


    /*
        Mod 0b11
    */
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ax, reg::ax ),
        _b( 0xc0 )
        );
}


////////////////////////////////////////////////////////////////////////////////
//
void asm_x86_modrm_sib_disp_reg_mem32()
{
    using namespace ytl::assembler::x86;

    /*
        Mod 0b00
    */
    // CX, [EAX]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::cx, ptr[reg::eax] ),
        _b( 0x08 )
        );

    // EDX, [ESP]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::esp] ),
        _b( 0x14, 0x24 )
        );

    // ECX, [EDI * 1]
    // !! Different from NASM -> ECX, [EDI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ecx, ptr[reg::edi*1] ),
        _b( 0x0c, 0x3d, 0x00, 0x00, 0x00, 0x00 )
        );

    // ECX, [EDI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ecx, ptr[reg::edi] ),
        _b( 0x0f )
        );

    // EDX, [EDI * 2]
    // !! Different from NASM -> EDX, [EDI + EDI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::edi*2] ),
        _b( 0x14, 0x7d, 0x00, 0x00, 0x00, 0x00 )
        );

    // EDX, [EDI + EDI]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edx, ptr[reg::edi + reg::edi] ),
        _b( 0x14, 0x3f )
        );

    // EAX, [EDI * 4]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, ptr[reg::edi*4] ),
        _b( 0x04, 0xbd, 0x00, 0x00, 0x00, 0x00 )
        );

    // EBX, [EDI * 8]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::ebx, ptr[reg::edi*8] ),
        _b( 0x1c, 0xfd, 0x00, 0x00, 0x00, 0x00 )
        );

    // BX, [100]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bx, ptr[100] ),
        _b( 0x1d, 0x64, 0x00, 0x00, 0x00 )
        );



    /*
        Mod 0b01
    */
    // ESI, [EBP]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::esi, ptr[reg::ebp] ),
        _b( 0x75, 0x00 )
        );

    // EBX, [EAX+12]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::bx, ptr[reg::eax + byte[12]] ),
        _b( 0x58, 0x0c )
        );

    // [EAX+12], EBX
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( ptr[reg::eax + byte[12]], reg::bx ),
        _b( 0x58, 0x0c )
        );

    // [ESP+12], EDI
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( ptr[reg::esp + byte[12]], reg::edi ),
        _b( 0x7c, 0x24, 0x0c )
        );

    // EAX, [EAX + EBX * 1 + 32]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, ptr[reg::eax + reg::ebx * 1 + byte[32]] ),
        _b( 0x44, 0x18, 0x20 )
        );

    // EAX, [EAX + EBX * 1 + 32]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( ptr[reg::eax + reg::ebx * 1 + byte[32]], reg::eax ),
        _b( 0x44, 0x18, 0x20 )
        );

    // EAX, [EAX + EBX * 8 + 32]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( ptr[reg::eax + reg::ebx * 8 + byte[32]], reg::eax ),
        _b( 0x44, 0xd8, 0x20 )
        );

    /*
        Mod 0b10
    */
    // EDI, [EAX + 128]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::edi, ptr[reg::eax + 128] ),
        _b( 0xb8, 0x80, 0x00, 0x00, 0x00 )
        );

    // ESI, [ESP + 128]
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::esi, ptr[reg::esp + 128] ),
        _b( 0xb4, 0x24, 0x80, 0x00, 0x00, 0x00 )
        );

    // [ESP + ESI * 1 + 128], EBP
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( ptr[reg::esp + reg::esi * 1 + 128], reg::ebp ),
        _b( 0xac, 0x34, 0x80, 0x00, 0x00, 0x00 )
        );


    /*
        Mod 0b11
    */
    // EAX, EAX
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, reg::eax ),
        _b( 0xc0 )
        );

    // EAX, EBX
    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( reg::eax, reg::ebx ),
        _b( 0xd8 )
        );

}


////////////////////////////////////////////////////////////////////////////////
//
void asm_x86_modrm_sib_disp_reg_mem_from_opcode()
{
    using namespace ytl::assembler::x86;

    BOOST_CHECK_EQUAL(
        generate_mod_rm_sib_disp( 2, reg::dx ),
        _b( 0xd2 )
        );
}


////////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
boost::unit_test::test_suite* init_unit_test_suite( int, char* [] )
{
    boost::unit_test::test_suite* const test = BOOST_TEST_SUITE( "ytl.assembler x86 value test" );

    test->add( BOOST_TEST_CASE( &asm_x86_modrm_sib_disp_reg_mem16 ) );
    test->add( BOOST_TEST_CASE( &asm_x86_modrm_sib_disp_reg_mem32 ) );
    test->add( BOOST_TEST_CASE( &asm_x86_modrm_sib_disp_reg_mem_from_opcode ) );

    return test;
}