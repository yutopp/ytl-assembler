#ifndef YTL_ASSEMBLER_X86_REGISTERS_HPP
#define YTL_ASSEMBLER_X86_REGISTERS_HPP

#include <cstddef>

#include "../detail/register_id.hpp"

#define RegisterTagDef( tag, bits )                 \
    namespace types {                               \
        struct tag {                                \
            static const std::size_t value = bits;  \
        };                                          \
    }

#define RegisterTypeDef( tag, name, bits )                          \
    RegisterTagDef( tag, bits )                                     \
    namespace types {                                               \
        /*typedef assembler::detail::register_operand<tag> name;*/  \
    }

#define RegisterDef( code, tag, name )                                                  \
    namespace types { typedef assembler::detail::register_code<code, tag> name##_t; }   \
    namespace { YTL_CONSTEXPR auto const name = types::name##_t(); }


namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            namespace reg
            {
                // 8bit register
                RegisterTypeDef( reg_8bit_tag, r8_t, 8 )
                RegisterDef( 0, reg_8bit_tag, al )
                RegisterDef( 1, reg_8bit_tag, cl )
                RegisterDef( 2, reg_8bit_tag, dl )
                RegisterDef( 3, reg_8bit_tag, bl )
                RegisterDef( 4, reg_8bit_tag, ah )
                RegisterDef( 5, reg_8bit_tag, ch )
                RegisterDef( 6, reg_8bit_tag, dh )
                RegisterDef( 7, reg_8bit_tag, bh )


                // 16bit register
                RegisterTypeDef( reg_16bit_tag, r16_t, 16 )
                RegisterDef( 0, reg_16bit_tag, ax )
                RegisterDef( 1, reg_16bit_tag, cx )
                RegisterDef( 2, reg_16bit_tag, dx )
                RegisterDef( 3, reg_16bit_tag, bx )
                RegisterDef( 4, reg_16bit_tag, sp )
                RegisterDef( 5, reg_16bit_tag, bp )
                RegisterDef( 6, reg_16bit_tag, si )
                RegisterDef( 7, reg_16bit_tag, di )
                

                // 16bit segment register
                RegisterTagDef( reg_seg_16bit_tag, 16 )
                RegisterDef( 0, reg_seg_16bit_tag, es )
                RegisterDef( 1, reg_seg_16bit_tag, cs )
                RegisterDef( 2, reg_seg_16bit_tag, ss )
                RegisterDef( 3, reg_seg_16bit_tag, ds )
                RegisterDef( 4, reg_seg_16bit_tag, fs )
                RegisterDef( 5, reg_seg_16bit_tag, gs )


                // 32bit register
                RegisterTypeDef( reg_32bit_tag, r32_t, 32 )
                RegisterDef( 0, reg_32bit_tag, eax )
                RegisterDef( 1, reg_32bit_tag, ecx )
                RegisterDef( 2, reg_32bit_tag, edx )
                RegisterDef( 3, reg_32bit_tag, ebx )
                RegisterDef( 4, reg_32bit_tag, esp )
                RegisterDef( 5, reg_32bit_tag, ebp )
                RegisterDef( 6, reg_32bit_tag, esi )
                RegisterDef( 7, reg_32bit_tag, edi )

            } // namespace registers
        } // namespece x86
    } // namespace assembler
} // namespace ytl

#undef RegisterTagDef
#undef RegisterTypeDef
#undef RegisterDef

#endif /*YTL_ASSEMBLER_X86_REGISTERS_HPP*/
