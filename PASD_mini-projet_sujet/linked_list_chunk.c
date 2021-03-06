# include <stdlib.h>   // malloc + free
# include <stdbool.h>
# include <assert.h>

# include "linked_list_chunk.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION


//Martin Réty / Bryce Marbois / Ayoub El Fahim

/*!
 * \file
 * \brief Structure used to store list of \link chunk\endlink 's.
 *
 * Please note that \c linked_list_chunk does not make any copy of \c chunk's.
 *
 * The list is bidirectional and can be accessed from both end.
 *
 * The structure is 2-level:
 * \li \c link's are used to provide the bidirectional links (with pointers) and points to \c chunk
 * \li the \c main structure has pointers to first and last \c link's.
 *
 * The empty list has the pointers of its \c main structure at \c NULL.
 *
 * \note Cela ressemble au TDM2 (sans être circulaire).
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*!
 * \c linked_list_chunk is a pointer to a hidden structure (\c main structure).
*/
typedef struct link_struct* link;
 struct link_struct{
  chunk value;
  link previous;//head
  link next;//tail
};

struct linked_list_chunk_struct{
  link  first;
  link  last;
};

/*!
 * Generate an empty \c linked_list_chunk
 *
 * \return an empty \c linked_list_chunk
 */
linked_list_chunk linked_list_chunk_create ( void )  {
  linked_list_chunk llc = (linked_list_chunk)malloc( sizeof( struct linked_list_chunk_struct ) );
  llc->first            = NULL;
  llc->last             = NULL;
  return llc ;
 }

link link_create ( chunk _value ){
    assert ( NULL != _value );
    link l      = (link) malloc( sizeof( struct link_struct ) );
    l->value    = _value ;
    l->previous = NULL;
    l->next     = NULL;
    return l;

}


/*!
 * Destroy the whole structure and the stored values.
 *
 * \param llc \c linked_list_chunk to destroy
 * \pre \c llc is valid (assert-ed)
 */
void linked_list_chunk_destroy ( linked_list_chunk llc )  {
  assert( NULL != llc );

  if(!linked_list_chunk_is_empty(llc))
  {
    	link l = llc->first;

    	while (l->next!=NULL){
          llc->first = l->next;
          chunk_destroy(l->value);
        	l          = l->next;
        	free(l->previous);

    	}
    	chunk_destroy(l->value);
    	free(l);
  }
  free(llc);
}


/*!
 * To know whether a \c linked_list_chunk is empty.
 *
 * \param llc \c linked_list_chunk to test
 * \pre \c llc is valid (assert-ed)
 * \return true iff \c llc is empty
 */
bool linked_list_chunk_is_empty ( linked_list_chunk llc)  {
  return ( llc->first == NULL);
 }


/*!
 * To print a \c linked_list_chunk.
 * Each chink is printed on a separate line with \c chunk_print.
 *
 * \param llc \c linked_list_chunk to print
 * \param f stream to print to
 * \pre \c llc is valid (assert-ed)
 * \pre \c f is not \c NULL (assert-ed)
 */
void linked_list_chunk_print ( linked_list_chunk llc ,
				      FILE * f )  {
  assert( NULL != llc );
  link l = llc->first;
  while( l != NULL){
    chunk_print( l->value, f );
    fprintf(f,"\n");
    l = l->next;
  }
}

/*!
 * Add a chunk at the beginning of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_front ( linked_list_chunk llc ,
					  chunk ch )  {
    assert( NULL != llc );
    assert( NULL != ch );

    if(linked_list_chunk_is_empty(llc)){
        link l     = link_create( ch );
        llc->first = l;
        llc->last  = l;
    }
    else{
        link l               = link_create( ch );
        l->next              = llc->first;
        llc->first->previous = l;
        llc->first           = l;
    }
}

/*!
 * Add a \c chunk at the end of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_back ( linked_list_chunk llc ,
					 chunk ch )  {

    assert( NULL != llc );
    assert( NULL != ch );

    if( linked_list_chunk_is_empty( llc ) ){
        link l     = link_create( ch );
        llc->first = l;
        llc->last  = l;
    }
    else{
        link l          = link_create( ch );
        l->previous     = llc->last;
        llc->last->next = l;
        llc->last       = l;
    }
}

/*!
 * Return the \c chunk at the beginning of the \c linked_list_chunk.
 * The \c chunk is removed from the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to pop from
 * \return The removed \c chunk at the beginning or \c NULL if linked_list_chunk empty
 */
chunk linked_list_chunk_pop_front ( linked_list_chunk llc )  {

    assert( NULL != llc );

    if (linked_list_chunk_is_empty(llc)){
        return NULL;
    }

    link l = llc->first;

    chunk ch = l->value;

    if( llc->first == llc->last ){
        free( l );
        llc->first = NULL;
        llc->last  = NULL;

        //chunk_destroy( l->value );

    }else{
        llc->first = l->next;
        //chunk_destroy( l->value );
        free( l );
    }

    return ch;
}



/*!
 * Add a \b copy of the \c k first \c chunk at the beginning of the \c linked_list_chunk to it-self.
 * If there is less than \c k \c chunk then no copy is made.
 *
 * For \c k, the following \c linked_list_chunk
 * \verbatim [front]                                     ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim
 * is transformed into
 * \verbatim [front]  ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim
 *
 * \param llc \c linked_list_chunk to add to
 * \param k size of the prefix to copy in front
 * \pre \c llc is valid (assert-ed)
 * \return false if there where less than k element. In such a case, no copy is made.
 */
bool linked_list_chunk_add_self_copy_front ( linked_list_chunk llc ,
						    unsigned int k )  {

    assert( NULL != llc );

    if( linked_list_chunk_is_empty( llc ) ){
        if( k!=0 ){
        	return false;
        }
        else{
        	return true;
        }
    }

    link p = llc->first;

    for( unsigned int i=0; i < k-1; i++ ){
        if( p->next == NULL ){
            return false;
        }
        p = p->next;
    }
    for ( unsigned int i = k; i > 0; i-- ){
        linked_list_chunk_add_front( llc, chunk_copy(p->value) );
        p = p->previous;
    }
    return true;
}

/*!
 * Generate a copy of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to copy
 * \pre \c llc is valid (assert-ed)
 */
 linked_list_chunk linked_list_chunk_copy ( linked_list_chunk llc )  {

    assert( NULL != llc );
    linked_list_chunk cpy = linked_list_chunk_create();

    if ( !linked_list_chunk_is_empty(llc) ){
        link p =  llc->first;

        while ( NULL != p->next ){
            linked_list_chunk_add_back( cpy, chunk_copy(p->value));
            p = p->next;
        }
        linked_list_chunk_add_back(cpy, chunk_copy(p->value));
    }
    return cpy;
}
