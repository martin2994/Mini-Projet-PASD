# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "basic_type.h"

# include "operator_if.h"

# include "macro_operator_c.h"

# include "value_boolean.h"
# include "value_block.h"
# include "value_protected_label.h"

# include "value_error.h"

# include "operator_label.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c if: when evaluated depending the \c value_boolean on top of the stack activates or not the next value on the stack.
 *
 * Both values are destroyed.
 * Value on top must be a \c value_boolean and under should be a \c value_block or \c value_protected_label.
 * If this last \c value is a \c value_block, the chunks evaluated in order.
 * If this last \c value is a \c value_protected_label, then it as evaluated as the corresponding \c operator_label.
 *
 * If the stack is not deep enough or a wrong kind of \c value is found, then a \c basic_type_error is returned.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

static basic_type operator_if_evaluate( chunk ch, va_list va ){
     assert( NULL != ch );

     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;
     basic_type              return_value = basic_type_error;

     if (! linked_list_chunk_is_empty ( stack ) ){
         chunk first_bool_chunk = linked_list_chunk_pop_front ( stack );
         if (! linked_list_chunk_is_empty ( stack ) ){
            chunk second_chunk = linked_list_chunk_pop_front ( stack );

            if( NULL != first_bool_chunk             &&
                chunk_is_value( first_bool_chunk )   &&
                value_is_boolean( first_bool_chunk ) &&

                NULL != second_chunk                 &&
                chunk_is_value( second_chunk ) )
            {
                if( value_is_block( second_chunk ) ){

                    bool first_bool_value = basic_type_get_boolean( value_get_value( first_bool_chunk ) );
                    if( first_bool_value ){
                        interprete_chunk_list ( value_block_get_list( second_chunk ) , inter_context);
                    }
                    return_value = basic_type_void;
                }
                else
                if( value_is_protected_label( second_chunk )){
                    sstring second_value_sstring = basic_type_get_pointer( value_get_value( second_chunk ) );
                    chunk operator_label_chunk   = operator_label_create( sstring_copy( second_value_sstring ) );
                    interprete_chunk( operator_label_chunk, inter_context );
                    chunk_destroy( operator_label_chunk );
                }
            }
            chunk_destroy( second_chunk );
         }
         chunk_destroy( first_bool_chunk );
     }
     return return_value;
}

OPERATOR_BASIC_FULL( if , if )
