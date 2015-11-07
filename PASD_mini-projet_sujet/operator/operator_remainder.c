# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_remainder.h"
# include "macro_operator_c.h"

# include "value_int.h"

# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c %: replace the two \c value_int's on top of the stack by the remainder of their integer division.
 *
 * The \c value's must be \c value_int.
 * Result is a \c value_int.
 * The \c value_int's are destroyed.
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

static basic_type operator_remainder_evaluate ( chunk ch, va_list va){

     assert( NULL != ch);
     interpretation_context inter_context = va_arg( va, interpretation_context );
     linked_list_chunk              stack = inter_context->stack;
     basic_type return_value;
     return_value = basic_type_error ;

     if (! linked_list_chunk_is_empty( stack ) ){
          chunk first_number_chunk = linked_list_chunk_pop_front( stack );
          if (! linked_list_chunk_is_empty( stack ) ){
              chunk second_number_chunk = linked_list_chunk_pop_front( stack );

              if( chunk_is_value( first_number_chunk ) &&
                  value_is_int( first_number_chunk ) &&

                  chunk_is_value( second_number_chunk ) &&
                  value_is_int( second_number_chunk ) ){

                  long long int lli1  = basic_type_get_long_long_int(value_get_value( first_number_chunk ));
                  long long int lli2  = basic_type_get_long_long_int(value_get_value( second_number_chunk ));

                  long long int new_int = lli1 % lli2;
                  return_value =  basic_type_long_long_int( new_int );
              }
              chunk_destroy( second_number_chunk );
          }
          chunk_destroy( first_number_chunk );
     }
     return return_value;
}

OPERATOR_BASIC_FULL( remainder, remainder )
