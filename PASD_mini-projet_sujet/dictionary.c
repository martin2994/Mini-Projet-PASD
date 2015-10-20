# include <stdlib.h> // malloc + free
# include <assert.h>

# include "dictionary.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file
 * \brief Provide a dictionary (associative table).
 * \li keys must be non-empty \c sstring's
 * \li values must be \c chunk's
 *
 * Keys and values are copied to get independent long term storage.
 * In the same spirit, queried values are \b copies. 
 * The <b>caller is in charge of destroying copies</b>; this is indicated by the \c _copy in the query function name.
 *
 * Values are stored in an ordered binary tree.
 * They are two data structures: one for the dictionary and one for nodes.
 *
 * \note Cela ressemble au TDM 3, même s'il y a des différences.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*! \c dictionary is a pointer to the hidden structure for dictionary. */
typedef struct node_struct * node;

struct node_struct{
	chunk* value;
	sstring key;
};

struct dictionary_struct{
	dictionary pere;
	dictionary filsdroit;
	dictionary filsgauche;
}; 
/*!
 * Generate an empty \c dictionary.
 *
 * \return an empty \c dictionary
 */
dictionary dictionary_create ( void )  { return NULL ; }


/*!
 * Add an entry \c (key,val) into a \c dictionary.
 *
 * Copies of key and val are made.
 * The original values can be safely destroyed after the call.
 *
 * \param dic \c dictionary to modify
 * \param key insertion key
 * \param val inserted value
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not an empty string  (assert-ed)
 */
void dictionary_set ( dictionary dic ,
			     sstring key ,
			     chunk val )  {}


/*!
 * Retrieve a \b copied value from a \c dictionary according to a \c key.
 *
 * \param dic \c dictionary to query from
 * \param key key to search a value for
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not empty
 * \return a \b copy of the associated \c chunk or NULL if undefined 
 */
chunk dictionary_get_copy ( dictionary dic ,
				   sstring key )  { return NULL ; }


/*!
 * Destroy a \c dictionary and released associated resources.
 * All keys and values are destroyed.
 *
 * \param dic \c dictionary to destroy
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_destroy ( dictionary dic )  {}


/*!
 * Print a \c dictionary to a stream.
 * Entries are printed in \c key alphabetical order.
 * The format is as in the following example with and \c value_integer and a \c value_blok
 \verbatim
 "Bob" => 3
 "Increment_Bob" => {
 Bob
 1
 +
 \Bob
 def
 } \endverbatim
 * \param dic \c dictionary to print
 * \param f stream to print to
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_print ( dictionary dic ,
			       FILE * f )  {}





