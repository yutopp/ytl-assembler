#ifndef YTL_ASSEMBLER_X86_VALUE_HPP
#define YTL_ASSEMBLER_X86_VALUE_HPP

#include <ytl/base/config.hpp>
#if !defined(YTL_OLD_IMPL)
# include <ytl/base/constexpr_utility/variant.hpp>
# define YTL_ASM_VARIANT_NAMESPACE  ytl::cu
#else
# include <boost/variant.hpp>
# define YTL_ASM_VARIANT_NAMESPACE  boost
#endif

#include <ytl/base/constexpr_utility.hpp>
#include <ytl/base/buffer/short_buffer.hpp>

#include "../../config.hpp"

#include "basic.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            // kit types
            namespace detail
            {
                /* ----------------
                    express only ModR/M byte
                ---------------- */
                template<unsigned int N>
                struct only_mod_rm
                {
                    YTL_CONSTEXPR only_mod_rm() : mod(0), rm(0) {}

                    YTL_CONSTEXPR only_mod_rm( byte_t const m, byte_t const r )
                        : mod( m )
                        , rm( r )
                    {}

                    byte_t mod;
                    //// register/opcode field will be set later.
                    byte_t rm;
                };
                typedef only_mod_rm<8>      only_mod_rm8;   //
                typedef only_mod_rm<16>     only_mod_rm16;  //
                typedef only_mod_rm<32>     only_mod_rm32;  //


                /* ----------------
                -----------------*/
                struct only_sib
                {
                    YTL_CONSTEXPR only_sib() : ss(0), index(0), base(0) {};

                    YTL_CONSTEXPR only_sib( byte_t const s, byte_t const i, byte_t const b )
                        : ss( s )
                        , index( i )
                        , base( b )
                    {}

                    byte_t ss;     // 2bit
                    byte_t index;  // 3bit
                    byte_t base;   // 3bit
                };


                /* ----------------
                -----------------*/
                template<typename ValueType>
                struct only_disp
                {
                    static_assert(
                        sizeof(ValueType) == 0 ||
                        sizeof(ValueType) == 1 ||
                        sizeof(ValueType) == 2 ||
                        sizeof(ValueType) == 4,
                        ""
                        );

                    YTL_CONSTEXPR only_disp() : value(0) {};

                    explicit YTL_CONSTEXPR only_disp( ValueType const v )
                        : value( v )
                    {}

                    ValueType value;
                };


                /* ----------------
                    express ModR/M byte + displacement
                ---------------- */
                template<unsigned int N, typename ValueType>
                struct mod_rm_disp
                {
                    YTL_CONSTEXPR mod_rm_disp() {};

                    YTL_CONSTEXPR mod_rm_disp( only_mod_rm<N> const m, ValueType const d )
                        : mod_rm( m )
                        , disp( d )
                    {}

                    only_mod_rm<N> mod_rm;
                    only_disp<ValueType> disp;
                };
                typedef mod_rm_disp<16, byte_value_t>       mod_rm16_disp8;
                typedef mod_rm_disp<16, word_value_t>       mod_rm16_disp16;
                typedef mod_rm_disp<32, byte_value_t>       mod_rm32_disp8;
                typedef mod_rm_disp<32, dword_value_t>      mod_rm32_disp32;


                /* ----------------
                    express ModR/M byte + sib_byte
                --  sib bites appear only in 32bit addresing format.
                -----------------*/
                struct mod_rm32_sib
                {
                    YTL_CONSTEXPR mod_rm32_sib() {};

                    template<typename ScaleRegisterCode>
                    YTL_CONSTEXPR mod_rm32_sib( only_mod_rm<32> const m, sib_value<register_none_t, ScaleRegisterCode> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor, ScaleRegisterCode::value, 5/* [*]:0b101 */ )
                    {}

                    template<typename BaseRegisterCode>
                    YTL_CONSTEXPR mod_rm32_sib( only_mod_rm<32> const m, sib_value<BaseRegisterCode, register_none_t> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor, 4/* none:0b100 */, BaseRegisterCode::value )
                    {}

                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    YTL_CONSTEXPR mod_rm32_sib( only_mod_rm<32> const m, sib_value<BaseRegisterCode, ScaleRegisterCode> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor, ScaleRegisterCode::value, BaseRegisterCode::value )
                    {}

                    only_mod_rm<32> mod_rm;
                    only_sib sib;
                };


                template<typename ValueType>
                struct mod_rm32_sib_disp
                {
                    YTL_CONSTEXPR mod_rm32_sib_disp() {};

                    YTL_CONSTEXPR mod_rm32_sib_disp( mod_rm32_sib const m, ValueType const d )
                        : mod_rm_sib( m )
                        , disp( d )
                    {}

                    mod_rm32_sib mod_rm_sib;
                    only_disp<ValueType> disp;
                };
                typedef mod_rm32_sib_disp<byte_value_t>     mod_rm32_sib_disp8;
                typedef mod_rm32_sib_disp<dword_value_t>    mod_rm32_sib_disp32;
            

                // provide explicit conversion
                template<unsigned int N, typename Tag>
                struct mod_rm_registers
                {
                    YTL_CONSTEXPR mod_rm_registers() {};

                    template<byte_t Code>
                    YTL_CONSTEXPR mod_rm_registers( assembler::detail::register_code<Code, Tag> const& )
                        : mod_rm( 3/*0b11*/, Code )
                    {}

                    only_mod_rm<N> mod_rm;
                };
                typedef mod_rm_registers<8, reg::types::reg_8bit_tag>        mod_rm_registers8;
                typedef mod_rm_registers<16, reg::types::reg_16bit_tag>      mod_rm_registers16;
                typedef mod_rm_registers<32, reg::types::reg_32bit_tag>      mod_rm_registers32;

            } // namespace detail


            //
            //
            //
            namespace types
            {
#define YTL_ASM_VARIANT_TYPE( ns, ... ) \
                ns :: variant< __VA_ARGS__ >

#define YTL_ASM_VARIANT_WRAPPER( name, bits, val ) \
                template<> \
                struct name<bits> \
                { \
                    typedef YTL_ASM_VARIANT_TYPE val    variant_type; \
                     \
                    YTL_CONSTEXPR name() \
                        : var() \
                    {} \
                     \
                    template<typename T> \
                    explicit YTL_CONSTEXPR name( T&& v ) \
                        : var( std::forward<T>( v ) ) \
                    {} \
                     \
                    variant_type var; \
                }; \
                typedef name <bits> name ## bits;

                template<unsigned int N> struct memory_access;

                // ----- Address : 16bit -----
                YTL_ASM_VARIANT_WRAPPER(
                    memory_access, 16,
                    ( YTL_ASM_VARIANT_NAMESPACE,
                        detail::only_mod_rm16,          // only mod_rm field(1Byte)
                        detail::mod_rm16_disp8,         // mod_rm(1Byte) & disp(1Byte) field
                        detail::mod_rm16_disp16         // mod_rm(1Byte) & disp(2Byte) field
                        )
                    )

                // ----- Address : 32bit -----
                YTL_ASM_VARIANT_WRAPPER(
                    memory_access, 32,
                    ( YTL_ASM_VARIANT_NAMESPACE,
                        detail::only_mod_rm32,          // only mod_rm field(1Byte)
                        detail::mod_rm32_disp8,         // mod_rm(1Byte) & disp(1Byte) field
                        detail::mod_rm32_disp32,        // mod_rm(1Byte) & disp(4Byte) field
                        detail::mod_rm32_sib,           // mod_rm(1Byte) & sib(1Byte) field
                        detail::mod_rm32_sib_disp8,     // mod_rm(1Byte) & sib(1Byte) field & disp(1Byte) field
                        detail::mod_rm32_sib_disp32     // mod_rm(1Byte) & sib(1Byte) field & disp(4Byte) field
                        )
                    )


                template<unsigned int N> struct r_m;

                // ----- 8bit -----
                // register access
                typedef detail::mod_rm_registers8 r8;

                // register access and memory
                YTL_ASM_VARIANT_WRAPPER(
                    r_m, 8,
                    ( YTL_ASM_VARIANT_NAMESPACE,
                        r8, memory_access16, memory_access32
                        )
                    )

                // ----- 16bit -----
                // register access
                typedef detail::mod_rm_registers16 r16;

                // register access and memory
                YTL_ASM_VARIANT_WRAPPER(
                    r_m, 16,
                    ( YTL_ASM_VARIANT_NAMESPACE,
                        r16, memory_access16, memory_access32
                        )
                    )

                // ----- 32bit -----
                // register access
                typedef detail::mod_rm_registers32 r32;

                // register access and memory
                YTL_ASM_VARIANT_WRAPPER(
                    r_m, 32,
                    ( YTL_ASM_VARIANT_NAMESPACE,
                        r32, memory_access16, memory_access32
                        )
                    )

#undef YTL_ASM_VARIANT_TYPE
#undef YTL_ASM_VARIANT_WRAPPER
            }


            //
            // specifier impl
            //
            namespace detail
            {
                /*
                    32bit pointer annotation
                */
                struct memory_specifier
                {
                    /* ---------
                        16bit ModR/M
                    --------- */
                    typedef types::memory_access16     return_16_type;

                    /*
                        Mod 0b00 -- [Registers] || disp8
                    */
                    //
                    // [BX+SI], [BX+DI], [BP+SI], [BP+DI]
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, Code/*0b000 ~ 0b011*/ )
                            );
                    }
                    // [SI]
                    inline YTL_CONSTEXPR auto operator[]( reg::types::si_t const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, 4/*0b100*/ )
                            );
                    }
                    // [DI]
                    inline YTL_CONSTEXPR auto operator[]( reg::types::di_t const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, 5/*0b101*/ )
                            );
                    }
                    // disp 16
                    inline YTL_CONSTEXPR auto operator[]( int16_t const& disp ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp16( only_mod_rm16( 0/*0b00*/, 6/*0b110*/ ), disp )
                            );
                    }
                    // [BX]
                    inline YTL_CONSTEXPR auto operator[]( reg::types::bx_t const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, 7/*0b111*/ )
                            );
                    }

                    /*
                        Mod 0b01 -- [Registers] + disp8
                    */
                    // ([BX+SI], [BX+DI], [BP+SI], [BP+DI], [SI], [DI], [BP], [BX])+disp8
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, byte_value_t> const& disp ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp8( only_mod_rm16( 1/*0b01*/, Code/*0b000 ~ 0b111*/ ), disp.value )
                            );
                    }

                    /*
                        Mod 0b10 -- [Registers] + disp16
                    */
                    // ([BX+SI], [BX+DI], [BP+SI], [BP+DI], [SI], [DI], [BP], [BX])+disp16
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, word_value_t> const& disp ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp16( only_mod_rm16( 2/*0b10*/, Code/*0b000 ~ 0b111*/ ), disp.value )
                            );
                    }

                    // Mod 0b11
                    // -> solve by call.hpp





                    /* ---------
                        32bit ModR/M
                    --------- */
                    typedef types::memory_access32     return_32_type;
                    /*
                        Mod 0b00 -- [Register] || [SIB] || disp32
                    */
                    // [EAX], [ECX], [EDX], [EBX], [ESI], [EDI]
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            only_mod_rm32( 0/*0b00*/, Code/*index*/ )
                            );
                    }
                    // [ESP] as [SIB(nobase+ESP*scale(1))]
                    inline YTL_CONSTEXPR auto operator[]( reg::types::esp_t const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) )
                            );
                    }
                    //// [EBP] -> Mod 0b01
                    // [--][--]
                    template<typename BaseReg, typename ScaleReg>
                    inline YTL_CONSTEXPR auto operator[]( detail::sib_value<BaseReg, ScaleReg> const& sib ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), sib )
                            );
                    }
                    // [SIB(nobase+[--][--]] + disp(base 0)
                    template<typename ScaleReg>
                    inline YTL_CONSTEXPR auto operator[]( detail::sib_value<detail::register_none_t, ScaleReg> const& sib ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), sib ), 0 )
                            );
                    }
                    // disp 32
                    inline YTL_CONSTEXPR auto operator[]( int32_t disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            return_32_type( mod_rm32_disp32( only_mod_rm32( 0/*0b00*/, 5/*0b101*/), disp ) )
                            );
                    }

                    
                    /*
                        Mod 0b01 -- [Register] + disp8 || [SIB] + disp8
                    */
                    // [EBP] as [EBP] + disp8(0)
                    inline YTL_CONSTEXPR auto operator[]( reg::types::ebp_t const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp8( only_mod_rm32( 1/*0b01*/, reg::types::ebp_t::value/*index*/ ), 0 )
                            );
                    }
                    // [EAX]+disp8, [ECX]+disp8, [EDX]+disp8, [EBX]+disp8, [EBP]+disp8, [ESI]+disp8, [EDI]+disp8
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( detail::reg_disp_value<assembler::detail::register_code<Code, reg::types::reg_32bit_tag>, byte_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp8( only_mod_rm32( 1/*0b01*/, Code/*index*/ ), disp.value )
                            );
                    }
                    // [ESP]+disp8 as SIB[base(ESP)+none(scale:1)] + disp8
                    inline YTL_CONSTEXPR auto operator[]( detail::reg_disp_value<reg::types::esp_t, byte_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp8( mod_rm32_sib( only_mod_rm32( 1/*0b01*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) ), disp.value )
                            );
                    }
                    // [--][--]+disp8
                    template<typename BaseReg, typename ScaleReg>
                    inline YTL_CONSTEXPR auto operator[]( detail::sib_disp_value<BaseReg, ScaleReg, byte_value_t> const& sib_disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp8( mod_rm32_sib( only_mod_rm32( 1/*0b01*/, 4/*0b100*/ ), sib_disp.sib ), sib_disp.value )
                            );
                    }


                    /*
                        Mod 0b10 -- [Register] + disp32 || [SIB] + disp32
                    */
                    // [EAX]+disp32, [ECX]+disp32, [EDX]+disp32, [EBX]+disp32, [EBP]+disp32, [ESI]+disp32, [EDI]+disp32
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator[]( detail::reg_disp_value<assembler::detail::register_code<Code, reg::types::reg_32bit_tag>, dword_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp32( only_mod_rm32( 2/*0b10*/, Code ), disp.value )
                            );
                    }
                    // [ESP]+disp32 as SIB[base(ESP)+none(scale:1)] + disp32
                    inline YTL_CONSTEXPR auto operator[]( detail::reg_disp_value<reg::types::esp_t, dword_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 2/*0b10*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) ), disp.value )
                            );
                    }
                    // [--][--]+disp32
                    template<typename BaseReg, typename ScaleReg>
                    inline YTL_CONSTEXPR auto operator[]( detail::sib_disp_value<BaseReg, ScaleReg, dword_value_t> const& sib_disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 2/*0b10*/, 4/*0b100*/), sib_disp.sib ), sib_disp.value )
                            );
                    }

                    // Mod 0b11
                    // -> solve by call.hpp
                };

                /*
                */
                template<typename ValueType>
                struct value_type_specifier
                {
                    template<typename T>
                    YTL_CONSTEXPR ValueType operator[]( T value ) const
                    {
                        return ValueType( value );
                    }
                };
            } // namespace detail

            //
            // specifier
            //
            namespace
            {
                YTL_CONSTEXPR auto const ptr = detail::memory_specifier();
                YTL_CONSTEXPR auto const byte = detail::value_type_specifier<byte_value_t>();
                YTL_CONSTEXPR auto const word = detail::value_type_specifier<word_value_t>();
                YTL_CONSTEXPR auto const dword = detail::value_type_specifier<dword_value_t>();
            }


            //
            // buffers
            //
            namespace detail
            {
                typedef ytl::detail::short_buffer<uint8_t, 0> empty_buffer;

                // for modr/m and sib and displacement(max 6bytes)
                typedef ytl::detail::short_buffer<uint8_t, 6> mod_rm_sib_disp_buffer;

                // for instruction(max 16bytes[15?])
                typedef ytl::detail::short_buffer<uint8_t, 16> instruction_buffer;

            } // namespace detail



            //
            // formatter
            //
            namespace detail
            {
                inline YTL_CONSTEXPR byte_t make_mod_rm( byte_t const mod, byte_t const op_code, byte_t const rm )
                {
                    return  ( ( mod << 6 ) & 0xc0       /*0b11000000*/ )
                          | ( ( op_code << 3 ) & 0x38   /*0b00111000*/ )
                          |   ( rm & 0x07               /*0b00000111*/ );
                }

                // for register only
                inline YTL_CONSTEXPR byte_t make_mod_rm( byte_t const op_code, byte_t const rm )
                {
                    return make_mod_rm( 3/*0b11*/, op_code, rm );
                }

                template<unsigned int N>
                inline YTL_CONSTEXPR byte_t make_mod_rm( detail::only_mod_rm<N> const& mod_rm_value, byte_t const reg )
                {
                    return make_mod_rm( mod_rm_value.mod, reg, mod_rm_value.rm );
                }


                inline YTL_CONSTEXPR byte_t make_sib( byte_t const ss, byte_t const index, byte_t const base )
                {
                    return  ( ( ss << 6 ) & 0xc0        /*0b11000000*/ )
                          | ( ( index << 3 ) & 0x38     /*0b00111000*/ )
                          |   ( base & 0x07             /*0b00000111*/ );
                }

                inline YTL_CONSTEXPR byte_t make_sib( detail::only_sib const& sib_value )
                {
                    return make_sib( sib_value.ss, sib_value.index, sib_value.base );
                }



                /*
                */
                class mod_rm_sib_disp_visitor
                {
                public:
                    typedef mod_rm_sib_disp_buffer      result_type;

                public:
                    YTL_CONSTEXPR mod_rm_sib_disp_visitor( byte_t const reg_op )
                        : reg_op_( reg_op )
                    {}

                public:
                    template<unsigned int N, typename Tag>
                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm_registers<N, Tag> const& v ) const
                    {
                        return ytl::detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ )
                                    );
                    }

                    template<unsigned int N>
                    inline YTL_CONSTEXPR result_type operator()( types::memory_access<N> const& v ) const
                    {
                        return YTL_ASM_VARIANT_NAMESPACE ::apply_visitor( mod_rm_sib_disp_visitor( reg_op_ ), v.var );
                    }

                public:
                    template<unsigned int N>
                    inline YTL_CONSTEXPR result_type operator()( detail::only_mod_rm<N> const& v ) const
                    {
                        return ytl::detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v, reg_op_ )
                                    );
                    }

                    template<unsigned int N, typename ValueType>
                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm_disp<N, ValueType> const& v ) const
                    {
                        return ytl::detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ ) / v.disp.value
                                    );
                    }

                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm32_sib const& v ) const
                    {
                        return ytl::detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ ) / make_sib( v.sib )
                                    );
                    }

                    template<typename ValueType>
                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm32_sib_disp<ValueType> const& v ) const
                    {
                        return ytl::detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm_sib.mod_rm, reg_op_ ) / make_sib( v.mod_rm_sib.sib ) / v.disp.value
                                    );
                    }

                private:
                    byte_t reg_op_;
                };

            } // namespace detail


            /*
                generate_mod_rm_sib_disp( p0, p1 )
                  ( r_mN, rN ) :: /r value is register code of rN
                  ( rN, r_mN ) :: /r value is register code of rN
                  ( r0N, r1N ) :: /r value is register code of r1N
            */
#define YTLASM_GENERATE_MOD_RM_SIB_DISP( regbit, otherbit ) \
            template<byte_t Code> \
            YTL_CONSTEXPR auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const&, \
                types::r_m ## otherbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return YTL_ASM_VARIANT_NAMESPACE ::apply_visitor( detail::mod_rm_sib_disp_visitor( Code ), v.var ); \
            } \
            template<byte_t Code> \
            YTL_CONSTEXPR inline auto generate_mod_rm_sib_disp( \
                types::r_m ## otherbit const& v, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return generate_mod_rm_sib_disp( r, v ); \
            } \
            \
            template<byte_t Code0, byte_t Code1> \
            YTL_CONSTEXPR inline auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code0, reg::types::reg_ ## regbit ## bit_tag> const& p0, \
                assembler::detail::register_code<Code1, reg::types::reg_ ## otherbit ## bit_tag> const& p1 \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return generate_mod_rm_sib_disp( p1, types::r_m ## regbit( detail::mod_rm_registers<regbit, reg::types::reg_ ## regbit ## bit_tag>( p0 ) ) ); \
            }

// opcode
#define YTLASM_GENERATE_MOD_RM_SIB_DISP_OP( regbit ) \
            inline YTL_CONSTEXPR auto generate_mod_rm_sib_disp_from_opcode( \
                byte_t const code, \
                types::r_m ## regbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return ( code <= 7 ) \
                            ? YTL_ASM_VARIANT_NAMESPACE ::apply_visitor( detail::mod_rm_sib_disp_visitor( code ), v.var ) \
                            : throw ""; \
            } \
            \
            template<byte_t Code> \
            inline YTL_CONSTEXPR auto generate_mod_rm_sib_disp( \
                byte_t const code, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& p0 \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return generate_mod_rm_sib_disp_from_opcode( code, types::r_m ## regbit( types::r ## regbit( p0 ) ) ); \
            }

// memory access
#define YTLASM_GENERATE_MOD_RM_SIB_DISP_MA( regbit, otherbit ) \
            template<byte_t Code> \
            inline YTL_CONSTEXPR auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r, \
                types::memory_access ## otherbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return generate_mod_rm_sib_disp( r, types::r_m ## otherbit( v ) ); \
            } \
            template<byte_t Code> \
            inline YTL_CONSTEXPR auto generate_mod_rm_sib_disp( \
                types::memory_access ## otherbit const& v, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r \
                ) \
                -> detail::mod_rm_sib_disp_visitor::result_type \
            { \
                return generate_mod_rm_sib_disp( r, types::r_m ## otherbit( v ) ); \
            }

            // generate
            YTLASM_GENERATE_MOD_RM_SIB_DISP(8, 8)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(8, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(8, 32)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(16, 8)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(16, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(16, 32)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(32, 8)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(32, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP(32, 32)

            YTLASM_GENERATE_MOD_RM_SIB_DISP_OP(8)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_OP(16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_OP(32)

            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(8, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(8, 32)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(16, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(16, 32)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(32, 16)
            YTLASM_GENERATE_MOD_RM_SIB_DISP_MA(32, 32)

#undef YTLASM_GENERATE_MOD_RM_SIB_DISP
#undef YTLASM_GENERATE_MOD_RM_SIB_DISP_OP
#undef YTLASM_GENERATE_MOD_RM_SIB_DISP_MA
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#undef YTL_ASM_VARIANT_NAMESPACE

#endif /*YTL_ASSEMBLER_X86_VALUE_HPP*/