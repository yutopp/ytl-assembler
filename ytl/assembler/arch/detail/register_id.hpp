#ifndef YTL_ASSEMBLER_REGISTER_ID_HPP
#define YTL_ASSEMBLER_REGISTER_ID_HPP

#include "../../config.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace detail
        {
            template<byte_t Code, typename Tag>
            class register_code
            {
            public:
                static byte_t const value = Code;

            public:
                register_code() {}

            public:
                byte_t code() const { return value; }
            };

        } // namespece detail
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_REGISTER_ID_HPP*/
