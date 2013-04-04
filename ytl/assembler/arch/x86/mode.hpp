#ifndef YTL_ASSEMBLER_X86_MODE_HPP
#define YTL_ASSEMBLER_X86_MODE_HPP

#include <cstddef>

#include "../../config.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            namespace mode
            {
                struct bits_16_tag { static const std::size_t value = 16; };
                struct bits_32_tag { static const std::size_t value = 32; };

            } // namespace mode
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_MODE_HPP*/
