#ifndef YTL_ASSEMBLER_X86_SHOW_HPP
#define YTL_ASSEMBLER_X86_SHOW_HPP

#include <iostream>
#include <iomanip>

#include "value.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            namespace detail
            {
                template<typename T, typename U, uint8_t N>
                std::basic_ostream<T, U>& operator<<( std::basic_ostream<T, U>& os, short_buffer<N> const& b )
                {
                    auto const manip = os.flags();
                    os << "[ " << std::hex;
                    for( std::size_t i=0; i<b.size; ++i ) {
                        os << "'0x" << std::setfill('0') << std::setw( 2 ) << static_cast<int>( b.bin[i] ) << "' ";
                    }
                    os.setf( manip );
                    os << "]";
                    return os;
                }

            } // namespace detail
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_SHOW_HPP*/