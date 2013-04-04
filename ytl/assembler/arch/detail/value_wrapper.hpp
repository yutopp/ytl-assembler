#ifndef YTL_ASSEMBLER_VALIE_WRAPPER_HPP
#define YTL_ASSEMBLER_VALIE_WRAPPER_HPP

#include <type_traits>

#include "../../config.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace detail
        {
            template<typename T>
            class value_wrapper
            {
                static_assert( std::is_pod<T>::value, "T must be POD." )

            public:
                typedef T       value_type;

            public:
                explicit value_wrapper( value_type const& value )
                    : value_( value )
                {}

            public:
                operator value_type() const
                {
                    return value();
                }

                value_type value() const
                {
                    return value_;
                }

            private:
                value_type value_;
            };

        } // namespace detail
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_VALIE_WRAPPER_HPP*/