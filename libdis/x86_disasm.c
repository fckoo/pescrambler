#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libdis.h"
#include "ia32_insn.h"
#include "ia32_invariant.h"
#include "x86_operand_list.h"


#ifdef _MSC_VER
        #define snprintf        _snprintf
        #define inline          __inline
#endif


int x86_disasm( unsigned char *buf, unsigned int buf_len,
                unsigned long buf_rva, unsigned int offset,
                x86_insn_t *insn ){
        int len, size;
	unsigned char *bytes;

        if ( ! buf || ! insn || ! buf_len ) {
                /* caller screwed up somehow */
                return(0);
        }

        /* clear insn struct */
        if ( offset >= buf_len ) {
                /* another caller screwup ;) */
                x86_report_error(report_disasm_bounds, (char*)buf_rva+offset);
                return(0);
        }

        len = buf_len - offset;
	bytes = &buf[offset];

        /* actually do the disassembly */
	/* TODO: allow switching when more disassemblers are added */
        size = ia32_disasm_addr( bytes, len, insn);

        /* check and see if we had an invalid instruction */
        if (! size ) {
                x86_report_error(report_invalid_insn, (char*)buf_rva+offset );
                return(0);
        }

        /* check if we overran the end of the buffer */
	/* FIXME: this may no longer have an effect : move to main
	 * disasm routine */
        if ( size > len ) {
                x86_report_error( report_insn_bounds, (char*)buf_rva + offset );
                return(0);
        }

        /* fill rva, offset, and bytes fields of insn */
        insn->addr = buf_rva + offset;
        insn->offset = offset;
        insn->size = size;
        memcpy( insn->bytes, bytes, size );

        return (size);
}

int x86_disasm_range( unsigned char *buf, unsigned long buf_rva,
                      unsigned int offset, unsigned int len,
                      DISASM_CALLBACK func, void *arg ) {
        x86_insn_t insn;
        int buf_len, size, count = 0, bytes = 0;

        /* buf_len is implied by the arguments */
        buf_len = len + offset;

        while ( bytes < len ) {
                size = x86_disasm( buf, buf_len, buf_rva, offset + bytes,
                                   &insn );
                if ( size ) {
                        /* invoke callback if it exists */
                        if ( func ) {
                                (*func)( &insn, arg );
                        }
                        bytes += size;
                        count ++;
                } else {
                        /* error */
                        bytes++;        /* try next byte */
                }
        }

        return( count );
}

static  int follow_insn_dest( x86_insn_t *insn ) {
        if ( insn->type == insn_jmp || insn->type == insn_jcc ||
             insn->type == insn_call || insn->type == insn_callcc ) {
                return(1);
        }
        return(0);
}

static  int insn_doesnt_return( x86_insn_t *insn ) {
        return( (insn->type == insn_jmp || insn->type == insn_return) ? 1: 0 );
}

static long internal_resolver( x86_op_t *op, x86_insn_t *insn ){
        long next_addr = -1;
        if ( x86_optype_is_address(op->type) ) {
                next_addr = op->data.sdword;
        } else if ( op->type == op_relative_near ) {
		next_addr = insn->addr + insn->size + op->data.relative_near;
        } else if ( op->type == op_relative_far ) {
		next_addr = insn->addr + insn->size + op->data.relative_far;
        }
        return( next_addr );
}

int x86_disasm_forward( unsigned char *buf, unsigned int buf_len,
                        unsigned long buf_rva, unsigned int offset,
                        DISASM_CALLBACK func, void *arg,
                        DISASM_RESOLVER resolver ){
        x86_insn_t insn;
        x86_op_t *op;
        long next_addr;
        unsigned long next_offset;
        int size, count = 0, bytes = 0, cont = 1;

        while ( cont && bytes < buf_len ) {
                size = x86_disasm( buf, buf_len, buf_rva, offset + bytes,
                           &insn );

                if ( size ) {
                        /* invoke callback if it exists */
                        if ( func ) {
                                (*func)( &insn, arg );
                        }
                        bytes += size;
                        count ++;
                } else {
                        /* error */
                        bytes++;        /* try next byte */
                }

                if ( follow_insn_dest(&insn) ) {
                        op = x86_get_dest_operand( &insn );
                        next_addr = -1;

                        /* if caller supplied a resolver, use it to determine
                         * the address to disassemble */
                        if ( resolver ) {
                                next_addr = resolver(op, &insn);
                        } else {
                                next_addr = internal_resolver(op, &insn);
                        }

                        if (next_addr != -1 ) {
                                next_offset = next_addr - buf_rva;
                                /* if offset is in this buffer... */
                                if ( next_offset >= 0 &&
                                     next_offset < buf_len ) {
                                        /* go ahead and disassemble */
                                        count += x86_disasm_forward( buf,
                                                            buf_len,
                                                            buf_rva,
                                                            next_offset,
                                                            func, arg,
                                                            resolver );
                                } else  {
                                        /* report unresolved address */
                                        x86_report_error( report_disasm_bounds,
                                                     (void *) next_addr );
                                }
                        }
                } /* end follow_insn */

                if ( insn_doesnt_return(&insn) ) {
                        /* stop disassembling */
                        cont = 0;
                }
        }
        return( count );
}

/* invariant instruction representation */
size_t x86_invariant_disasm( unsigned char *buf, int buf_len, 
		x86_invariant_t *inv ){
	if (! buf || ! buf_len || ! inv  ) {
		return(0);
	}

	return ia32_disasm_invariant(buf, buf_len, inv);
}
size_t x86_size_disasm( unsigned char *buf, unsigned int buf_len ) {
	if (! buf || ! buf_len  ) {
		return(0);
	}

	return ia32_disasm_size(buf, buf_len);
}
