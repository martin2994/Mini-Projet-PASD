# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"
//# include "stdbool.h"
# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION

/*!
 * \file 
 * \brief \c value to hold an \c int.
 * 
 * Integers are read and printed in the usual way:
 * \li '-' sign if negative (nothing for positive) followed by
 * \li sequence of digits.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


const message_action value_int_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(int),
  {NULL, NULL}

};


basic_type value_int_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  long long int value = basic_type_get_long_long_int( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%lld"
	    , value );
  return basic_type_void; 
}

basic_type value_int_destroy(chunk const ch, va_list va){ 
  free(ch->state);
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_int_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_int_create(basic_type_get_long_long_int(*bt));
  return basic_type_pointer(c);
}

basic_type value_int_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_int_create ( long long int const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_int_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_long_long_int(val);		      	
    return ch;							
  }

VALUE_IS_FULL(int)
//VALUE_DECLARE( int, long long int )
