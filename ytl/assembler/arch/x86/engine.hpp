#ifndef YTL_ASSEMBLER_X86_ENGINE_HPP
#define YTL_ASSEMBLER_X86_ENGINE_HPP

#include "../../config.hpp"
#include "../../exception.hpp"

#include "../detail/pseudo_operations.hpp"

#include "registers.hpp"
#include "data_types.hpp"
#include "alignment.hpp"
#include "address.hpp"


namespace ytl
{
	namespace assembler
	{
		namespace x86
		{
			template<typename Writer>
			class engine
				: public detail::pseudo_operations<engine<Writer>, Writer>
			{
				typedef engine											self_type;
				typedef detail::pseudo_operations<self_type, Writer>	base_type;

			public:
				typedef typename self_type::writer_type					writer_type;
				typedef typename self_type::writer_shared_pointer		writer_shared_pointer;

				typedef typename self_type::status_type					status_type;
				typedef typename self_type::status_shared_pointer		status_shared_pointer;

				typedef typename self_type::size_type					size_type;
				typedef typename self_type::return_type					return_type;

			public:
				engine( writer_shared_pointer w, status_shared_pointer s )
					: base_type( w, s )
				{}

				/* -----------------------------------------------------
					Assembler
				----------------------------------------------------- */

				// Mnemonic --------------------------------------------

				// -- A --

				// AAA
				YTL_ASM_OP_BEGIN( aaa() )
					this->db( 0x37 );
				YTL_ASM_OP_END


				// AAD
				YTL_ASM_OP_BEGIN( aad() )
					this->db( 0xd5 );
					this->db( 0x0a );
				YTL_ASM_OP_END
				/*// AAD ib
				YTL_ASM_OP_BEGIN aad( byte_value dst ) { db( 0xd5 ); db( dst ); }*/


				// AAM
				YTL_ASM_OP_BEGIN( aam() )
					this->db( 0xd4 );
					this->db( 0x0a );
				YTL_ASM_OP_END
				/*// AAM ib
				YTL_ASM_OP_BEGIN aam( byte_value dst ) { db( 0xD4 ); db( dst ); }*/


				// AAS
				YTL_ASM_OP_BEGIN( aas() )
					this->db( 0x3f );
				YTL_ASM_OP_END


				// ADC
//				YTL_ASM_TYPICAL_OPERATION( adc, 0x14, 0x15, 2, 0x80, 0x81, 0x83, 0x10, 0x11, 0x12, 0x13 )


				// ADD
				YTL_ASM_TYPICAL_OPERATION( add, 0x04, 0x05, 0, 0x80, 0x81, 0x83, 0x00, 0x01, 0x02, 0x03 )


				// ADDPD

				// ADDPS

				// ADDSD

				// ADDSS

				// ADDSUBPD

				// ADDSUBPS


				// AND
//				YTL_ASM_TYPICAL_OPERATION( and, 0x24, 0x25, 4, 0x80, 0x81, 0x83, 0x20, 0x21, 0x22, 0x23 )


				// ANDPD

				// ANDPS

				// ANDNPD

				// ANDNPS


				// ARPL
				YTL_ASM_OP_LAZY_2( arpl )

				YTL_ASM_OP_BEGIN( arpl( r_m16 dst, registers::r16_t src ) )
					this->db( 0x63 );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END

				// -- B --
				// -- C --
				// -- D --
				// -- E --
				// -- F --
				// -- G --
				// -- H --
				// -- I --

				// INT 3, INT, INTO
				YTL_ASM_OP_LAZY_1( int_ )

				YTL_ASM_OP_BEGIN( int_( int8_t src ) )
					if ( src == 3 ) {
						this->db( 0xcc );
					} else {
						this->db( 0xcd );
						this->db( src );
					}
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( into() )
					this->db( 0xce );
				YTL_ASM_OP_END


				// -- J --
				// -- K --
				// -- L --
				// -- M --

				// MOV
				YTL_ASM_OP_LAZY_2( mov )

				YTL_ASM_OP_BEGIN( mov( r_m8_implicit dst, registers::r8_t src ) )
					this->db( 0x88 );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END
					
				YTL_ASM_OP_BEGIN( mov( r_m16_implicit dst, registers::r16_t src ) )
					this->db( 0x88 );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( r_m32_implicit dst, registers::r32_t src ) )
					this->db( 0x89 );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( registers::r8_t dst, r_m8 src ) )
					this->db( 0x8a );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( registers::r16_t dst, r_m16 src ) )
					this->db( 0x8a );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( registers::r32_t dst, r_m32 src ) )
					this->db( 0x8b );
					dmod_rm_sib_disp( dst, src );
				YTL_ASM_OP_END

				/*
				*/


				YTL_ASM_OP_BEGIN( mov( registers::r8_t dst, int8_t src ) )
					this->db( 0xb0 + dst );
					this->db( src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( registers::r16_t dst, int16_t src ) )
					this->db( 0xb8 + dst );
					this->dw( src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( registers::r32_t dst, int32_t src ) )
					this->db( 0xb8 + dst );
					this->dd( src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( r_m8 dst, int8_t src ) )
					this->db( 0xc6 );
					dmod_rm_sib_disp( dst, 0 );
					this->db( src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( r_m16 dst, int16_t src ) )
					this->db( 0xc7 );
					dmod_rm_sib_disp( dst, 0 );
					this->dw( src );
				YTL_ASM_OP_END

				YTL_ASM_OP_BEGIN( mov( r_m32 dst, int32_t src ) )
					this->db( 0xc7 );
					dmod_rm_sib_disp( dst, 0 );
					this->dd( src );
				YTL_ASM_OP_END

				// -- N --
//				YTL_ASM_OP_BEGIN not( registers::detail::id<32> reg ) {
//				}

				// -- O --

				//
				// -- P --
				//

				// POP, POPA, POPAD, POPF, POPFD
				#include "operation/pop_family.hpp"

				// PUSH, PUSHA, PUSHAD, PUSHF, PUSHFD
				#include "operation/push_family.hpp"


				// -- Q --
				// -- R --
				YTL_ASM_OP_BEGIN( ret() )
					this->db( 0xc3 );
				YTL_ASM_OP_END

				// -- S --

				// SUB
				YTL_ASM_TYPICAL_OPERATION( sub, 0x2c, 0x2d, 5, 0x80, 0x81, 0x83, 0x28, 0x29, 0x2a, 0x2b )


				// -- T --
				// -- U --
				// -- V --
				// -- W --
				// -- X --
				// -- Y --
				// -- Z --



			private:
				template<typename EngineT>
				class mod_rm_sib_disp_visitor
					: public boost::static_visitor<void>
				{
					typedef EngineT engine_type;

				public:
					mod_rm_sib_disp_visitor( byte_t reg_op, engine_type* engine_ptr )
						: reg_op_( reg_op )
						, engine_ptr_( engine_ptr )
					{}

					template<unsigned int N>
					void operator()( mod_rm<N> const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
					}

					void operator()( mod_rm32_disp8 const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						engine_ptr_->db( v.disp_ );
					}

					void operator()( mod_rm32_disp32 const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						engine_ptr_->dd( v.disp_ );
					}

					/* --------
						Only 32bit addresing format.
					-------- */
					void operator()( mod_rm32_sib const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						engine_ptr_->db( make_sib( v.sib_.ss_, v.sib_.index_, v.sib_.base_ ) );
					}

					void operator()( mod_rm32_sib_disp8 const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						engine_ptr_->db( make_sib( v.sib_disp_.sib_.ss_, v.sib_disp_.sib_.index_, v.sib_disp_.sib_.base_ ) );
						engine_ptr_->db( v.sib_disp_.disp_ );
					}

					void operator()( mod_rm32_sib_disp32 const& v ) const
					{
						engine_ptr_->db( make_mod_rm( v.mod_, reg_op_, v.rm_ ) );
						engine_ptr_->db( make_sib( v.sib_disp_.sib_.ss_, v.sib_disp_.sib_.index_, v.sib_disp_.sib_.base_ ) );
						engine_ptr_->dd( v.sib_disp_.disp_ );
					}

				private:
					byte_t reg_op_;
					engine_type* engine_ptr_;
				};

				template<typename T, unsigned int N>
				void dmod_rm_sib_disp( T const& data, detail::register_id<N> reg )
				{
					boost::apply_visitor( mod_rm_sib_disp_visitor<self_type>( reg.get(), this ), data.value_ );
				}
				template<typename T, unsigned int N>
				void dmod_rm_sib_disp( detail::register_id<N> reg, T const& data )
				{
					dmod_rm_sib_disp( data, reg );
				}
				template<typename T>
				void dmod_rm_sib_disp( T const& data, byte_t op_code )
				{
					boost::apply_visitor( mod_rm_sib_disp_visitor<self_type>( op_code, this ), data.value_ );
				}
			};

		} // namespece x86
	} // namespace assembler
} // namespace ytl

#undef YTL_ASM_TYPICAL_OPERATION

#endif /*YTL_ASSEMBLER_X86_ENGINE_HPP*/
