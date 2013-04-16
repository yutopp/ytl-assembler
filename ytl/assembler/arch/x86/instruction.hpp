#ifndef YTL_ASSEMBLER_X86_INSTRUCTION_HPP
#define YTL_ASSEMBLER_X86_INSTRUCTION_HPP

#include "value.hpp"
#include "call.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            namespace instruction
            {
                ////////////////////////////////////////////////////
                // -- OR
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct OR_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x08 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x08 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x09 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0a ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0a ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0c ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x0c ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x0d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x0d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x0d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x0d ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto OR()
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>();
                }
                template<typename BitsTag, typename P0>
                YTL_CONSTEXPR auto OR( P0&& p0 )
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ) ) )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ) );
                }
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto OR( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                template<typename BitsTag, typename P0, typename P1, typename P2>
                YTL_CONSTEXPR auto OR( P0&& p0, P1&& p1, P2&& p2 )
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ) ) )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ) );
                }
                template<typename BitsTag, typename P0, typename P1, typename P2, typename P3>
                YTL_CONSTEXPR auto OR( P0&& p0, P1&& p1, P2&& p2, P3&& p3 )
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ), std::forward<P3>( p3 ) ) )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ), std::forward<P3>( p3 ) );
                }
                ////////////////////////////////////////////////////
                
            } // namespace instruction
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_INSTRUCTION_HPP*/
