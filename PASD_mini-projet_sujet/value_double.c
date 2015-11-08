# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_double.h"

# include "macro_value_c.h"



# undef NDEBUG   // FORCE ASSERT ACTIVATION!_




/*!
 * \file 
 * \brief \c value used to hold a \c double.
 *
 * For I/O these are just numbers with decimal point like 78.0 0.75 -568.58.
 * No exponential form is supported.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


const message_action value_double_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(double),
  {NULL, NULL}

};


basic_type value_double_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  long double value = basic_type_get_long_double( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%Lf"
	    , value );
  return basic_type_void; 
}

basic_type value_double_destroy(chunk const ch, va_list va){ 
  free(ch->state);
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_double_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_double_create(basic_type_get_long_double(*bt));
  return basic_type_pointer(c);
}

basic_type value_double_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_double_create ( long double const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_double_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_long_double(val);		      	
    return ch;							
  }

VALUE_IS_FULL(double)
//VALUE_DECLARE( double , long double ) 


