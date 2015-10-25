# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_protected_label.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file 
 * \brief \c value to hold a constant label protected/inactivated.
 *
 * A label is a letter followed by a limited number of characters among: letters, digits and '_'.
 * A label may not be a keyword (or reserved word of the language).
 *
 * \c value_protected_label are read and printed in the following form: \c '\\' followed by the label.
 * For example: \c \Bob and \c\in_34
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


const message_action value_protected_label_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(protected_label),
  {NULL, NULL}

};


basic_type value_protected_label_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  sstring value =(sstring) basic_type_get_pointer( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf(f,"\\");
  sstring_print(value,f);
  return basic_type_void; 
}

basic_type value_protected_label_destroy(chunk const ch, va_list va){ 
  basic_type* bt=(basic_type*)ch->state;
  sstring value =(sstring) basic_type_get_pointer( *bt );
  sstring_destroy(value);	
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_protected_label_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_protected_label_create(sstring_copy(basic_type_get_pointer(*bt)));
  return basic_type_pointer(c);
}

basic_type value_protected_label_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_protected_label_create ( sstring const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_protected_label_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_pointer(val);		      	
    return ch;							
  }


//VALUE_DECLARE( protected_label , sstring ) 






