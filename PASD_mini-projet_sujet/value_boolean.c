# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_boolean.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file 
 * \brief \c value used to hold a boolean.
 *
 * For I/O, they appear as keyword \c true and \c false.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*!
 * Keyword for boolean constant \c true. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_TRUE "true"


/*!
 * Keyword for boolean constant \c false. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_FALSE "false"



const message_action value_boolean_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(boolean),
  {NULL, NULL}

};


basic_type value_boolean_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  bool value = basic_type_get_boolean( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf ( f
	    , "%s"
	    , value ? VALUE_BOOLEAN_STRING_TRUE : VALUE_BOOLEAN_STRING_FALSE );
  return basic_type_void; 
}

basic_type value_boolean_destroy(chunk const ch, va_list va){ 
  free(ch->state);
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_boolean_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_boolean_create(basic_type_get_boolean(*bt));
  return basic_type_pointer(c);
}

basic_type value_boolean_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_boolean_create ( bool const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_boolean_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_boolean(val);		      	
    return ch;							
  }

VALUE_IS_FULL(boolean)
//VALUE_DECLARE ( boolean , bool )


