# -*- coding: utf8 -*-
import os
import urllib.request
from bs4 import BeautifulSoup


def read_x86_table( domain = "http://ref.x86asm.net/", filename = "coder32.html" ):
    local_filename = "_temp/" + filename
    if not os.path.exists( local_filename ):
        urllib.request.urlretrieve( domain + filename, local_filename )
    html = BeautifulSoup( open( local_filename ) )
    
#    for table in [ x for x in html.find_all("table") if x.get("class") == ["ref_table", "notpublic"] ]:
    for table in [ x for x in html.find_all("table") if x.get("class") == ["ref_table", "notpublic"] ][0:1]:
#        for tbody in [ x for x in table.find_all("tbody") ]:
        for tbody in [ x for x in table.find_all("tbody") ][8:14]:#[48:70]:
            tds = tbody.find_all("td")
            access_index = 0
            
            # pf
            prefix = None
            prefix_str = tds[access_index].string 
            if prefix_str is not None:
                # add prefix byte
                prefix = int( prefix_str, 16 )
            else:
                pass
            access_index += 1  ###
            
            
            # 0f
            prefix_0f = None
            byte_0f_str = tds[access_index].string
            if byte_0f_str is not None:
                # add prefix primary opcode byte
                prefix_0f = int( byte_0f_str, 16 )
            else:
                pass
            access_index += 1  ###
            
            
            # primary opcode
            opcode = []
            with_register = False
            byte_po_str = tds[access_index].string
            if byte_po_str is not None:
                # 
                col = tds[access_index].get("colspan")
                if col is None:
                    opcode.append( int( byte_po_str, 16 ) ) # add opcode byte
                    access_index += 1  ###
                    
                    ## second opcode
                    byte_so_str = tds[access_index].string
                    if byte_so_str is not None:
                        opcode.append( int( byte_so_str, 16 ) ) # add opcode byte
                    
                else:
                    n = int( col )
                    if n == 3:
                        # maybe 0F prefix
                        continue
                        ## ^
                        
                    elif n == 2:
                        # with register type
                        with_register = True
                        opcode.append( int( byte_po_str[0:2], 16 ) ) # add opcode byte
                        
                    else:
                        raise( "unknown col" )
            else:
                if prefix is not None:
                    ## only prefix
                    continue
                    ##
                    
                else:
                    raise( "no operator" )
            access_index += 1  ###
            
            
            # ModR/M o
            byte_o_str = tds[access_index].string
            access_index += 1  ###
            
            
            # proc
            access_index += 1  ###
            
            
            # st
            access_index += 1  ###
            
            
            # m
            access_index += 1  ###
            
            
            # rl
            access_index += 1  ###
            
            
            # x
            access_index += 1  ###
            
            
            # mnemonic
            mnemonic = tds[access_index].string
            access_index += 1  ###
            
            
            # operand1~4
            operand = []
            for i in range(4):
                s = tds[access_index].string
                if s is not None:
                    operand.append( s.lower() ) # TO LOWER
                access_index += 1  ###
            
            # print
            generate_x86_ytl_code( prefix, prefix_0f, opcode, with_register, byte_o_str, mnemonic, operand )
            #
    ###
    print( "finished!" )

generated_mnemonic = {}

special_inst = {
    "PUSHA": [],
    "PUSHAD": [],
    "POPA": [],
    "POPAD": [],
    "BOUND": [0, 1]
}

pattern = {
    ('r/m8','r8'): [ ( [ "byte_t Code" ],
                       [ "types::r_m8", "assembler::detail::register_code<Code, reg::types::reg_8bit_tag>" ],
                       [ False, False ],
                       None
                     ),
                   ],
    ('r/m16/32', 'r16/32' ): [ ( [ "byte_t Code" ],
                                 [ "types::r_m16", "assembler::detail::register_code<Code, reg::types::reg_16bit_tag>" ],
                                 [ False, False ],
                                 None
                               ),
                               ( [ "byte_t Code" ],
                                 [ "types::r_m16", "assembler::detail::register_code<Code, reg::types::reg_32bit_tag>" ],
                                 [ False, False ],
                                 None
                               ),
                               ( [ "byte_t Code" ],
                                 [ "types::r_m32", "assembler::detail::register_code<Code, reg::types::reg_16bit_tag>" ],
                                 [ False, False ],
                                 None
                               ),
                               ( [ "byte_t Code" ],
                                 [ "types::r_m32", "assembler::detail::register_code<Code, reg::types::reg_32bit_tag>" ],
                                 [ False, False ],
                                 None
                               ),
                             ],
    ('r8', 'r/m8'): [ ( [ "byte_t Code" ],
                        [ "assembler::detail::register_code<Code, reg::types::reg_8bit_tag>", "types::r_m8" ],
                        [ False, False ],
                        None
                     ),
                   ],
    ('r16/32', 'r/m16/32'): [ ( [ "byte_t Code" ],
                                [ "assembler::detail::register_code<Code, reg::types::reg_16bit_tag>", "types::r_m16" ],
                                [ False, False ],
                                None
                              ),
                              ( [ "byte_t Code" ],
                                [ "assembler::detail::register_code<Code, reg::types::reg_16bit_tag>", "types::r_m32" ],
                                [ False, False ],
                                None
                              ),
                              ( [ "byte_t Code" ],
                                [ "assembler::detail::register_code<Code, reg::types::reg_32bit_tag>", "types::r_m16" ],
                                [ False, False ],
                                None
                              ),
                              ( [ "byte_t Code" ],
                                [ "assembler::detail::register_code<Code, reg::types::reg_32bit_tag>", "types::r_m32" ],
                                [ False, False ],
                                None
                              ),
                            ],
    ('al', 'imm8'): [ ( None,
                        [ "reg::types::al_t", "byte_value_t" ],
                        [ True, False ],
                        [ "p1" ]
                      ),
                    ],
    ('eax', 'imm16/32'): [ ( None,
                             [ "reg::types::ax_t", "word_value_t" ],
                             [ True, False ],
                             [ "p1" ]
                           ),
                           ( None,
                             [ "reg::types::ax_t", "dword_value_t" ],
                             [ True, False ],
                             [ "p1" ]
                           ),
                           ( None,
                             [ "reg::types::eax_t", "word_value_t" ],
                             [ True, False ],
                             [ "p1" ]
                           ),
                           ( None,
                             [ "reg::types::eax_t", "dword_value_t" ],
                             [ True, False ],
                             [ "p1" ]
                           ),
                         ],
}

def generate_x86_ytl_code_entry( mnemonic ):
    entry_code = "template<typename BitsTag>\n" \
               + "YTL_CONSTEXPR auto {0}()\n".format( mnemonic ) \
               + "    -> decltype( generic_instruction_call<detail::{0}_impl, BitsTag>() )\n".format( mnemonic ) \
               + "{\n" \
               + "    return generic_instruction_call<detail::{0}_impl, BitsTag>();\n".format( mnemonic ) \
               + "}\n"
    for i in range( 1, 5 ):
        entry_code += "template<typename BitsTag, {0}>\n".format( ", ".join( [ "typename P{0}".format( i ) for i in range( 0, i )] ) ) \
                   +  "YTL_CONSTEXPR auto {0}( {1} )\n".format( mnemonic, ", ".join( [ "P{0}&& p{0}".format( i ) for i in range( 0, i )] ) ) \
                   + "    -> decltype( generic_instruction_call<detail::{0}_impl, BitsTag>( {1} ) )\n".format( mnemonic, ", ".join( [ "std::forward<P{0}>( p{0} )".format( i ) for i in range( 0, i )] ) ) \
                   +  "{\n" \
                   +  "    return generic_instruction_call<detail::{0}_impl, BitsTag>( {1} );\n".format( mnemonic, ", ".join( [ "std::forward<P{0}>( p{0} )".format( i ) for i in range( 0, i )] ) ) \
                   +  "}\n"
    
    return entry_code



def generate_x86_ytl_code( prefix, prefix_0f, opcode, with_register, byte_o_str, mnemonic, operands ):
    # adjust operands
    if mnemonic in special_inst:
        operands = [ operands[i] for i in special_inst[mnemonic] ]
    operands = tuple( operands )
    
    
    # create commmon interface funciton
    if mnemonic not in generated_mnemonic:
        generated_mnemonic[mnemonic] = {
            "entry_code": generate_x86_ytl_code_entry( mnemonic ),
            "detail_code_list": []
        }
    detail_code_list = generated_mnemonic[mnemonic]["detail_code_list"]
    
    
    #
    decl_list = pattern.get( operands, None )
    if decl_list is None:
        print( operands, "is not supported." )
        return
    
    
    #
    modrm_sib_disp = None
    if byte_o_str is not None:
        if byte_o_str == 'r':
            modrm_sib_disp = "generate_mod_rm_sib_disp( p0, p1 )"
        else:
            modrm_sib_disp = "generate_mod_rm_sib_disp( {0}, p1 )".format( int( byte_o_str ) )
    
    
    #
    detail_code_list.append( "//\n" + "// ( " + ", ".join( operands ) + " )" )
    for decl in decl_list:
        print( prefix, prefix_0f, opcode, with_register, byte_o_str, mnemonic, operands, sep=', ' )
        
        c_template = decl[0]
        c_parameter_list = decl[1]
        c_is_masked_param = decl[2]
        c_appendix = decl[3]
        
        c_parameter_list = ", ".join( ["{0} const&{1}".format(p, "" if c_is_masked_param[index] else  " p{0}".format( index ) ) for index, p in enumerate(decl[1])] )
        
        #
        c_decl = ""
        if c_template is not None:
            c_decl += "template<" + ", ".join( c_template ) + ">\n"
        c_decl += "YTL_CONSTEXPR auto op( Buffer const& buffer{0}) const".format( " " if len( c_parameter_list ) == 0 else ", {0} ".format( c_parameter_list ) )
        #print( c_decl )
        
        
        #
        c_body_prefix = []
        if prefix is not None:
            c_body_prefix.append( prefix )
        if prefix_0f is not None:
            c_body_prefix.append( prefix_0f )
            
        c_body_prefix = "/ ".join( [ "static_cast<byte_t>( 0x{0:02x} )".format( p ) for p in c_body_prefix ] )
        if len( c_body_prefix ) == 0:
            c_body_prefix = None
        
        #print( c_body_prefix )
        
        
        #
        c_statement = "buffer"
        if c_body_prefix is not None:
            c_statement += " / " + c_body_prefix
        
        c_statement += " / " + "/ ".join( [ "static_cast<byte_t>( 0x{0:02x} )".format( p ) for p in opcode ] )
        
        if modrm_sib_disp is not None:
            c_statement += " / " + modrm_sib_disp
        if c_appendix is not None:
            for ap in c_appendix:
                 c_statement += " / " + ap
        print( c_statement )
        
        
        #
        c_definition  = c_decl + "\n" \
                      + "    -> decltype( " + c_statement + " )\n" \
                      + "{\n" \
                      + "    return " + c_statement + ";\n" \
                      + "}\n"
        
        
        #
        detail_code_list.append( c_definition )
        
        #print( modrm_sib_disp )
        
print( "\n" )



def is_only_x86_register( operand ):
    return True


if __name__ == "__main__":
    read_x86_table()
    
    output_body = ""
    for k, v in generated_mnemonic.items():
        output_body += "////////////////////////////////////////////////////\n" \
                    +  "// -- {0}\n".format( k ) \
                    +  "// implementation\n" \
                    +  "namespace detail\n" \
                    +  "{\n" \
                    +  "    template<typename Buffer>\n" \
                    +  "    struct {0}_impl\n".format( k ) \
                    +  "    {\n"
        for code in v["detail_code_list"]:
            output_body += "".join( [ "        {0}\n".format( l ) for l in ( code + "\n" ).splitlines()] )
        output_body += "    };\n" \
                    +  "} // namespace detail\n" \
                    +  "\n" \
                    +  "// interface\n" \
                    +  v["entry_code"] \
                    +  "////////////////////////////////////////////////////\n" \
                    +  "\n"
    
    output   = ""
    
    output  +="#ifndef YTL_ASSEMBLER_X86_INSTRUCTION_HPP\n" \
            + "#define YTL_ASSEMBLER_X86_INSTRUCTION_HPP\n" \
            + "\n" \
            + "#include \"value.hpp\"\n" \
            + "#include \"call.hpp\"\n" \
            + "\n" \
            + "namespace ytl\n" \
            + "{\n" \
            + "    namespace assembler\n" \
            + "    {\n" \
            + "        namespace x86\n" \
            + "        {\n" \
            + "            namespace instruction\n" \
            + "            {\n" \
            + "".join( [ "                {0}\n".format( l ) for l in output_body.splitlines()] ) \
            + "            } // namespace instruction\n" \
            + "        } // namespace x86\n" \
            + "    } // namespace assembler\n" \
            + "} // namespace ytl\n" \
            + "\n" \
            + "#endif /*YTL_ASSEMBLER_X86_INSTRUCTION_HPP*/\n"
    #print( output )
    

    with open( "instruction.hpp", "w" ) as f:
        f.write( output )

