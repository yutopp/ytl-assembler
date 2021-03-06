#ifndef YTL_ASSEMBLER_GENERATOR_HPP
#define YTL_ASSEMBLER_GENERATOR_HPP

#include <memory>
#include <functional>
#include <type_traits>

#include <boost/proto/proto.hpp>
#include <boost/noncopyable.hpp>

#include <ytl/utility.hpp>

#include "config.hpp"
#include "writer.hpp"
#include "endian_writer.hpp"
#include "binary_code.hpp"

namespace ytl
{
	namespace assembler
	{
		template<
			template<typename> class Engine,
			typename EndianWritter = assembler::endian::little
		>
		class generator
			: private boost::noncopyable
		{
			typedef std::size_t											index_type;
			typedef std::shared_ptr<index_type const>					index_const_ptr_type;
			typedef EndianWritter										endian_writer_type;
			
			class index_wrapper;
			typedef index_wrapper&										index_wrapper_ref_type;
			typedef typename
				boost::proto::terminal<index_wrapper_ref_type>::type	lazy_index_type;

		public:
			generator()
				: index_wrapper_()
				, $( lazy_index_type::make( std::ref( index_wrapper_ ) ) )
			{}

			template<typename Buffer>
			Engine<typename suitable_writer<Buffer, endian_writer_type>::type> // engine_type
			operator()( Buffer& buffer,
						index_type const index = 0u
						)
			{
				typedef typename suitable_writer<Buffer, endian_writer_type>::type		writer_type;
				typedef Engine<writer_type>												engine_type;
				typedef detail::get_asm_status_traits<Buffer>							buffer_asm_status_traits;

				auto const writer( std::make_shared<writer_type>( buffer, index ) );
				index_wrapper_ = index_wrapper( writer->get_index_ptr() );
				
				return engine_type( writer, buffer_asm_status_traits::get( buffer ) );
			}
		};

	} // namespace assembler
} // namespace ytl

#endif /*YTL_ASSEMBLER_GENERATOR_HPP*/
