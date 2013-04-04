#ifndef YTL_ASSEMBLER_X86_VALUE_HPP
#define YTL_ASSEMBLER_X86_VALUE_HPP

#include <array>

#include <boost/variant.hpp>
#include <boost/config.hpp>

#include "../../config.hpp"
#include "../../detail/raw_little_endian_method.hpp"

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
                    only_mod_rm( byte_t const m, byte_t const r )
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
                    only_sib( byte_t const s, byte_t const i, byte_t const b )
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

                    explicit only_disp( ValueType const v )
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
                    mod_rm_disp( only_mod_rm<N> const m, ValueType const d )
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
                    template<typename ScaleRegisterCode>
                    mod_rm32_sib( only_mod_rm<32> const m, sib_value<register_none_t, ScaleRegisterCode> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor_, ScaleRegisterCode::value, 5/* [*]:0b101 */ )
                    {}

                    template<typename BaseRegisterCode>
                    mod_rm32_sib( only_mod_rm<32> const m, sib_value<BaseRegisterCode, register_none_t> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor_, 4/* none:0b100 */, BaseRegisterCode::value )
                    {}

                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    mod_rm32_sib( only_mod_rm<32> const m, sib_value<BaseRegisterCode, ScaleRegisterCode> const& s )
                        : mod_rm( m )
                        , sib( s.scaling_factor_, ScaleRegisterCode::value, BaseRegisterCode::value )
                    {}

                    only_mod_rm<32> mod_rm;
                    only_sib sib;
                };


                template<typename ValueType>
                struct mod_rm32_sib_disp
                {
                    mod_rm32_sib_disp( mod_rm32_sib const m, ValueType const d )
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
                    template<byte_t Code>
                    mod_rm_registers( assembler::detail::register_code<Code, Tag> const& )
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
#define YTLASM_VARIANT_WRAPPER( name, bits, ... ) \
                template<> \
                struct name<bits> \
                { \
                    typedef __VA_ARGS__    variant_type; \
                     \
                    template<typename T> \
                    explicit name( T&& v ) \
                        : var( std::forward<T>( v ) ) \
                    {} \
                     \
                    variant_type var; \
                }; \
                typedef name <bits> name ## bits;


                template<unsigned int N> struct memory_access;

                // ----- Address : 16bit -----
                YTLASM_VARIANT_WRAPPER(
                    memory_access, 16,
                    boost::variant<
                        detail::only_mod_rm16,          // only mod_rm field(1Byte)
                        detail::mod_rm16_disp8,         // mod_rm(1Byte) & disp(1Byte) field
                        detail::mod_rm16_disp16         // mod_rm(1Byte) & disp(2Byte) field
                    >
                    )

                // ----- Address : 32bit -----
                YTLASM_VARIANT_WRAPPER(
                    memory_access, 32,
                    boost::variant<
                        detail::only_mod_rm32,          // only mod_rm field(1Byte)
                        detail::mod_rm32_disp8,         // mod_rm(1Byte) & disp(1Byte) field
                        detail::mod_rm32_disp32,        // mod_rm(1Byte) & disp(4Byte) field
                        detail::mod_rm32_sib,           // mod_rm(1Byte) & sib(1Byte) field
                        detail::mod_rm32_sib_disp8,     // mod_rm(1Byte) & sib(1Byte) field & disp(1Byte) field
                        detail::mod_rm32_sib_disp32     // mod_rm(1Byte) & sib(1Byte) field & disp(4Byte) field
                    >
                    )


                template<unsigned int N> struct r_m;

                // ----- 8bit -----
                // register access
                typedef detail::mod_rm_registers8 r8;

                // register access and memory
                YTLASM_VARIANT_WRAPPER(
                    r_m, 8,
                    boost::variant<
                        r8, memory_access16, memory_access32
                    >
                    )

                // ----- 16bit -----
                // register access
                typedef detail::mod_rm_registers16 r16;

                // register access and memory
                YTLASM_VARIANT_WRAPPER(
                    r_m, 16,
                    boost::variant<
                        r16, memory_access16, memory_access32
                    >
                    )

                // ----- 32bit -----
                // register access
                typedef detail::mod_rm_registers32 r32;

                // register access and memory
                YTLASM_VARIANT_WRAPPER(
                    r_m, 32,
                    boost::variant<
                        r32, memory_access16, memory_access32
                    >
                    )

#undef YTLASM_VARIANT_WRAPPER
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
                    YTL_CONSTEXPR memory_specifier() {}

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
                    inline auto operator[]( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, Code/*0b000 ~ 0b011*/ )
                            );
                    }
                    // [SI]
                    inline auto operator[]( reg::types::si_t const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, 4/*0b100*/ )
                            );
                    }
                    // [DI]
                    inline auto operator[]( reg::types::di_t const& ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            only_mod_rm16( 0/*0b00*/, 5/*0b101*/ )
                            );
                    }
                    // disp 16
                    inline auto operator[]( int16_t const& disp ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp16( only_mod_rm16( 0/*0b00*/, 6/*0b110*/ ), disp )
                            );
                    }
                    // [BX]
                    inline auto operator[]( reg::types::bx_t const& ) const
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
                    inline auto operator[]( detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, byte_value_t> const& r ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp8( only_mod_rm16( 1/*0b01*/, Code/*0b000 ~ 0b111*/ ), r.disp_ )
                            );
                    }

                    /*
                        Mod 0b10 -- [Registers] + disp16
                    */
                    // ([BX+SI], [BX+DI], [BP+SI], [BP+DI], [SI], [DI], [BP], [BX])+disp16
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline auto operator[]( detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, word_value_t> const& r ) const
                        -> return_16_type
                    {
                        return return_16_type(
                            mod_rm16_disp16( only_mod_rm16( 2/*0b10*/, Code/*0b000 ~ 0b111*/ ), r.disp_ )
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
                    inline auto operator[]( assembler::detail::register_code<Code, reg::types::reg_32bit_tag> const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            only_mod_rm32( 0/*0b00*/, Code/*index*/ )
                            );
                    }
                    // [ESP] as [SIB(nobase+ESP*scale(1))]
                    inline auto operator[]( reg::types::esp_t const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) )
                            );
                    }
                    //// [EBP] -> Mod 0b01
                    // [--][--]
                    template<typename BaseReg, typename ScaleReg>
                    inline auto operator[]( detail::sib_value<BaseReg, ScaleReg> const& sib ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), sib )
                            );
                    }
                    // [SIB(nobase+[--][--]] + disp(base 0)
                    template<typename ScaleReg>
                    inline auto operator[]( detail::sib_value<detail::register_none_t, ScaleReg> const& sib ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 0/*0b00*/, 4/*0b100*/ ), sib ), 0 )
                            );
                    }
                    // disp 32
                    inline auto operator[]( int32_t disp ) const
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
                    inline auto operator[]( reg::types::ebp_t const& /*reg*/ ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp8( only_mod_rm32( 1/*0b01*/, reg::types::ebp_t::value/*index*/ ), 0 )
                            );
                    }
                    // [EAX]+disp8, [ECX]+disp8, [EDX]+disp8, [EBX]+disp8, [EBP]+disp8, [ESI]+disp8, [EDI]+disp8
                    template<byte_t Code>
                    inline auto operator[]( detail::reg_disp_value<assembler::detail::register_code<Code, reg::types::reg_32bit_tag>, byte_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp8( only_mod_rm32( 1/*0b01*/, Code/*index*/ ), disp.value_ )
                            );
                    }
                    // [ESP]+disp8 as SIB[base(ESP)+none(scale:1)] + disp8
                    inline auto operator[]( detail::reg_disp_value<reg::types::esp_t, byte_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp8( mod_rm32_sib( only_mod_rm32( 1/*0b01*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) ), disp.value_ )
                            );
                    }
                    // [--][--]+disp8
                    template<typename BaseReg, typename ScaleReg>
                    inline auto operator[]( detail::sib_disp_value<BaseReg, ScaleReg, byte_value_t> const& sib_disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp8( mod_rm32_sib( only_mod_rm32( 1/*0b01*/, 4/*0b100*/ ), sib_disp.sib_ ), sib_disp.disp_ )
                            );
                    }


                    /*
                        Mod 0b10 -- [Register] + disp32 || [SIB] + disp32
                    */
                    // [EAX]+disp32, [ECX]+disp32, [EDX]+disp32, [EBX]+disp32, [EBP]+disp32, [ESI]+disp32, [EDI]+disp32
                    template<byte_t Code>
                    inline auto operator[]( detail::reg_disp_value<assembler::detail::register_code<Code, reg::types::reg_32bit_tag>, dword_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_disp32( only_mod_rm32( 2/*0b10*/, Code ), disp.value_ )
                            );
                    }
                    // [ESP]+disp32 as SIB[base(ESP)+none(scale:1)] + disp32
                    inline auto operator[]( detail::reg_disp_value<reg::types::esp_t, dword_value_t> const& disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 2/*0b10*/, 4/*0b100*/ ), detail::sib_value<reg::types::esp_t, register_none_t>( 1 ) ), disp.value_ )
                            );
                    }
                    // [--][--]+disp32
                    template<typename BaseReg, typename ScaleReg>
                    inline auto operator[]( detail::sib_disp_value<BaseReg, ScaleReg, dword_value_t> const& sib_disp ) const
                        -> return_32_type
                    {
                        return return_32_type(
                            mod_rm32_sib_disp32( mod_rm32_sib( only_mod_rm32( 2/*0b10*/, 4/*0b100*/), sib_disp.sib_ ), sib_disp.disp_ )
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
                    value_type_specifier() {}

                    template<typename T>
                    ValueType operator[]( T value ) const
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
                detail::memory_specifier const ptr;
                detail::value_type_specifier<byte_value_t> const byte;
                detail::value_type_specifier<word_value_t> const word;
                detail::value_type_specifier<dword_value_t> const dword;
            }


            //
            // buffers
            //
            namespace detail
            {
                namespace {
                    struct buffer_construct_tag {} const buffer_construct;
                }

                template<uint8_t MaxSize>
                struct short_buffer
                {
                    typedef short_buffer                                    self_type;
                    typedef uint8_t                                         size_type;

                    static size_type const max_size = MaxSize;

                    typedef assembler::detail::raw_little_endian_method     write_method_type;
                    typedef std::array<byte_t, max_size>                    buffer_type;

                public:
                    short_buffer()
                        : size( 0 )
                    {}

#if defined( BOOST_NO_VARIADIC_TEMPLATES )
# include <boost/preprocessor/repetition/enum.hpp>
# include <boost/preprocessor/repetition/enum_params.hpp>
# include <boost/preprocessor/repetition/enum_binary_params.hpp>
# include <boost/preprocessor/repetition/repeat_from_to.hpp>

#define ARR_P(z, n, unused) static_cast<typename buffer_type::value_type const>( p##n )
#define DEF_CTOR(z, n, unused) \
    template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
    short_buffer( buffer_construct_tag, BOOST_PP_ENUM_BINARY_PARAMS(n, T, const& p) ) \
    { \
        buffer_type arr = { BOOST_PP_ENUM(n, ARR_P, ) }; \
         \
        bin = std::move( arr ); \
        size = n; \
    }

                    short_buffer( buffer_construct_tag )
                    {
                        buffer_type arr = {};

                        bin = std::move( arr );
                        size = 0;
                    }
                    BOOST_PP_REPEAT_FROM_TO(1, 128/*temp*/, DEF_CTOR, )

#undef ARR_P
#undef DEF_CTOR

#else
                    template<typename... T>
                    short_buffer( buffer_construct_tag, T const&... xs )
                    {
                        buffer_type arr = { static_cast<typename buffer_type::value_type const>( xs )... };

                        bin = std::move( arr );
                        size = sizeof...( xs );
                    }
#endif

                public:
                    template<uint8_t NRhs>
                    bool operator==( short_buffer<NRhs> const& rhs ) const
                    {
                        if ( size != rhs.size )
                            return false;

                        return std::equal( bin.cbegin(), bin.cbegin() + size, rhs.bin.cbegin() );
                    }

                    template<uint8_t NRhs>
                    bool operator!=( short_buffer<NRhs> const& rhs ) const
                    {
                        return !( (*this) == rhs );
                    }

                public:
                    friend auto operator<<( self_type& lhs, byte_t const b ) -> self_type&
                    {
                        assert( lhs.size + 1 <= self_type::max_size );

                        lhs.bin[lhs.size] = b;
                        ++lhs.size;

                        return lhs;
                    }

                    template<uint8_t N>
                    friend auto operator<<( self_type& lhs, short_buffer<N> const& rhs ) -> self_type&
                    {
                        assert( lhs.size + rhs.size <= self_type::max_size );

                        write_method_type::write( lhs.bin.data() + lhs.size, rhs.bin.data(), rhs.size );
                        lhs.size += rhs.size;

                        return lhs;
                    }

                    template<typename T>
                    auto append( T const& b ) -> short_buffer&
                    {
                        assert( size + sizeof(T) <= MaxSize );

                        write_method_type::write( bin.data() + size, std::addressof( b ), sizeof(T) );
                        size += sizeof(T);

                        return *this;
                    }

                public:
                    buffer_type bin;
                    size_type size;
                };

                // for modr/m and sib and displacement(max 6bytes)
                typedef short_buffer<6> mod_rm_sib_disp_buffer;

                // for instruction(max 16bytes[15?])
                typedef short_buffer<16> instruction_buffer;

            } // namespace detail



            //
            // formatter
            //
            namespace detail
            {
                inline byte_t make_mod_rm( byte_t const mod, byte_t const op_code, byte_t const rm )
                {
                    return  ( ( mod << 6 ) & 0xc0       /*0b11000000*/ )
                          | ( ( op_code << 3 ) & 0x38   /*0b00111000*/ )
                          |   ( rm & 0x07               /*0b00000111*/ );
                }

                // for register only
                inline byte_t make_mod_rm( byte_t const op_code, byte_t const rm )
                {
                    return make_mod_rm( 3/*0b11*/, op_code, rm );
                }

                template<unsigned int N>
                inline byte_t make_mod_rm( detail::only_mod_rm<N> const& mod_rm_value, byte_t const reg )
                {
                    return make_mod_rm( mod_rm_value.mod, reg, mod_rm_value.rm );
                }


                inline byte_t make_sib( byte_t const ss, byte_t const index, byte_t const base )
                {
                    return  ( ( ss << 6 ) & 0xc0        /*0b11000000*/ )
                          | ( ( index << 3 ) & 0x38     /*0b00111000*/ )
                          |   ( base & 0x07             /*0b00000111*/ );
                }

                inline byte_t make_sib( detail::only_sib const& sib_value )
                {
                    return make_sib( sib_value.ss, sib_value.index, sib_value.base );
                }


                /*
                */
                class mod_rm_sib_disp_visitor
                    : public boost::static_visitor<mod_rm_sib_disp_buffer>
                {
                public:
                    typedef mod_rm_sib_disp_buffer    buffer_type;

                public:
                    mod_rm_sib_disp_visitor( byte_t const reg_op )
                        : reg_op_( reg_op )
                    {}

                public:
                    template<unsigned int N, typename Tag>
                    inline buffer_type operator()( detail::mod_rm_registers<N, Tag> const& v ) const
                    {
                        buffer_type b;
                        b << make_mod_rm( v.mod_rm, reg_op_ );

                        return b;
                    }

                    template<unsigned int N>
                    inline buffer_type operator()( types::memory_access<N> const& v ) const
                    {
                        return boost::apply_visitor( mod_rm_sib_disp_visitor( reg_op_ ), v.var );
                    }


                    template<typename T>
                    inline buffer_type operator()( T const& v ) const
                    {
                        std::cout << "!!!!!!!! -> " << typeid(T).name() << std::endl;

                        return buffer_type();
                    }

                public:
                    template<unsigned int N>
                    buffer_type operator()( detail::only_mod_rm<N> const& v ) const
                    {
                        buffer_type b;
                        b << make_mod_rm( v, reg_op_ );

                        return b;
                    }

                    template<unsigned int N, typename ValueType>
                    buffer_type operator()( detail::mod_rm_disp<N, ValueType> const& v ) const
                    {
                        buffer_type b;
                        b << make_mod_rm( v.mod_rm, reg_op_ );
                        b.append( v.disp );

                        return b;
                    }

                    buffer_type operator()( detail::mod_rm32_sib const& v ) const
                    {
                        buffer_type b;
                        b << make_mod_rm( v.mod_rm, reg_op_ );
                        b << make_sib( v.sib );

                        return b;
                    }

                    template<typename ValueType>
                    buffer_type operator()( detail::mod_rm32_sib_disp<ValueType> const& v ) const
                    {
                        buffer_type b;
                        b << make_mod_rm( v.mod_rm_sib.mod_rm, reg_op_ );
                        b << make_sib( v.mod_rm_sib.sib );
                        b.append( v.disp );

                        return b;
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
            auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const&, \
                types::r_m ## otherbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                return boost::apply_visitor( detail::mod_rm_sib_disp_visitor( Code ), v.var ); \
            } \
            template<byte_t Code> \
            inline auto generate_mod_rm_sib_disp( \
                types::r_m ## otherbit const& v, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                return generate_mod_rm_sib_disp( r, v ); \
            } \
            \
            template<byte_t Code0, byte_t Code1> \
            inline auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code0, reg::types::reg_ ## regbit ## bit_tag> const& p0, \
                assembler::detail::register_code<Code1, reg::types::reg_ ## otherbit ## bit_tag> const& p1 \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                return generate_mod_rm_sib_disp( p1, types::r_m ## regbit( detail::mod_rm_registers<regbit, reg::types::reg_ ## regbit ## bit_tag>( p0 ) ) ); \
            }

// opcode
#define YTLASM_GENERATE_MOD_RM_SIB_DISP_OP( regbit ) \
            inline auto generate_mod_rm_sib_disp_from_opcode( \
                byte_t const code, \
                types::r_m ## regbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                assert( code <= 7 ); \
                return boost::apply_visitor( detail::mod_rm_sib_disp_visitor( code ), v.var ); \
            } \
            \
            template<byte_t Code> \
            inline auto generate_mod_rm_sib_disp( \
                int const code, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& p0 \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                return generate_mod_rm_sib_disp_from_opcode( code, types::r_m ## regbit( types::r ## regbit( p0 ) ) ); \
            }

// memory access
#define YTLASM_GENERATE_MOD_RM_SIB_DISP_MA( regbit, otherbit ) \
            template<byte_t Code> \
            auto generate_mod_rm_sib_disp( \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r, \
                types::memory_access ## otherbit const& v \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
            { \
                return generate_mod_rm_sib_disp( r, types::r_m ## otherbit( v ) ); \
            } \
            template<byte_t Code> \
            inline auto generate_mod_rm_sib_disp( \
                types::memory_access ## otherbit const& v, \
                assembler::detail::register_code<Code, reg::types::reg_ ## regbit ## bit_tag> const& r \
                ) \
                -> detail::mod_rm_sib_disp_visitor::buffer_type \
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

#endif /*YTL_ASSEMBLER_X86_VALUE_HPP*/