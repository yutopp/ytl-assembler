#ifndef YTL_ASSEMBLER_X86_VALUE_HPP
#define YTL_ASSEMBLER_X86_VALUE_HPP

#include <array>

#if !defined(YTL_OLD_IMPL)

#include <ytl/base/constexpr_utility/variant.hpp>
#define YTL_ASM_VARIANT_NAMESPACE ytl::cu

#else

#include <boost/variant.hpp>
#define YTL_ASM_VARIANT_NAMESPACE boost

#endif

#include <boost/config.hpp>
#include <boost/integer.hpp>

#include <ytl/base/constexpr_utility.hpp>

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
#define YTLASM_VARIANT_WRAPPER( name, bits, ... ) \
                template<> \
                struct name<bits> \
                { \
                    typedef __VA_ARGS__    variant_type; \
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
                YTLASM_VARIANT_WRAPPER(
                    memory_access, 16,
                    YTL_ASM_VARIANT_NAMESPACE::variant<
                        detail::only_mod_rm16,          // only mod_rm field(1Byte)
                        detail::mod_rm16_disp8,         // mod_rm(1Byte) & disp(1Byte) field
                        detail::mod_rm16_disp16         // mod_rm(1Byte) & disp(2Byte) field
                    >
                    )

                // ----- Address : 32bit -----
                YTLASM_VARIANT_WRAPPER(
                    memory_access, 32,
                    YTL_ASM_VARIANT_NAMESPACE::variant<
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
                    YTL_ASM_VARIANT_NAMESPACE::variant<
                        r8, memory_access16, memory_access32
                    >
                    )

                // ----- 16bit -----
                // register access
                typedef detail::mod_rm_registers16 r16;

                // register access and memory
                YTLASM_VARIANT_WRAPPER(
                    r_m, 16,
                    YTL_ASM_VARIANT_NAMESPACE::variant<
                        r16, memory_access16, memory_access32
                    >
                    )

                // ----- 32bit -----
                // register access
                typedef detail::mod_rm_registers32 r32;

                // register access and memory
                YTLASM_VARIANT_WRAPPER(
                    r_m, 32,
                    YTL_ASM_VARIANT_NAMESPACE::variant<
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
                namespace
                {
                    struct buffer_construct_tag {};
                    YTL_CONSTEXPR auto const buffer_construct = buffer_construct_tag();

                    struct buffer_construct_n_tag {};
                    YTL_CONSTEXPR auto const buffer_construct_n = buffer_construct_n_tag();
                }


#if !defined(YTL_OLD_IMPL)

                template<typename SizeType, SizeType MaxSize>
                class short_buffer;


                template<typename T, std::size_t... Indexes>
                inline YTL_CONSTEXPR auto to_binary_little( T const& val, ytl::cu::index_tuple<std::size_t, Indexes...> const& )
                    -> assembler::x86::detail::short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>;

                template<typename T>
                inline YTL_CONSTEXPR auto to_binary_little( T const& val )
                    -> assembler::x86::detail::short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>;


                template<
                    typename SizeType1, SizeType1 MaxSize1, std::size_t... Indexes1,
                    typename SizeType2, SizeType2 MaxSize2, std::size_t... Indexes2
                >
                inline YTL_CONSTEXPR auto concat(
                    short_buffer<SizeType1, MaxSize1> const& p1, ytl::cu::index_tuple<std::size_t, Indexes1...> const&,
                    short_buffer<SizeType2, MaxSize2> const& p2, ytl::cu::index_tuple<std::size_t, Indexes2...> const&
                    ) -> assembler::x86::detail::short_buffer<
                                typename std::conditional<
                                    ( std::numeric_limits<SizeType1>::max() > std::numeric_limits<SizeType2>::max() ),
                                    SizeType1,
                                    SizeType2
                                >::type,
                                ( MaxSize1 + MaxSize2 )
                         >;

                template<typename SizeType1, SizeType1 MaxSize1, typename SizeType2, SizeType2 MaxSize2>
                inline YTL_CONSTEXPR auto concat(
                    short_buffer<SizeType1, MaxSize1> const& p1,
                    short_buffer<SizeType2, MaxSize2> const& p2
                    ) -> decltype(
                            concat(
                                p1, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize1>(),
                                p2, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize2>()
                                )
                         );


                template<typename Arr1, std::size_t N1, typename Arr2, std::size_t N2>
                inline YTL_CONSTEXPR bool equal(
                    Arr1 const (&arr1)[N1],
                    Arr2 const (&arr2)[N2],
                    int const index = 0
                    )
                {
                    return index == ( N1 < N2 ? N1 : N2 )
                            ? true
                            : ( arr1[index] == arr2[index]
                                    ? equal( arr1, arr2, index + 1 )
                                    : false
                                    );
                }


                // Implementation for GCC and clang!
                template<typename SizeType, SizeType MaxSize>
                class short_buffer
                {
                public:
                    typedef short_buffer                                    self_type;
                    typedef SizeType                                        size_type;

                    static size_type const                                  max_size = MaxSize;

                private:
                    template<typename T, size_type N>
                    struct array
                    {
                        typedef T       value_type;
                        T data[N == 0 ? 1 : N];
                    };

                    template<typename OtherSizeType, OtherSizeType OtherMaxSize>
                    friend class short_buffer;

                public:
                    typedef array<byte_t, max_size>                         buffer_type;
                    typedef typename buffer_type::value_type                value_type;

                public:
                    explicit YTL_CONSTEXPR short_buffer()
                        : bin_{{}}
                        , size_{ 0 }
                    {}

                    template<typename... T>
                    explicit YTL_CONSTEXPR short_buffer( buffer_construct_tag, T const... xs )
                        : bin_{{ static_cast<value_type const>( xs )... }}
                        , size_{ sizeof...( xs ) }
                    {}

                    template<typename... T>
                    explicit YTL_CONSTEXPR short_buffer( buffer_construct_n_tag, size_type const size, T const... xs )
                        : bin_{{ static_cast<value_type const>( xs )... }}
                        , size_{ size }
                    {}
                    
                public:
                    template<typename SizeTypeR, SizeTypeR MaxSizeR>
                    YTL_CONSTEXPR bool operator==( short_buffer<SizeTypeR, MaxSizeR> const& rhs ) const
                    {
                        return ( size_ == rhs.size_ ) && ( equal( bin_.data, rhs.bin_.data ) );
                    }

                    template<typename SizeTypeR, SizeTypeR MaxSizeR>
                    YTL_CONSTEXPR bool operator!=( short_buffer<SizeTypeR, MaxSizeR> const& rhs ) const
                    {
                        return !( (*this) == rhs );
                    }

                public:
                    inline YTL_CONSTEXPR value_type operator[]( std::size_t const& index ) const
                    {
                        return bin_.data[index];
                    }

                    inline YTL_CONSTEXPR size_type size() const
                    {
                        return size_;
                    }

                    inline value_type const* data() const
                    {
                        return bin_.data;
                    }
                    

                    template<typename SizeTypeRhs, SizeTypeRhs MaxSizeRhs>
                    friend YTL_CONSTEXPR auto operator/(
                        self_type const& lhs,
                        short_buffer<SizeTypeRhs, MaxSizeRhs> const& rhs
                        ) -> assembler::x86::detail::short_buffer<
                                typename std::conditional<
                                    ( std::numeric_limits<SizeType>::max() > std::numeric_limits<SizeTypeRhs>::max() ),
                                    SizeType,
                                    SizeTypeRhs
                                >::type,
                                ( MaxSize + MaxSizeRhs )
                         >
                    {
                        return concat( lhs, rhs );
                    }

                    template<typename T>
                    friend YTL_CONSTEXPR auto operator/(
                        self_type const& lhs,
                        T const& b
                        ) -> typename std::enable_if<
                                    std::is_arithmetic<T>::value,
                                    assembler::x86::detail::short_buffer<
                                        SizeType,
                                        ( MaxSize + sizeof( b ) )
                                    >
                             >::type
                    {
                        return concat( lhs, to_binary_little( b ) );
                    }
/*
                    template<typename T>
                    auto append_raw_data( T const& b ) -> short_buffer&
                    {
//                        assert( size + sizeof(T) <= MaxSize );
//
//                        write_method_type::write( bin.data() + size, std::addressof( b ), sizeof(T) );
//                        size += sizeof(T);

                        return *this;
                    }*/

                private:
                    buffer_type bin_;
                    size_type size_;
                };



                template<typename T>
                struct little_endian_byte_access_impl
                {
                    inline YTL_CONSTEXPR byte_t operator()( T const& val, std::size_t const index )
                    {
#if defined(YTL_LITTLE_ENDIAN)
                        return ( val & ( 0xff << ( index * 8/*bits*/ ) ) ) >> ( index * 8/*bits*/ );
#elif defined(YTL_BIG_ENDIAN)
                        return ( val & ( 0xff << ( ( sizeof( T ) - index - 1 ) * 8/*bits*/ ) ) ) >> ( ( sizeof( T ) - index - 1 ) * 8/*bits*/ );
#endif
                    }
                };

                template<typename T>
                inline YTL_CONSTEXPR byte_t little_endian_byte_access( T const& val, std::size_t const index )
                {
                    return little_endian_byte_access_impl<T>()( val, index );
                }


                template<typename T, std::size_t... Indexes>
                inline YTL_CONSTEXPR auto to_binary_little( T const& val, ytl::cu::index_tuple<std::size_t, Indexes...> const& )
                    -> assembler::x86::detail::short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>
                {
                    using namespace ytl::assembler::x86::detail;

                    return assembler::x86::detail::short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>(
                        buffer_construct, 
                        little_endian_byte_access( val, Indexes )...
                        );
                }

                template<typename T>
                inline YTL_CONSTEXPR auto to_binary_little( T const& val )
                    -> assembler::x86::detail::short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>
                {
                    return to_binary_little( val, ytl::cu::make_index_tuple<std::size_t, 0, sizeof(T)>() );
                }


                template<std::size_t Index, typename SizeType, SizeType MaxSize>
                inline YTL_CONSTEXPR auto element(
                    short_buffer<SizeType, MaxSize> const& t,
                    typename short_buffer<SizeType, MaxSize>::value_type const val
                    ) -> typename short_buffer<SizeType, MaxSize>::value_type
                {
                    return Index < t.size() ? t[Index] : val;
                }


                template<typename SizeType, SizeType MaxSize, std::size_t... Indexes>
                inline YTL_CONSTEXPR auto write_back(
                    short_buffer<SizeType, MaxSize> const& t, ytl::cu::index_tuple<std::size_t, Indexes...> const&,
                    typename short_buffer<SizeType, MaxSize>::value_type const val
                    ) -> short_buffer<SizeType, MaxSize>
                {
                    return short_buffer<SizeType, MaxSize>( buffer_construct_n, t.size() + 1, element<Indexes>( t, val )... );
                    /*return t.size() < MaxSize
                            ? short_buffer<SizeType, MaxSize>( buffer_construct_n, t.size() + 1, element<Indexes>( t, val )... )
                            : throw "";*/
                }

                template<typename SizeType, SizeType MaxSize>
                inline YTL_CONSTEXPR auto write_back(
                    short_buffer<SizeType, MaxSize> const& t,
                    typename short_buffer<SizeType, MaxSize>::value_type const val
                    ) -> decltype(
                            write_back( t, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize>(), val )
                         )
                {
                    return write_back( t, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize>(), val );
                }


                template<
                    typename SizeType1, SizeType1 MaxSize1,
                    typename SizeType2, SizeType2 MaxSize2, std::size_t Index2
                >
                inline YTL_CONSTEXPR auto concat_impl(
                    short_buffer<SizeType1, MaxSize1> const& p1,
                    short_buffer<SizeType2, MaxSize2> const& p2, ytl::cu::index_tuple<std::size_t, Index2> const&
                    ) -> short_buffer<SizeType1, MaxSize1>
                {
                    return Index2 < p2.size()
                                ? write_back( p1, p2[Index2] )
                                : p1
                                ;
                }

                template<
                    typename SizeType1, SizeType1 MaxSize1,
                    typename SizeType2, SizeType2 MaxSize2, std::size_t Index2, std::size_t... Indexes2
                >
                inline YTL_CONSTEXPR auto concat_impl(
                    short_buffer<SizeType1, MaxSize1> const& p1,
                    short_buffer<SizeType2, MaxSize2> const& p2, ytl::cu::index_tuple<std::size_t, Index2, Indexes2...> const&
                    ) -> short_buffer<SizeType1, MaxSize1>
                {
                    return Index2 < p2.size()
                                ? concat_impl(
                                        write_back( p1, p2[Index2] ),
                                        p2, ytl::cu::make_index_tuple<std::size_t, Index2 + 1, MaxSize2>()
                                        )
                                : p1
                                ;
                }


                template<
                    typename SizeType1, SizeType1 MaxSize1, std::size_t... Indexes1,
                    typename SizeType2, SizeType2 MaxSize2, std::size_t... Indexes2
                >
                inline YTL_CONSTEXPR auto concat(
                    short_buffer<SizeType1, MaxSize1> const& p1, ytl::cu::index_tuple<std::size_t, Indexes1...> const&,
                    short_buffer<SizeType2, MaxSize2> const& p2, ytl::cu::index_tuple<std::size_t, Indexes2...> const&
                    ) -> short_buffer<
                                typename std::conditional<
                                    ( std::numeric_limits<SizeType1>::max() > std::numeric_limits<SizeType2>::max() ),
                                    SizeType1,
                                    SizeType2
                                >::type,
                                ( MaxSize1 + MaxSize2 )
                         >
                {
                    return concat_impl(
                                assembler::x86::detail::short_buffer<
                                    typename std::conditional<
                                        ( std::numeric_limits<SizeType1>::max() > std::numeric_limits<SizeType2>::max() ),
                                        SizeType1, SizeType2
                                    >::type,
                                    ( MaxSize1 + MaxSize2 )
                                >( buffer_construct_n, p1.size(), p1[Indexes1]... ),
                                p2, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize2>()
                                );
                }

                template<typename SizeType1, SizeType1 MaxSize1, typename SizeType2, SizeType2 MaxSize2>
                inline YTL_CONSTEXPR auto concat(
                    short_buffer<SizeType1, MaxSize1> const& p1,
                    short_buffer<SizeType2, MaxSize2> const& p2
                    ) -> decltype(
                            concat(
                                p1, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize1>(),
                                p2, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize2>()
                                )
                         )
                {
                    return concat(
                                p1, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize1>(),
                                p2, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize2>()
                                );
                }



                template<typename ToBuffer, typename SizeType, SizeType MaxSize, std::size_t... Indexes>
                inline YTL_CONSTEXPR auto fit_to(
                    short_buffer<SizeType, MaxSize> const& source,
                    ytl::cu::index_tuple<std::size_t, Indexes...> const&
                    ) -> ToBuffer
                {
                    return ToBuffer( buffer_construct_n, source.size(), source[Indexes]... );
                }

                template<typename ToBuffer, typename SizeType, SizeType MaxSize>
                inline YTL_CONSTEXPR auto fit_to(
                    short_buffer<SizeType, MaxSize> const& source
                    ) -> typename std::enable_if<
                                ( ToBuffer::max_size >= MaxSize ),
                                ToBuffer
                         >::type
                {
                    return fit_to<ToBuffer>( source, ytl::cu::make_index_tuple<std::size_t, 0, MaxSize>() );
                }

#else

                // Implementation for VC++!! Yay!
                template<typename SizeType, SizeType MaxSize>
                class short_buffer
                {
                public:
                    typedef short_buffer                                    self_type;
                    typedef SizeType                                        size_type;

                    static size_type const                                  max_size = MaxSize;

                private:
                    template<typename OtherSizeType, OtherSizeType OtherMaxSize>
                    friend class short_buffer;

                private:
                    typedef assembler::detail::raw_little_endian_method     write_method_type;
                    typedef std::array<byte_t, max_size>                    buffer_type;
                    typedef typename buffer_type::value_type                value_type;

                public:
                    short_buffer()
                        : size_( 0 )
                    {}

                    short_buffer( buffer_construct_tag )
                    {
                        buffer_type arr = {};

                        bin_ = std::move( arr );
                        size_ = 0;
                    }

# include <boost/preprocessor/repetition/enum.hpp>
# include <boost/preprocessor/repetition/enum_params.hpp>
# include <boost/preprocessor/repetition/enum_binary_params.hpp>
# include <boost/preprocessor/repetition/repeat_from_to.hpp>
# define ARR_P(z, n, unused) static_cast<value_type const>( p##n )
# define DEF_CTOR(z, n, unused) \
                    template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
                    short_buffer( buffer_construct_tag, BOOST_PP_ENUM_BINARY_PARAMS(n, T, const p) ) \
                    { \
                        buffer_type arr = { BOOST_PP_ENUM(n, ARR_P, ) }; \
                         \
                        bin_ = arr; \
                        size_ = n; \
                    }

                    BOOST_PP_REPEAT_FROM_TO(1, 64/*temp*/, DEF_CTOR, )
# undef ARR_P
# undef DEF_CTOR

                    template<typename SizeTypeS, SizeTypeS MaxSizeS>
                    short_buffer(
                        buffer_construct_n_tag,
                        short_buffer<SizeTypeS, MaxSizeS> const& s,
                        typename std::enable_if<( MaxSizeS <= MaxSize )>::type* = 0
                        )
                    {
                        std::memcpy( bin_.data(), s.bin_.data(), s.size() );
                        size_ = s.size();
                    }

                public:
                    template<typename SizeTypeR, SizeTypeR MaxSizeR>
                    bool operator==( short_buffer<SizeTypeR, MaxSizeR> const& rhs ) const
                    {
                        return ( size_ == rhs.size_ )
                            && ( std::equal( bin_.cbegin(), bin_.cbegin() + size_, rhs.bin_.cbegin() ) );
                    }

                    template<typename SizeTypeR, SizeTypeR MaxSizeR>
                    bool operator!=( short_buffer<SizeTypeR, MaxSizeR> const& rhs ) const
                    {
                        return !( (*this) == rhs );
                    }

                public:
                    inline value_type operator[]( std::size_t const& index ) const
                    {
                        return bin_[index];
                    }

                    inline size_type size() const
                    {
                        return size_;
                    }

                public:
                    template<typename SizeTypeRhs, SizeTypeRhs MaxSizeRhs>
                    friend auto operator/( self_type const& lhs, short_buffer<SizeTypeRhs, MaxSizeRhs> const& rhs )
                        -> short_buffer<
                                typename std::conditional<
                                    ( boost::integer_traits<SizeType>::const_max > boost::integer_traits <SizeTypeRhs>::const_max ),
                                    SizeType,
                                    SizeTypeRhs
                                >::type,
                                ( MaxSize + MaxSizeRhs )
                           >
                    {
                        short_buffer<
                            typename std::conditional<
                                ( boost::integer_traits<SizeType>::const_max > boost::integer_traits <SizeTypeRhs>::const_max ),
                                SizeType,
                                SizeTypeRhs
                            >::type,
                            ( MaxSize + MaxSizeRhs )
                        > target( buffer_construct_n, lhs );

                        write_method_type::write( target.bin_.data() + target.size_, rhs.bin_.data(), rhs.size_ );
                        target.size_ += rhs.size_;

                        return target;
                    }

                    template<typename T>
                    friend auto operator/( self_type const& lhs, T const& b )
                        -> typename std::enable_if<
                                    std::is_arithmetic<T>::value,
                                    short_buffer<
                                        SizeType,
                                        ( MaxSize + sizeof(T) )
                                    >
                           >::type
                    {
                        short_buffer<
                            SizeType,
                            ( MaxSize + sizeof(T) )
                        > target( buffer_construct_n, lhs );

                        write_method_type::write( target.bin_.data() + target.size_, std::addressof( b ), sizeof(T) );
                        target.size_ += sizeof(T);

                        return target;
                    }

                public:
                    buffer_type bin_;
                    size_type size_;
                };

                template<typename ToBuffer, typename SizeType, SizeType MaxSize>
                inline YTL_CONSTEXPR auto fit_to(
                    short_buffer<SizeType, MaxSize> const& source
                    ) -> ToBuffer
                {
                    return ToBuffer( buffer_construct_n, source );
                }

#endif /*!defined(YTL_OLD_IMPL)*/


                typedef short_buffer<uint8_t, 0> empty_buffer;

                // for modr/m and sib and displacement(max 6bytes)
                typedef short_buffer<uint8_t, 6> mod_rm_sib_disp_buffer;

                // for instruction(max 16bytes[15?])
                typedef short_buffer<uint8_t, 16> instruction_buffer;

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
                        return detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ )
                                    );
                    }

                    template<unsigned int N>
                    inline YTL_CONSTEXPR result_type operator()( types::memory_access<N> const& v ) const
                    {
                        return YTL_ASM_VARIANT_NAMESPACE::apply_visitor( mod_rm_sib_disp_visitor( reg_op_ ), v.var );
                    }

                public:
                    template<unsigned int N>
                    inline YTL_CONSTEXPR result_type operator()( detail::only_mod_rm<N> const& v ) const
                    {
                        return detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v, reg_op_ )
                                    );
                    }

                    template<unsigned int N, typename ValueType>
                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm_disp<N, ValueType> const& v ) const
                    {
                        return detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ ) / v.disp.value
                                    );
                    }

                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm32_sib const& v ) const
                    {
                        return detail::fit_to<result_type>(
                                    empty_buffer() / make_mod_rm( v.mod_rm, reg_op_ ) / make_sib( v.sib )
                                    );
                    }

                    template<typename ValueType>
                    inline YTL_CONSTEXPR result_type operator()( detail::mod_rm32_sib_disp<ValueType> const& v ) const
                    {
                        return detail::fit_to<result_type>(
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
                return YTL_ASM_VARIANT_NAMESPACE::apply_visitor( detail::mod_rm_sib_disp_visitor( Code ), v.var ); \
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
                            ? YTL_ASM_VARIANT_NAMESPACE::apply_visitor( detail::mod_rm_sib_disp_visitor( code ), v.var ) \
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