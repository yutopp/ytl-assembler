#ifndef YTL_ASSEMBLER_X86_CALL_HPP
#define YTL_ASSEMBLER_X86_CALL_HPP

#include <type_traits>
#include <utility>

#include "mode.hpp"
#include "value.hpp"
#include "prefix.hpp"


namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            // method check
            namespace detail
            {
#define YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( name ) \
    template<template<typename> class Impl> \
    class has_pattern_ ## name

#define YTL_ASM_DEF_HAS_PATTERN( ... ) \
    template<typename U> \
    static auto check( U v ) -> decltype( __VA_ARGS__, std::true_type() );

#define YTL_ASM_DEF_HAS_PATTERN_IMPL_REST \
    static auto check( ... ) -> decltype( std::false_type() ); \
public: \
    typedef decltype( check( Impl<detail::empty_buffer/*dummy*/>() ) ) type; \
    static bool const value = type::value; \


                // ( r/m8, r8 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( rm8_r8 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( detail::empty_buffer(), std::declval<types::r_m8>(), reg::al/*8bit*/ )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

                // ( r/m16/32, r16/32 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( rm1632_r1632 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( detail::empty_buffer(), std::declval<types::r_m16>(), reg:: ax/*16bit*/ ),
                        v.op( detail::empty_buffer(), std::declval<types::r_m16>(), reg::eax/*32bit*/ ),
                        v.op( detail::empty_buffer(), std::declval<types::r_m32>(), reg:: ax/*16bit*/ ),
                        v.op( detail::empty_buffer(), std::declval<types::r_m32>(), reg::eax/*32bit*/ )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

                // ( r16/32, r/m16/32 )
                YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL( r1632_rm1632 )
                {
                public:
                    YTL_ASM_DEF_HAS_PATTERN(
                        v.op( detail::empty_buffer(), reg:: ax/*16bit*/, std::declval<types::r_m16>() ),
                        v.op( detail::empty_buffer(), reg:: ax/*16bit*/, std::declval<types::r_m32>() ),
                        v.op( detail::empty_buffer(), reg::eax/*32bit*/, std::declval<types::r_m16>() ),
                        v.op( detail::empty_buffer(), reg::eax/*32bit*/, std::declval<types::r_m32>() )
                        )

                    YTL_ASM_DEF_HAS_PATTERN_IMPL_REST
                };

#undef YTL_ASM_DEF_HAS_PATTERN_IMPL_DECL
#undef YTL_ASM_DEF_HAS_PATTERN
#undef YTL_ASM_DEF_HAS_PATTERN_IMPL_REST

            } // namespace detail

#define YTL_ASM_PATTERN( impl, name ) \
    detail::has_pattern_ ## name<impl>::value


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
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const 
                        -> decltype( buffer )
                    {
                        return buffer; // nothing to do
                    }
                };

                template<>
                struct size_prefix_injector<16, 32, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::address_size_override )
                    {
                        return buffer / prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 16, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::operand_size_override )
                    {
                        return buffer / prefix::operand_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 32, mode::bits_16_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::operand_size_override / prefix::address_size_override )
                    {
                        return buffer / prefix::operand_size_override / prefix::address_size_override;
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
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::operand_size_override / prefix::address_size_override )
                    {
                        return buffer / prefix::operand_size_override / prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<16, 32, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::operand_size_override )
                    {
                        return buffer / prefix::operand_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 16, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const
                        -> decltype( buffer / prefix::address_size_override )
                    {
                        return buffer / prefix::address_size_override;
                    }
                };

                template<>
                struct size_prefix_injector<32, 32, mode::bits_32_tag>
                {
                    template<typename Buffer>
                    inline YTL_CONSTEXPR auto operator()( Buffer const& buffer ) const 
                        -> decltype( buffer )
                    {
                        return buffer; // nothing to do
                    }
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


            // regN0, regN1 => r/mN0, regN1
            template<
                template<typename> class Impl,
                typename BitsTag,
                byte_t Code0, typename Tag0,
                byte_t Code1, typename Tag1
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                assembler::detail::register_code<Code0, Tag0> const& p0,
                assembler::detail::register_code<Code1, Tag1> const& p1
                ) -> decltype(
                        Impl<
                            decltype( detail::size_prefix_injector<Tag1::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                        >().op(
                            detail::size_prefix_injector<Tag1::value, BitsTag::value, BitsTag >()( detail::empty_buffer() ),
                            types::r_m<Tag0::value>( detail::mod_rm_registers<Tag0::value, Tag0>( p0 ) ),
                            p1
                            )
                     )
            {
                return Impl<
                            decltype( detail::size_prefix_injector<Tag1::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag1::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ),
                            types::r_m<Tag0::value>( detail::mod_rm_registers<Tag0::value, Tag0>( p0 ) ),
                            p1
                            );
            }

            // rN0, memN1(N0,N1 == 16 or 32) => rN0, r/mN0 if op has operand(rN0, r/mN1) and has no operand(rN0, memN1)
            template<
                template<typename> class Impl,
                typename BitsTag,
                byte_t Code0, typename Tag0,
                unsigned int Bits1
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                assembler::detail::register_code<Code0, Tag0> const& p0,
                types::memory_access<Bits1> const& p1,
                typename std::enable_if<
                        YTL_ASM_PATTERN( Impl, r1632_rm1632 ) // && !YTL_ASM_PATTERN( Impl, memN0 )
                >::type* =0
                ) -> decltype(
                        Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, Bits1, BitsTag>()( detail::empty_buffer() ) )
                        >().op(
                            detail::size_prefix_injector<Tag0::value, Bits1, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            types::r_m<Tag0::value>( p1 )
                            )
                     )
            {
                return Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, Bits1, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag0::value, Bits1, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            types::r_m<Tag0::value>( p1 )
                            );
            }

            // memN0, rN1(N0,N1 == 16 or 32) => r/mN1, rN1 if op has operand(r/mN0, rN1) and has no operand(memN0, rN1)
            template<
                template<typename> class Impl,
                typename BitsTag,
                unsigned int Bits0,
                byte_t Code1, typename Tag1
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                types::memory_access<Bits0> const& p0,
                assembler::detail::register_code<Code1, Tag1> const& p1,
                typename std::enable_if<
                    YTL_ASM_PATTERN( Impl, rm1632_r1632 ) // && !YTL_ASM_PATTERN( Impl, memN0 )
                >::type* = 0
                ) -> decltype(
                        Impl<
                            decltype( detail::size_prefix_injector<Tag1::value, Bits0, BitsTag>()( detail::empty_buffer() ) )
                        >().op(
                            detail::size_prefix_injector<Tag1::value, Bits0, BitsTag>()( detail::empty_buffer() ),
                            types::r_m<Tag1::value>( p0 ),
                            p1
                            )
                     )
            {
                return Impl<
                            decltype( detail::size_prefix_injector<Tag1::value, Bits0, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag1::value, Bits0, BitsTag>()( detail::empty_buffer() ),
                            types::r_m<Tag1::value>( p0 ),
                            p1
                            );
            }

            // AL, imm8
            template<
                template<typename> class Impl,
                typename BitsTag,
                typename NumType
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                reg::types::al_t const& p0,
                NumType const p1,
                typename std::enable_if<
                    std::is_integral<NumType>::value
                >::type* = 0
                ) -> decltype(
                        Impl<decltype( detail::empty_buffer() )>().op(
                            detail::empty_buffer(),
                            p0,
                            byte[0]
                            )
                     )
            {
                return Impl<decltype( detail::empty_buffer() )>().op(
                            detail::empty_buffer(),
                            p0,
                            byte[p1]
                            );
            }

            // (AX or EAX), imm16
            template<
                template<typename> class Impl,
                typename BitsTag,
                byte_t Code0, typename Tag0,
                typename NumType
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                assembler::detail::register_code<Code0, Tag0> const& p0,
                NumType const p1,
                typename std::enable_if<
                    (     std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::ax_t>::value
                       || std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::eax_t>::value )
                    && std::is_integral<NumType>::value
                    && ( sizeof( NumType ) <= sizeof( word_value_t ) )
                >::type* = 0
                ) -> decltype(
                        Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            word[0]
                            )
                     )
            {
                return Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            word[p1]
                            );
            }

            // (AX or EAX), imm32
            template<
                template<typename> class Impl,
                typename BitsTag,
                byte_t Code0, typename Tag0,
                typename NumType
            >
            inline YTL_CONSTEXPR auto generic_instruction_call(
                assembler::detail::register_code<Code0, Tag0> const& p0,
                NumType const& p1,
                typename std::enable_if<
                    (     std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::ax_t>::value
                       || std::is_same<assembler::detail::register_code<Code0, Tag0>, reg::types::eax_t>::value )
                    && std::is_integral<NumType>::value
                    && ( sizeof( NumType ) > sizeof( word_value_t ) )
                >::type* = 0
                ) -> decltype(
                        Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            dword[0]
                            )
                     )
            {
                return Impl<
                            decltype( detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ) )
                       >().op(
                            detail::size_prefix_injector<Tag0::value, BitsTag::value, BitsTag>()( detail::empty_buffer() ),
                            p0,
                            dword[p1]
                            );
            }







        } // namespace x86
    } // namesoace assembler
} // namespace ytl

#undef YTL_ASM_PATTERN


#endif /*YTL_ASSEMBLER_X86_CALL_HPP*/
