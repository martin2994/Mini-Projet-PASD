# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_not.h"
# include "macro_operator_c.h"

# include "value.h"
# include "value_boolean.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c !: replace the \c value_boolean on top of the stack by its negation.
 *
 * The \c value_boolean is destroyed.
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

 static basic_type operator_not_evaluate( chunk ch, va_list va){
     assert( NULL != ch );

     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;


     if (! linked_list_chunk_is_empty ( stack ) ){
         chunk ch = linked_list_chunk_pop_front ( stack );
         if( NULL != ch           &&
             chunk_is_value( ch ) &&
             value_is_boolean( ch ) ) {
               bool value_bool = basic_type_get_boolean(value_get_value(ch));
               bool not_value_bool = value_bool ? false : true;
               chunk_destroy(ch);

               linked_list_chunk_add_front( stack , value_boolean_create( not_value_bool ) );
               return basic_type_void;
         }
     }
     return basic_type_error;

 }

 OPERATOR_BASIC_FULL( not , not )
