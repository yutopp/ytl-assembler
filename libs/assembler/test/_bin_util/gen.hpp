#ifndef YTL_TEST_ASM_X86__BIN_UTIL_HPP
#define YTL_TEST_ASM_X86__BIN_UTIL_HPP

#include <ytl/assembler/arch/x86/value.hpp>

# include <boost/preprocessor/repetition/enum_params.hpp>
# include <boost/preprocessor/repetition/enum_binary_params.hpp>
# include <boost/preprocessor/repetition/repeat_from_to.hpp>

#define FORWARD_P(z, n, unused) std::forward<T##n>( p##n )

#define GEN_B(z, n, unused) \
    template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
    auto _b( BOOST_PP_ENUM_BINARY_PARAMS(n, T, && p) ) -> ytl::assembler::x86::detail::short_buffer<128/*temporary*/> \
    { \
        using namespace ytl::assembler::x86::detail; \
        \
        return ytl::assembler::x86::detail::short_buffer<128>( \
            buffer_construct, \
            BOOST_PP_ENUM(n, FORWARD_P, ) \
            ); \
    }
BOOST_PP_REPEAT_FROM_TO(1, 128/*temp*/, GEN_B, )



#endif /*YTL_TEST_ASM_X86__BIN_UTIL_HPP*/