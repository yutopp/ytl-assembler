#ifndef YTL_ASSEMBLER_X86_SHOW_HPP
#define YTL_ASSEMBLER_X86_SHOW_HPP

#include <iostream>
#include <iomanip>

#include "value.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace detail
        {
            template<typename SizeType, SizeType N>
            std::ostream& operator<<( std::ostream& os, short_buffer<SizeType, N> const& b )
            {
                auto const manip = os.flags();
                os << "[ " << std::hex;
                for( std::size_t i=0; i<b.size(); ++i ) {
                    os << "'0x" << std::setfill('0') << std::setw( 2 ) << static_cast<int>( b[i] ) << "' ";
                }
                os.setf( manip );
                os << "]";
                return os;
            }

        } // namespace detail
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_SHOW_HPP*/