# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_def.h"
# include "macro_operator_c.h"

# include "value_protected_label.h"
# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c def: add an entry into the \c dictionary.
 *
 * The entry is composed of a protected label on the top of the stack and the value under it.
 * Both values are removed from the stack.
 * Nothing else is modified.
 *
 * If the stack is not deep enough or the first one is not a \c value_protected_label, a \c basic_type_error is returned.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


static basic_type operator_def_evaluate(chunk ch, va_list va){
    assert( NULL != ch );

    interpretation_context inter_context = va_arg( va, interpretation_context );
    linked_list_chunk              stack = inter_context->stack;
    dictionary                      dico = inter_context->dic;
    basic_type              return_value = basic_type_error;


    if (! linked_list_chunk_is_empty ( stack ) ){
        chunk  first_chunk = linked_list_chunk_pop_front ( stack );

        if (! linked_list_chunk_is_empty ( stack ) ){
            chunk second_chunk = linked_list_chunk_pop_front ( stack );

            if( NULL != first_chunk                     &&
                chunk_is_value( first_chunk )           &&
                value_is_protected_label( first_chunk ) &&

                NULL != second_chunk                    &&
                chunk_is_value( second_chunk ) )
            {
                sstring first_value_sstring  = basic_type_get_pointer( value_protected_label_get_value( first_chunk, va ) );
                dictionary_set ( dico , first_value_sstring , chunk_copy(second_chunk) );



                return_value = basic_type_void;
            }
            chunk_destroy( second_chunk );
        }
         chunk_destroy( first_chunk );
    }

    return return_value;

}

OPERATOR_BASIC_FULL( def , def)
