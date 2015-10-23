# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"

# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION

//Réty Martin, Marbois Bryce, Imad SALKI, Ayoub EL FAHIM
/*
*
* 
* Nous avons un problème de core dumped à la compilation du value_int, 
*après de nombreuses modifications nous n'avons pas réussi à résoudre le problème.
* Pouvez nous donner des pistes pour la résoudre.
*
*
*
*

**/


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


static basic_type value_int_get_value ( chunk const ch ,
					  va_list va ) {
    if(ch->state !=NULL){	
        basic_type* bt=(basic_type*)ch->state;
	return *bt;
    }else{
        return basic_type_error;
    } 
}


static basic_type value_int_print ( chunk const ch ,
				      va_list va ) {
  FILE * f = va_arg ( va , FILE * ) ;
  basic_type * bt=(basic_type*)ch->state;
  long long int value=basic_type_get_long_long_int(*bt);
  fprintf ( f
	    , "%lld"
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

static basic_type value_int_copy ( chunk const ch ,
				     va_list va ) {
  basic_type * bt=(basic_type*)ch->state;	
 chunk c= value_int_create(basic_type_get_long_long_int(*bt));
 return basic_type_pointer( c ) ; 
}

static const message_action value_int_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE( int ) ,
  { NULL, NULL }
} ;


chunk value_int_create (long long int const val  ) {
  //  Allocation
  chunk ch = ( chunk ) malloc ( sizeof ( chunk_struct ) ) ; 
  assert ( NULL != ch ) ;
  basic_type bt=basic_type_long_long_int(val);
  ch->state=&bt;
  assert ( NULL != ch -> state ) ;
  ch -> reactions = value_int_reactions ;
  return ch ;
}


//VALUE_IS_FULL( int )


//VALUE_DECLARE( int , long long int ) 



