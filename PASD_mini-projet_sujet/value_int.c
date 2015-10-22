# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"

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


static char const * const value_int_string = "--int-- #" ;


/*!
 * Just record the error nbr as enum \link error_code \endlink.
 */
typedef struct {
    basic_type bt;
} value_error_state_struct ,
  * value_int_state ;



static basic_type value_int_get_value ( chunk const ch ,
					  va_list va ) {
    if(ch->state !=NULL){
        return (basic_type)ch->state;
    }else{
        return basic_type_error;
    } 
}


static basic_type value_int_print ( chunk const ch ,
				      va_list va ) {
  FILE * f = va_arg ( va , FILE * ) ;
  long long int value=basic_type_get_long_long_int((basic_type)ch->state);
  fprintf ( f
	    , "%s %lld"
	    , value_int_string
	    , value );
  return basic_type_void ;
}


static basic_type value_int_destroy ( chunk const ch ,
					va_list va ) {
    free ( ch -> state ) ;  
    ch -> state = NULL ;
    ch -> reactions = NULL ;
    free ( ch ) ;
    return basic_type_void ;
}


static const message_action value_int_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE( int ) ,
  { NULL, NULL }
} ;


chunk value_int_create (long long int const val  ) {
  //  Allocation
  chunk ch = ( chunk ) malloc ( sizeof ( chunk_struct ) ) ; 
  assert ( NULL != ch ) ;
  ch -> state = malloc ( sizeof ( value_int_state ) ) ; 
  assert ( NULL != ch -> state ) ;
  //  Initialisation
  (basic_type)ch->state=basic_type_long_long_int(val);
  ch -> reactions = value_int_reactions ;
  return ch ;
}


VALUE_IS_FULL( int )


VALUE_DECLARE( int , long long int ) 



