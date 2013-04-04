#ifndef YTL_ASSEMBLER_X86_CALL_HPP
#define YTL_ASSEMBLER_X86_CALL_HPP

#include <type_traits>
#include <utility>

#include "mode.hpp"
#include "value.hpp"
#include "prefix.hpp"


#define YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( name ) \
    template<template<typename> class Impl> \
    class has_pattern_ ## name

#define YTL_ASM_DEF_HAS_PATTERN( ... ) \
    template<typename U> \
    static auto check( U v ) -> decltype( __VA_ARGS__, std::true_type() );

#define YTL_ASM_DEF_HAS_PATTERN_IMPL_REST \
    static auto check( ... ) -> decltype( std::false_type() ); \
public: \
    typedef decltype( check( Impl<int/*dummy*/>() ) ) type; \
    static bool const value = type::value; \

#define YTL_ASM_PATTERN( impl, name ) \
    detail::has_pattern_ ## name<impl>::value

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            // method check
            namespace detail
            {
                // ( r/m8, r8 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( rm8_r8 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( std::declval<int&>(), std::declval<types::r_m8>(), reg::al/*8bit*/ )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

                // ( r/m16/32, r16/32 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( rm1632_r1632 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( std::declval<int&>(), std::declval<types::r_m16>(), reg:: ax/*16bit*/ ),
                        v.op( std::declval<int&>(), std::declval<types::r_m16>(), reg::eax/*32bit*/ ),
                        v.op( std::declval<int&>(), std::declval<types::r_m32>(), reg:: ax/*16bit*/ ),
                        v.op( std::declval<int&>(), std::declval<types::r_m32>(), reg::eax/*32bit*/ )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

                // ( r16/32, r/m16/32 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( r1632_rm1632 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( std::declval<int&>(), reg:: ax/*16bit*/, std::declval<types::r_m16>() ),
                        v.op( std::declval<int&>(), reg:: ax/*16bit*/, std::declval<types::r_m32>() ),
                        v.op( std::declval<int&>(), reg::eax/*32bit*/, std::declval<types::r_m16>() ),
                        v.op( std::declval<int&>(), reg::eax/*32bit*/, std::declval<types::r_m32>() )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

            } // namespace detail


            /*
                2 argument function call cushion
            */

            // size_prefix_injector
            namespace detail
            {
                template<byte_t OperandSize, byte_t AddressSize, typename BitsTag>
                struct size_prefix_injector;

                // 16 bit
                template<>
                struct size_prefix_injector<16, 16, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer const& ) const { /* nothing to do */ }
                };

                template<>
                struct size_prefix_injector<16, 32, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 16, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::operand_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 32, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::operand_size_override << prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<8, 16, mode::bits_16_tag>
                    : size_prefix_injector<16, 16, mode::bits_16_tag>
                {};

                template<>
                struct size_prefix_injector<8, 32, mode::bits_16_tag>
                    : size_prefix_injector<16, 32, mode::bits_16_tag>
                {};


                // 32 bit
                template<>
                struct size_prefix_injector<16, 16, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::operand_size_override;
                        buffer << prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<16, 32, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::operand_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 16, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer& buffer ) const
                    {
                        buffer << prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 32, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline void operator()( Buffer const& ) const { /* nothing to do */ }
                };

                template<>
                struct size_prefix_injector<8, 16, mode::bits_32_tag>
                    : size_prefix_injector<32, 16, mode::bits_32_tag>
                {};

                template<>
                struct size_prefix_injector<8, 32, mode::bits_32_tag>
                    : size_prefix_injector<32, 32, mode::bits_32_tag>
                {};

            } // namespace detail

            /*
            // generic
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                typename P0, typename P1
            >
            inline void generic_instruction_call(
                Buffer& buffer,
                P0&& p0,
                P1&& p1
                )
            {
                std::cout << "P0, P1(" << typeid( P0 ).name() << ", " << typeid( P1 ).name() << ")" << std::endl;

                Impl<Buffer>().op( buffer, std::forward<P0>( p0 ), std::forward<P1>( p1 ) );
            }*/

            // regN0, regN1 => r/mN0, regN1
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                byte_t Code0, typename Tag0,
                byte_t Code1, typename Tag1
            >
            inline void generic_instruction_call(
                Buffer& buffer,
                assembler::detail::register_code<Code0, Tag0> const& p0,
                assembler::detail::register_code<Code1, Tag1> const& p1
                )
            {
                std::cout << "regN0, regN1 = " << YTL_ASM_PATTERN( Impl, rm1632_r1632 ) << std::endl;


                detail::size_prefix_injector<Tag1::value, BitsTag::value, BitsTag>()( buffer );
                Impl<Buffer>().op(
                    buffer,
                    types::r_m<Tag0::value>( detail::mod_rm_registers<Tag0::value, Tag0>( p0 ) ),
                    p1
                    );
            }



            // rN0, memN1(N0,N1 == 16 or 32) => rN0, r/mN0 if op has operand(rN0, r/mN1) and has no operand(rN0, memN1)
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                byte_t Code0, typename Tag0,
                unsigned int Bits1
            >
            void generic_instruction_call(
                Buffer& buffer,
                assembler::detail::register_code<Code0, Tag0> const& p0,
                types::memory_access<Bits1> const& p1,
                typename std::enable_if<
                    YTL_ASM_PATTERN( Impl, r1632_rm1632 ) /* && !YTL_ASM_PATTERN( Impl, memN0 ) */
                >::type* = 0
                )
            {
                std::cout << "r" << static_cast<int>( Tag0::value ) << ", " << "r/m" << static_cast<int>( Bits1 ) << std::endl;

                detail::size_prefix_injector<Tag0::value, Bits1, BitsTag>()( buffer );
                Impl<Buffer>().op( buffer, p0, types::r_m<Tag0::value>( p1 ) );
            }

            // memN0, rN1(N0,N1 == 16 or 32) => r/mN1, rN1 if op has operand(r/mN0, rN1) and has no operand(memN0, rN1)
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                unsigned int Bits0,
                byte_t Code1, typename Tag1
            >
            void generic_instruction_call(
                Buffer& buffer,
                types::memory_access<Bits0> const& p0,
                assembler::detail::register_code<Code1, Tag1> const& p1,
                typename std::enable_if<
                    YTL_ASM_PATTERN( Impl, rm1632_r1632 ) /* && !YTL_ASM_PATTERN( Impl, memN0 ) */
                >::type* = 0
                )
            {
                std::cout << "r/m" << static_cast<int>( Bits0 ) << ", " << "r" << static_cast<int>( Tag1::value ) << std::endl;

                detail::size_prefix_injector<Tag1::value, Bits0, BitsTag>()( buffer );
                Impl<Buffer>().op( buffer, types::r_m<Tag1::value>( p0 ), p1 );
            }

            // AL, imm8
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                typename NumType
            >
            void generic_instruction_call(
                Buffer& buffer,
                reg::types::al_t const& p0,
                NumType const p1,
                typename std::enable_if<
                    std::is_integral<NumType>::value
                >::type* = 0
                )
            {
                std::cout << "AL, " << "imm8" << std::endl;

                Impl<Buffer>().op( buffer, p0, byte[p1] );
            }

            // (AX or EAX), imm16
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                byte_t Code0, typename Tag0,
                typename NumType
            >
            void generic_instruction_call(
                Buffer& buffer,
                assembler::detail::register_code<Code0, Tag0> const& p0,
                NumType const p1,
                typename std::enable_if<
                    (  std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::ax_t>::value
                    || std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::eax_t>::value )
                    && std::is_integral<NumType>::value
                    && ( sizeof( NumType ) <= sizeof( word_value_t ) )
                >::type* = 0
                )
            {
                std::cout << "[E]AX, " << "imm16" << std::endl;

                detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( buffer );
                Impl<Buffer>().op( buffer, p0, word[p1] );
            }

            // (AX or EAX), imm32
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename Buffer,
                byte_t Code0, typename Tag0,
                typename NumType
            >
            void generic_instruction_call(
                Buffer& buffer,
                assembler::detail::register_code<Code0, Tag0> const& p0,
                NumType const& p1,
                typename std::enable_if<
                    (  std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::ax_t>::value
                    || std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::eax_t>::value )
                    && std::is_integral<NumType>::value
                    && ( sizeof( NumType ) > sizeof( word_value_t ) )
                >::type* = 0
                )
            {
                std::cout << "[E]AX, " << "imm32" << std::endl;

                detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( buffer );
                Impl<Buffer>().op( buffer, p0, dword[p1] );
            }
            /**/






        } // namespace x86
    } // namesoace assembler
} // namespace ytl

#undef YTL_ASM_PATTERN
#undef YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL
#undef YTL_ASM_DEF_HAS_PATTERN
#undef YTL_ASM_DEF_HAS_PATTERN_IMPL_REST

#endif /*YTL_ASSEMBLER_X86_CALL_HPP*/
