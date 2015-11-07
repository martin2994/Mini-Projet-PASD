# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_print.h"
# include "macro_operator_c.h"


# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION


/*!
 * \file
 * \brief Operator \c print: print the top \c value of the stack on \c stdout.
 *
 * The \c value is destroyed.
 * Nothing else is modified.
 *
 * If the stack is empty, a \c basic_type_error is returned.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

 static basic_type operator_print_evaluate( chunk ch, va_list va){
     assert( NULL != ch );

     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;

     if (! linked_list_chunk_is_empty ( stack ) ){
         chunk ch = linked_list_chunk_pop_front ( stack );
         if( NULL != ch ) chunk_print( ch , stdout );
         return basic_type_void;
     }
     else{
         return basic_type_error;
     }
}

OPERATOR_BASIC_FULL( print, print )
