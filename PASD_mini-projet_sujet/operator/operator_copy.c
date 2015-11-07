# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_copy.h"
# include "macro_operator_c.h"

# include "value_int.h"
# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator copy: duplicate the \c k top values on the stack.
 *
 * Integer \c k is on top of the stack.
 * Once \c k is removed, the remaining top \c k elements are duplicate.
 *
 * On
\verbatim [top]                                k   ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2\endverbatim
* the evaluation of \c operator_copy results in:
\verbatim [top] ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2\endverbatim
 *
 * If there are less than \k elements, only the present elements are duplicated; this is not considered as an error.
 *
 * If the top element is not a \c value_int an error is returned.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


 static basic_type operator_copy_evaluate( chunk ch, va_list va){

     assert( NULL != ch );
     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;


     if (! linked_list_chunk_is_empty( stack ) ){
         chunk chunk_k = linked_list_chunk_pop_front( stack );
         if( chunk_is_value( chunk_k) &&
             value_is_int( chunk_k ) ){

             long long int k = basic_type_get_long_long_int(value_get_value( chunk_k ));
             //Le k' avec lequelle on peut appeler k' copies
             //( dans le cas ou le nombre d'élement est inférieur à k)
             unsigned int real_k = 0;

             while (!linked_list_chunk_is_empty( stack ) && k > 0 ){
                  real_k++;
             }

             linked_list_chunk_add_self_copy_front ( stack , real_k );
             chunk_destroy( chunk_k );
         }else{
            return basic_type_error;
         }
     }
     return basic_type_void;

 }

 OPERATOR_BASIC_FULL( copy, copy )
