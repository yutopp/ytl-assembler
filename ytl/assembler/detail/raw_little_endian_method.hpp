#ifndef YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_METHOD_HPP
#define YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_METHOD_HPP

#include <cstring>

#include <type_traits>
#include <boost/type_traits.hpp>

#include "../config.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace detail
        {

#if defined(YTL_LITTLE_ENDIAN)

            struct raw_little_endian_method
            {
                template<typename T>
                static void write( byte_t* const dst, T const* const src, std::size_t const size )
                {
                    static_assert( boost::has_trivial_copy<T>::value, "" );

                    std::memcpy( dst, src, size );
                }

                template<typename T>
                static void read( byte_t* const dst, T const* const src, std::size_t const size )
                {
                    static_assert( boost::has_trivial_copy<T>::value, "" );

                    std::memcpy( dst, src, size );
                }
            };

#elif defined(YTL_BIG_ENDIAN)
# error "not implemented."
#else
# error "ƒ{"
#endif

        } // namespace detail
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_LITTLE_ENDIAN_METHOD_HPP*/
