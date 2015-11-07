# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_print_stack.h"
# include "macro_operator_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c print_stack: print the stack on \c stdout.
 *
 * For example: \verbatim
vvvvvvvv stack  top  vvvvvvvvvv
-52.580000
"zet"
{
33
pop
print
}
true
^^^^^^^^ stack bottom ^^^^^^^^^ \endverbatim
 *
 * Nothing is modified.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


 static basic_type operator_print_stack_evaluate( chunk ch, va_list va){
     assert( NULL != ch );

     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;

     if (! linked_list_chunk_is_empty ( stack ) ){

         printf("vvvvvvvv stack  top  vvvvvvvvvv\n");

         while ( ! linked_list_chunk_is_empty ( stack ) ) {
             chunk ch = linked_list_chunk_pop_front ( stack ) ;
             if( NULL != ch ) chunk_print ( ch , stdout ) ;
         }

         printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");

         return basic_type_void;
     }
     else{
         return basic_type_error;
     }
 }



 OPERATOR_BASIC_FULL( print_stack, print_stack )
