# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <assert.h>

# include "value_block.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION






/*!
 * \file 
 * \brief \c value used to hold an \c linked_list_chunk.
 *
 * The \c linked_list_chunk can be accessed and manipulated directly with \link value_block_get_list() \endlink.
 * Thus \c value_block is \em mutable.
 *
 * Copies are distinct (deep copy of the structure) so that acting on one does not modify any copy.
 *
 * Its output is like:
 * \verbatim
 { 
 "Bob"
 3
 +
 \Bob
 def
 true
 } \endverbatim
 *
 * In input, spaces and newlines are meaningless as long as the order of \c chunk's are preserved and they are separated, as in any \c pf program.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

const message_action value_block_reactions [] = {
  MESSAGE_ACTION__BASIC_VALUE(block),
  {NULL, NULL}

};


basic_type value_block_print(chunk const ch, va_list va){
  basic_type* bt=(basic_type*)ch->state;
  linked_list_chunk value =(linked_list_chunk) basic_type_get_pointer( *bt );
  FILE * f = va_arg ( va , FILE * ) ;
  fprintf(f,"\"");
  linked_list_chunk_print(value,f);
  fprintf(f,"\"");
  return basic_type_void; 
}

basic_type value_block_destroy(chunk const ch, va_list va){ 
  basic_type* bt=(basic_type*)ch->state;
  linked_list_chunk value =(linked_list_chunk) basic_type_get_pointer( *bt );
  linked_list_chunk_destroy(value);	
  ch->state = NULL;
  ch->reactions = NULL;
  free(ch);
  return basic_type_void;
}

basic_type value_block_copy(chunk const ch, va_list va){
    basic_type* bt=(basic_type*)(ch->state);
    chunk c = value_block_create(linked_list_chunk_copy(basic_type_get_pointer(*bt)));
  return basic_type_pointer(c);
}

basic_type value_block_get_value(chunk const ch, va_list va){
  if(ch->state != NULL){
    basic_type* bt= (basic_type*) (ch->state);
    return *bt;
  }else{
    basic_type res = basic_type_error; 
    return res;
  }
}

chunk value_block_create ( linked_list_chunk const val ) {		
    chunk ch = ( chunk ) malloc( sizeof(struct chunk_struct) );	
    ch -> state = malloc ( sizeof (basic_type) ); 
    ch->reactions = value_block_reactions;			 		
    *(basic_type*)(ch->state) = basic_type_pointer(val);		      	
    return ch;							
  }





//VALUE_DECLARE ( block , linked_list_chunk )


/*!
 * Return the \c linked_list_chunk held.
 *
 * This is not a copy but a direct access.
 *
 * \param vb chunk to query
 * \pre \c vb must be a \c value_block (assert-ed)
 * \return the \c linked_list_chunk held
 */
linked_list_chunk value_block_get_list ( chunk const vb )  {
	basic_type* bt= (basic_type*) (vb->state);
	linked_list_chunk llc=(linked_list_chunk)basic_type_get_pointer(*bt);
	return llc ;
}


VALUE_IS_FULL(block)