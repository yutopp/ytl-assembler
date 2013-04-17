#ifndef YTL_ASSEMBLER_DETAIL_SHORT_BUFFER_HPP
#define YTL_ASSEMBLER_DETAIL_SHORT_BUFFER_HPP

#if defined(YTL_OLD_IMPL)
#include <array>
#endif /*defined(YTL_OLD_IMPL)*/

#include <boost/config.hpp>
#include <boost/integer.hpp>

#include <ytl/base/constexpr_utility.hpp>

#include "../config.hpp"
#include "raw_little_endian_method.hpp"


namespace ytl
{
    namespace assembler
    {
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
                -> short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>;

            template<typename T>
            inline YTL_CONSTEXPR auto to_binary_little( T const& val )
                -> short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>;


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
                    ) -> short_buffer<
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
                                short_buffer<
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
//                    assert( size + sizeof(T) <= MaxSize );
//
//                    write_method_type::write( bin.data() + size, std::addressof( b ), sizeof(T) );
//                    size += sizeof(T);

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
                -> short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>
            {
                return short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>(
                            buffer_construct, 
                            little_endian_byte_access( val, Indexes )...
                            );
            }

            template<typename T>
            inline YTL_CONSTEXPR auto to_binary_little( T const& val )
                -> short_buffer<typename boost::int_max_value_t<sizeof(T)>::least, sizeof(T)>
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
                            short_buffer<
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

        } // namespace detail
    } // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_DETAIL_SHORT_BUFFER_HPP*/