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
                // -- AAA
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct AAA_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x37 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x37 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto AAA()
                    -> decltype( generic_instruction_call<detail::AAA_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::AAA_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- AAS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct AAS_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3f ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3f );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto AAS()
                    -> decltype( generic_instruction_call<detail::AAS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::AAS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- ADC
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct ADC_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x10 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x10 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x11 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x12 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x12 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x13 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x14 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x14 ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x15 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x15 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x15 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x15 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x15 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x15 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x15 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x15 ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto ADC( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::ADC_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::ADC_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- ADD
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct ADD_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x00 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x00 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x01 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x02 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x02 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x03 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x04 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x04 ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x05 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x05 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x05 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x05 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x05 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x05 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x05 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x05 ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto ADD( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::ADD_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::ADD_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- AND
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct AND_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x20 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x20 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x21 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x22 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x22 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x23 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x24 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x24 ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x25 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x25 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x25 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x25 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x25 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x25 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x25 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x25 ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto AND( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::AND_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::AND_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- ARPL
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct ARPL_impl
                    {
                        //
                        // ( r/m16, r16 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x63 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x63 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto ARPL( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::ARPL_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::ARPL_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- BOUND
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct BOUND_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto BOUND()
                    -> decltype( generic_instruction_call<detail::BOUND_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::BOUND_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- CMP
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct CMP_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x38 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x38 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x39 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3a ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3a ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x3b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3c ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x3c ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x3d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x3d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x3d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x3d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x3d ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto CMP( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::CMP_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::CMP_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- DAA
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct DAA_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x27 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x27 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto DAA()
                    -> decltype( generic_instruction_call<detail::DAA_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::DAA_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- DAS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct DAS_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2f ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2f );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto DAS()
                    -> decltype( generic_instruction_call<detail::DAS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::DAS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- DEC
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct DEC_impl
                    {
                        //
                        // ( r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x48 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x48 + Code );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x48 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x48 + Code );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0>
                YTL_CONSTEXPR auto DEC( P0&& p0 )
                    -> decltype( generic_instruction_call<detail::DEC_impl, BitsTag>( std::forward<P0>( p0 ) ) )
                {
                    return generic_instruction_call<detail::DEC_impl, BitsTag>( std::forward<P0>( p0 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- IMUL
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct IMUL_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto IMUL()
                    -> decltype( generic_instruction_call<detail::IMUL_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::IMUL_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- INC
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct INC_impl
                    {
                        //
                        // ( r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x40 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x40 + Code );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x40 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x40 + Code );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0>
                YTL_CONSTEXPR auto INC( P0&& p0 )
                    -> decltype( generic_instruction_call<detail::INC_impl, BitsTag>( std::forward<P0>( p0 ) ) )
                {
                    return generic_instruction_call<detail::INC_impl, BitsTag>( std::forward<P0>( p0 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- INS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct INS_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto INS()
                    -> decltype( generic_instruction_call<detail::INS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::INS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JB
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JB_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JB()
                    -> decltype( generic_instruction_call<detail::JB_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JB_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JBE
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JBE_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JBE()
                    -> decltype( generic_instruction_call<detail::JBE_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JBE_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JNB
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JNB_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JNB()
                    -> decltype( generic_instruction_call<detail::JNB_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JNB_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JNBE
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JNBE_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JNBE()
                    -> decltype( generic_instruction_call<detail::JNBE_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JNBE_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JNO
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JNO_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JNO()
                    -> decltype( generic_instruction_call<detail::JNO_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JNO_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JNS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JNS_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JNS()
                    -> decltype( generic_instruction_call<detail::JNS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JNS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JNZ
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JNZ_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JNZ()
                    -> decltype( generic_instruction_call<detail::JNZ_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JNZ_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JO
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JO_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JO()
                    -> decltype( generic_instruction_call<detail::JO_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JO_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JP
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JP_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JP()
                    -> decltype( generic_instruction_call<detail::JP_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JP_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JS_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JS()
                    -> decltype( generic_instruction_call<detail::JS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- JZ
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct JZ_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto JZ()
                    -> decltype( generic_instruction_call<detail::JZ_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::JZ_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
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
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto OR( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::OR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- OUTS
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct OUTS_impl
                    {
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto OUTS()
                    -> decltype( generic_instruction_call<detail::OUTS_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::OUTS_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- POP
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct POP_impl
                    {
                        //
                        // ( es )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::es_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x07 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x07 );
                        }
                        
                        //
                        // ( ss )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ss_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x17 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x17 );
                        }
                        
                        //
                        // ( ds )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ds_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1f ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1f );
                        }
                        
                        //
                        // ( r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x58 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x58 + Code );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x58 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x58 + Code );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0>
                YTL_CONSTEXPR auto POP( P0&& p0 )
                    -> decltype( generic_instruction_call<detail::POP_impl, BitsTag>( std::forward<P0>( p0 ) ) )
                {
                    return generic_instruction_call<detail::POP_impl, BitsTag>( std::forward<P0>( p0 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- POPA
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct POPA_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x61 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x61 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto POPA()
                    -> decltype( generic_instruction_call<detail::POPA_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::POPA_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- POPAD
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct POPAD_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x61 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x61 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto POPAD()
                    -> decltype( generic_instruction_call<detail::POPAD_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::POPAD_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- PUSH
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct PUSH_impl
                    {
                        //
                        // ( es )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::es_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x06 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x06 );
                        }
                        
                        //
                        // ( cs )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::cs_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x0e ) )
                        {
                            return buffer / static_cast<byte_t>( 0x0e );
                        }
                        
                        //
                        // ( ss )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ss_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x16 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x16 );
                        }
                        
                        //
                        // ( ds )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ds_t const& ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1e ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1e );
                        }
                        
                        //
                        // ( r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x50 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x50 + Code );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x50 + Code ) )
                        {
                            return buffer / static_cast<byte_t>( 0x50 + Code );
                        }
                        
                        //
                        // ( imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, byte_value_t const& p0 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x6a ) / p0 )
                        {
                            return buffer / static_cast<byte_t>( 0x6a ) / p0;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0>
                YTL_CONSTEXPR auto PUSH( P0&& p0 )
                    -> decltype( generic_instruction_call<detail::PUSH_impl, BitsTag>( std::forward<P0>( p0 ) ) )
                {
                    return generic_instruction_call<detail::PUSH_impl, BitsTag>( std::forward<P0>( p0 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- PUSHA
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct PUSHA_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x60 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x60 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto PUSHA()
                    -> decltype( generic_instruction_call<detail::PUSHA_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::PUSHA_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- PUSHAD
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct PUSHAD_impl
                    {
                        //
                        // (  )
                        YTL_CONSTEXPR auto op( Buffer const& buffer ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x60 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x60 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag>
                YTL_CONSTEXPR auto PUSHAD()
                    -> decltype( generic_instruction_call<detail::PUSHAD_impl, BitsTag>() )
                {
                    return generic_instruction_call<detail::PUSHAD_impl, BitsTag>();
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- SBB
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct SBB_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x18 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x18 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x19 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1a ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1a ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x1b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1c ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x1c ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x1d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x1d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x1d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x1d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x1d ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto SBB( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::SBB_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::SBB_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- SUB
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct SUB_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x28 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x28 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x29 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2a ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2a ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x2b ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2c ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x2c ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x2d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x2d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x2d ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x2d ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x2d ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto SUB( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::SUB_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::SUB_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
                ////////////////////////////////////////////////////
                // -- XOR
                // implementation
                namespace detail
                {
                    template<typename Buffer>
                    struct XOR_impl
                    {
                        //
                        // ( r/m8, r8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x30 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x30 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x31 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x32 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x32 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        YTL_CONSTEXPR auto op( Buffer const& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 ) )
                        {
                            return buffer / static_cast<byte_t>( 0x33 ) / generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x34 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x34 ) / p1;
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x35 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x35 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x35 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x35 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x35 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x35 ) / p1;
                        }
                        
                        YTL_CONSTEXPR auto op( Buffer const& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                            -> decltype( buffer / static_cast<byte_t>( 0x35 ) / p1 )
                        {
                            return buffer / static_cast<byte_t>( 0x35 ) / p1;
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename P0, typename P1>
                YTL_CONSTEXPR auto XOR( P0&& p0, P1&& p1 )
                    -> decltype( generic_instruction_call<detail::XOR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) ) )
                {
                    return generic_instruction_call<detail::XOR_impl, BitsTag>( std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                ////////////////////////////////////////////////////
                
            } // namespace instruction
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_INSTRUCTION_HPP*/
