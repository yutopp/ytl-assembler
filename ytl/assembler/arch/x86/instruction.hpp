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
                        void op( Buffer& buffer, types::r_m8 const& p0, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p1 ) const
                        {
                            buffer << 0x08;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r/m16/32, r16/32 )
                        template<byte_t Code>
                        void op( Buffer& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                        {
                            buffer << 0x09;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, types::r_m16 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                        {
                            buffer << 0x09;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p1 ) const
                        {
                            buffer << 0x09;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, types::r_m32 const& p0, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p1 ) const
                        {
                            buffer << 0x09;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r8, r/m8 )
                        template<byte_t Code>
                        void op( Buffer& buffer, assembler::detail::register_code<Code, reg::types::reg_8bit_tag> const& p0, types::r_m8 const& p1 ) const
                        {
                            buffer << 0x0a;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( r16/32, r/m16/32 )
                        template<byte_t Code>
                        void op( Buffer& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m16 const& p1 ) const
                        {
                            buffer << 0x0b;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, assembler::detail::register_code<Code, reg::types::reg_16bit_tag> const& p0, types::r_m32 const& p1 ) const
                        {
                            buffer << 0x0b;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m16 const& p1 ) const
                        {
                            buffer << 0x0b;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        template<byte_t Code>
                        void op( Buffer& buffer, assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& p0, types::r_m32 const& p1 ) const
                        {
                            buffer << 0x0b;
                            buffer << generate_mod_rm_sib_disp( p0, p1 );
                        }
                        
                        //
                        // ( al, imm8 )
                        void op( Buffer& buffer, reg::types::al_t const&, byte_value_t const& p1 ) const
                        {
                            buffer << 0x0c;
                            buffer.append( p1 );
                        }
                        
                        //
                        // ( eax, imm16/32 )
                        void op( Buffer& buffer, reg::types::ax_t const&, word_value_t const& p1 ) const
                        {
                            buffer << 0x0d;
                            buffer.append( p1 );
                        }
                        
                        void op( Buffer& buffer, reg::types::ax_t const&, dword_value_t const& p1 ) const
                        {
                            buffer << 0x0d;
                            buffer.append( p1 );
                        }
                        
                        void op( Buffer& buffer, reg::types::eax_t const&, word_value_t const& p1 ) const
                        {
                            buffer << 0x0d;
                            buffer.append( p1 );
                        }
                        
                        void op( Buffer& buffer, reg::types::eax_t const&, dword_value_t const& p1 ) const
                        {
                            buffer << 0x0d;
                            buffer.append( p1 );
                        }
                        
                    };
                } // namespace detail
                
                // interface
                template<typename BitsTag, typename Buffer>
                void OR( Buffer& buffer )
                {
                    generic_instruction_call<detail::OR_impl, BitsTag>( buffer );
                }
                template<typename BitsTag, typename Buffer, typename P0>
                void OR( Buffer& buffer, P0&& p0 )
                {
                    generic_instruction_call<detail::OR_impl, BitsTag>( buffer, std::forward<P0>( p0 ) );
                }
                template<typename BitsTag, typename Buffer, typename P0, typename P1>
                void OR( Buffer& buffer, P0&& p0, P1&& p1 )
                {
                    generic_instruction_call<detail::OR_impl, BitsTag>( buffer, std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
                }
                template<typename BitsTag, typename Buffer, typename P0, typename P1, typename P2>
                void OR( Buffer& buffer, P0&& p0, P1&& p1, P2&& p2 )
                {
                    generic_instruction_call<detail::OR_impl, BitsTag>( buffer, std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ) );
                }
                template<typename BitsTag, typename Buffer, typename P0, typename P1, typename P2, typename P3>
                void OR( Buffer& buffer, P0&& p0, P1&& p1, P2&& p2, P3&& p3 )
                {
                    generic_instruction_call<detail::OR_impl, BitsTag>( buffer, std::forward<P0>( p0 ), std::forward<P1>( p1 ), std::forward<P2>( p2 ), std::forward<P3>( p3 ) );
                }
                ////////////////////////////////////////////////////
                
            } // namespace instruction
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_INSTRUCTION_HPP*/
