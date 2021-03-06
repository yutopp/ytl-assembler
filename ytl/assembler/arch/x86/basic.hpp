#ifndef YTL_ASSEMBLER_X86_BASIC_HPP
#define YTL_ASSEMBLER_X86_BASIC_HPP

#include <stdexcept>

#include "../../config.hpp"

#include "registers.hpp"

namespace ytl
{
    namespace assembler
    {
        namespace x86
        {
            //
            // value types
            //
            typedef int8_t      byte_value_t;
            typedef int16_t     word_value_t;
            typedef int32_t     dword_value_t;


            // kit types
            namespace detail
            {
                struct register_none_t {}; // for Base:none


                /* ----------------
                --  16bit
                -----------------*/
                template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                struct register_pair16
                {};

                template<typename RegisterCode0, typename RegisterCode1, byte_t Code, typename DataType>
                struct register_pair16_disp_value
                {
                    typedef DataType    data_type;

                    YTL_CONSTEXPR register_pair16_disp_value( data_type const d )
                        : value( d )
                    {}

                    data_type value;
                };


                /* ----------------
                --  32bit
                -----------------*/
                template<typename BaseRegisterCode, typename ScaleRegisterCode>
                struct sib_value
                {
                    YTL_CONSTEXPR sib_value( byte_t const scale )
                        : scaling_factor( scale==1 ? 0 : scale==2 ? 1 : scale==4 ? 2 : scale==8 ? 3 : throw/**/ )
                    {}

                    template<typename OtherBaseRegisterCode>
                    YTL_CONSTEXPR sib_value( sib_value<OtherBaseRegisterCode, ScaleRegisterCode> const& rhs )
                        : scaling_factor( rhs.scaling_factor )
                    {}

                    byte_t scaling_factor;
                };

                /*
                    register + displacement value
                */
                template<typename RegisterCode, typename DataType>
                struct reg_disp_value
                {
                    typedef RegisterCode    register_code_type;
                    typedef DataType        data_type;

                    YTL_CONSTEXPR reg_disp_value( data_type const disp )
                        : value( disp )
                    {}

                    data_type value;
                };

                /*
                    sib + displacement value
                */
                template<typename BaseRegisterCode, typename ScaleRegisterCode, typename DataType>
                struct sib_disp_value
                {
                    typedef DataType    data_type;

                    YTL_CONSTEXPR sib_disp_value( sib_value<BaseRegisterCode, ScaleRegisterCode> const& s, data_type const disp )
                        : sib( s )
                        , value( disp ) {}

                    sib_value<BaseRegisterCode, ScaleRegisterCode> sib;
                    data_type value;
                };

            } // namespace detail


            // operator supports
            namespace reg
            {
                namespace types
                {
                    // support [base + reg * scale + disp]

                    /* ----------------
                    --  16bit ModR/M
                    --  Table Spec 2-1
                    -----------------*/
                    /*
                        [BX+SI], [BX+DI], [BP+SI], [BP+DI]
                    */
                    inline YTL_CONSTEXPR auto operator+( reg::types::bx_t const&, reg::types::si_t const& )
                        -> detail::register_pair16<reg::types::bx_t, reg::types::si_t, 0>
                    {
                        return detail::register_pair16<reg::types::bx_t, reg::types::si_t, 0>();
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bx_t const&, reg::types::di_t const& )
                        -> detail::register_pair16<reg::types::bx_t, reg::types::di_t, 1>
                    {
                        return detail::register_pair16<reg::types::bx_t, reg::types::di_t, 1>();
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bp_t const&, reg::types::si_t const& )
                        -> detail::register_pair16<reg::types::bp_t, reg::types::si_t, 2>
                    {
                        return detail::register_pair16<reg::types::bp_t, reg::types::si_t, 2>();
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bp_t const&, reg::types::di_t const& )
                        -> detail::register_pair16<reg::types::bp_t, reg::types::di_t, 3>
                    {
                        return detail::register_pair16<reg::types::bp_t, reg::types::di_t, 3>();
                    }

                    /*
                        REG_PAIR16 +- disp8
                    */
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, byte_value_t>
                    {
                        return disp;
                    }
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator-( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, byte_value_t>
                    {
                        return -disp;
                    }
                    /*
                        ( [SI], [DI], [BP], [BX] ) +- disp8
                    */
                    inline YTL_CONSTEXPR auto operator+( reg::types::si_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::si_t, 4, byte_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::si_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::si_t, 4, byte_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::di_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::di_t, 5, byte_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::di_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::di_t, 5, byte_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bp_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bp_t, 6, byte_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::bp_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bp_t, 6, byte_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bx_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bx_t, 7, byte_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::bx_t const&, byte_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bx_t, 7, byte_value_t>
                    {
                        return -disp;
                    }

                    /*
                        REG_PAIR16 +- disp16
                    */
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, word_value_t>
                    {
                        return disp;
                    }
                    template<typename RegisterCode0, typename RegisterCode1, byte_t Code>
                    inline YTL_CONSTEXPR auto operator-( detail::register_pair16<RegisterCode0, RegisterCode1, Code> const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<RegisterCode0, RegisterCode1, Code, word_value_t>
                    {
                        return -disp;
                    }
                    /*
                        ( [SI], [DI], [BP], [BX] ) +- disp16
                    */
                    inline YTL_CONSTEXPR auto operator+( reg::types::si_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::si_t, 4, word_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::si_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::si_t, 4, word_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::di_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::di_t, 5, word_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::di_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::di_t, 5, word_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bp_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bp_t, 6, word_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::bp_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bp_t, 6, word_value_t>
                    {
                        return -disp;
                    }
                    inline YTL_CONSTEXPR auto operator+( reg::types::bx_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bx_t, 7, word_value_t>
                    {
                        return disp;
                    }
                    inline YTL_CONSTEXPR auto operator-( reg::types::bx_t const&, word_value_t const disp )
                        -> detail::register_pair16_disp_value<detail::register_none_t, reg::types::bx_t, 7, word_value_t>
                    {
                        return -disp;
                    }



                    /* ----------------
                    --  32bit ModR/M
                    --  Table Spec 2-2
                    -----------------*/
                    /*
                        disp scales : REG32 +- disp8 (OR disp8 + REG32)
                    */
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/, byte_value_t const disp )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, byte_value_t>
                    {
                        return disp;
                    }
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( byte_value_t const disp, assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/ )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, byte_value_t>
                    {
                        return disp;
                    }
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator-( assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/, byte_value_t const disp )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, byte_value_t>
                    {
                        return -disp;
                    }
                    /*
                        disp scales : REG32 +- disp32 (OR disp32 + REG32)
                    */
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/, dword_value_t const disp )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, dword_value_t>
                    {
                        return disp;
                    }
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator+( dword_value_t const disp, assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/ )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, dword_value_t>
                    {
                        return disp;
                    }
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator-( assembler::detail::register_code<Code, types::reg_32bit_tag> const& /*reg*/, dword_value_t const disp )
                        -> detail::reg_disp_value<assembler::detail::register_code<Code, types::reg_32bit_tag>, dword_value_t>
                    {
                        return -disp;
                    }


                    /*
                        sib & disp scales(Mod R/M[R/M 0b100]) : [--][--] +- disp8 (OR disp8 + [--][--] )
                    */
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator+( detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib, byte_value_t const disp )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>( sib, disp );
                    }
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator+( byte_value_t const disp, detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>( sib, disp );
                    }
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator-( detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib, byte_value_t const disp )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, byte_value_t>( sib, -disp );
                    }

                    /*
                        sib & disp scales(Mod R/M[R/M 0b100]) : [--][--] +- disp32 (OR disp32 + [--][--] )
                    */
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator+( detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib, dword_value_t const disp )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>( sib, disp );
                    }
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator+( dword_value_t const disp, detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>( sib, disp );
                    }
                    template<typename BaseRegisterCode, typename ScaleRegisterCode>
                    inline YTL_CONSTEXPR auto operator-( detail::sib_value<BaseRegisterCode, ScaleRegisterCode> const& sib, dword_value_t const disp )
                        -> detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>
                    {
                        return detail::sib_disp_value<BaseRegisterCode, ScaleRegisterCode, dword_value_t>( sib, -disp );
                    }


                    /* ----------------
                    --  sib bites appear only in 32bit addresing format.
                    --  Table Spec 2-3
                    -----------------*/
                    /*
                        SIB scales(No Base) : [REG32 * n] (OR n * REG32)
                        Base[*]
                    */
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator*( assembler::detail::register_code<Code, types::reg_32bit_tag> const&, byte_t const scale )
                        -> detail::sib_value<detail::register_none_t, assembler::detail::register_code<Code, types::reg_32bit_tag>>
                    {
                        static_assert( !std::is_same<assembler::detail::register_code<Code, types::reg_32bit_tag>, reg::types::esp_t>::value, "register ESP can't be multiplied." );

                        return ( scale == 1 || scale == 2 || scale == 4 || scale == 8 )
                             ? scale
                             : throw std::invalid_argument( "scale must be 1, 2, 4 or 8." );
                    }
                    template<byte_t Code>
                    inline YTL_CONSTEXPR auto operator*( byte_t const scale, assembler::detail::register_code<Code, types::reg_32bit_tag> const& reg )
                        -> detail::sib_value<detail::register_none_t, assembler::detail::register_code<Code, types::reg_32bit_tag>>
                    {
                        return reg * scale;
                    }

                    /*
                        SIB scales : BASE_REG32 + SIB scales(No Base) (OR SIB scales(No Base) + BASE_REG32)
                    */
                    // BASE_REG32 + SIB scales(No Base)
                    template<byte_t Code, byte_t SibRegisterCode>
                    inline YTL_CONSTEXPR auto operator+(
                        assembler::detail::register_code<Code, types::reg_32bit_tag> const&,
                        detail::sib_value<detail::register_none_t, assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag>> const& sib
                        ) -> detail::sib_value<
                                assembler::detail::register_code<Code, types::reg_32bit_tag>,
                                assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag>
                                >
                    {
                        return sib;
                    }
                    template<byte_t Code, byte_t SibRegisterCode>
                    inline YTL_CONSTEXPR auto operator+(
                        detail::sib_value<detail::register_none_t, assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag>> const& sib,
                        assembler::detail::register_code<Code, types::reg_32bit_tag> const&
                        ) -> detail::sib_value<
                                assembler::detail::register_code<Code, types::reg_32bit_tag>,
                                assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag>
                                >
                    {
                        return sib;
                    }
                    // BASE_REG32 + BASE_REG32(SIB scale * 1)
                    template<byte_t Code, byte_t SibRegisterCode>
                    inline YTL_CONSTEXPR auto operator+(
                        assembler::detail::register_code<Code, types::reg_32bit_tag> const&,
                        assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag> const&
                        ) -> detail::sib_value<
                                assembler::detail::register_code<Code, types::reg_32bit_tag>,
                                assembler::detail::register_code<SibRegisterCode, types::reg_32bit_tag>
                                >
                    {
                        return 1;
                    }

                } // namespace types
            } // namespace reg
        } // namespace x86
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_X86_BASIC_HPP*/