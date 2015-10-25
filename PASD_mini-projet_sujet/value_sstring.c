# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_sstring.h"

# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file 
 * \brief \c value to hold a \c sstring.
 *
 * \c value_sstring are read and printed with \c '"' delimiters like <tt>"ed fe"</tt>.
 * On input, use \c "\"" for a \c '"', \c '\n' for a newline, \c '\t' for a tabulation and \c '\\' for a backspace char.
 * Protective \c '\\' are not restored on printing (i.e. follows the \c print of \link sstring\endlink)
 *
 * There is \em no \em limit to the length of a string.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

const message_action value_sstring_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(sstring),
  {NULL, NULL}

};


basic_type value_sstring_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  sstring value =(sstring) basic_type_get_pointer( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf(f,"\"");
  sstring_print(value,f);
  fprintf(f,"\"");
  return basic_type_void; 
}

basic_type value_sstring_destroy(chunk const ch, va_list va){ 
  basic_type* bt=(basic_type*)ch->state;
  sstring value =(sstring) basic_type_get_pointer( *bt );
  sstring_destroy(value);	
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_sstring_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_sstring_create(sstring_copy(basic_type_get_pointer(*bt)));
  return basic_type_pointer(c);
}

basic_type value_sstring_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_sstring_create ( sstring const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_sstring_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_pointer(val);		      	
    return ch;							
  }



//VALUE_DECLARE ( sstring , sstring ) 


