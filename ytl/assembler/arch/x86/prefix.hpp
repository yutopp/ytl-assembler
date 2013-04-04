#ifndef YTL_ASSEMBLER_X86_PREFIX_HPP
#define YTL_ASSEMBLER_X86_PREFIX_HPP

#include "../../config.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            namespace prefix
            {
                namespace
                {
                    byte_t const operand_size_override = 0x66;
                    byte_t const precision_size_override = 0x66;

                    byte_t const address_size_override = 0x67;
                }
            } // namespace prefix
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_PREFIX_HPP*/
