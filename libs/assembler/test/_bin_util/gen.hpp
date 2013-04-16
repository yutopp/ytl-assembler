#ifndef YTL_TEST_ASM_X86__BIN_UTIL_HPP
#define YTL_TEST_ASM_X86__BIN_UTIL_HPP

#include <ytl/assembler/arch/x86/value.hpp>

#if !defined(YTL_OLD_IMPL)

template<typename ...T>
YTL_CONSTEXPR auto _b( T&&... xs ) -> ytl::assembler::x86::detail::short_buffer<ytl::uint8_t, sizeof...(T)>
{
    using namespace ytl::assembler::x86::detail;

    return ytl::assembler::x86::detail::short_buffer<ytl::uint8_t, sizeof...(T)>( buffer_construct, std::forward<T>( xs )... );
}

#else

# include <boost/preprocessor/repetition/enum_params.hpp>
# include <boost/preprocessor/repetition/enum_binary_params.hpp>
# include <boost/preprocessor/repetition/repeat_from_to.hpp>

# define FORWARD_P(z, n, unused) std::forward<T##n>( p##n )
# define GEN_B(z, n, unused) \
    template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
    auto _b( BOOST_PP_ENUM_BINARY_PARAMS(n, T, && p) ) -> ytl::assembler::x86::detail::short_buffer<ytl::uint8_t, 64/*temporary*/> \
    { \
        using namespace ytl::assembler::x86::detail; \
        \
        return short_buffer<ytl::uint8_t, 64/*temporary*/>( buffer_construct, BOOST_PP_ENUM(n, FORWARD_P, ) ); \
    }

BOOST_PP_REPEAT_FROM_TO(1, 64/*temporary*/, GEN_B, )

# undef FORWARD_P
# undef GEN_B

#endif /*!defined(YTL_OLD_IMPL)*/

#endif /*YTL_TEST_ASM_X86__BIN_UTIL_HPP*/