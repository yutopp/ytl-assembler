#ifndef YTL_ASSEMBLER_ENDIAN_WRITER_HPP
#define YTL_ASSEMBLER_ENDIAN_WRITER_HPP

#include "detail/raw_little_endian_method.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace endian
        {
            typedef detail::raw_little_endian_method    little;

		} // namespace endian
	} // namespace assembeler
} // namespace ytl

#endif /*YTL_ASSEMBLER_ENDIAN_WRITER_HPP*/