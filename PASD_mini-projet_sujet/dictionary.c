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
	chunk value;
	sstring key;
};

struct dictionary_struct{
	node val;
	dictionary father;
	dictionary right;
	dictionary left;
}; 
/*!
 * Generate an empty \c dictionary.
 *
 * \return an empty \c dictionary
 */
dictionary dictionary_create ( void )  { 

	dictionary d=(dictionary)malloc(sizeof(dictionary));
	d->val=NULL;
	d->father =NULL;
	d->right =NULL;
	d->left =NULL;
	return d ; 
}


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
			     chunk val )  {
/*dico vide*/
	if(dic==NULL)
	{
		node n=(node)malloc(sizeof(node));
		n->key=sstring_copy(key);
		n->value =chunk_copy(val);
		dictionary d=dictionary_create ();
		d->father=dic;
		d->val=n;
		dic->left=d;
	}else if(sstring_compare(key,dic->val->key) < 0)
	{	
		if(dic->left==NULL)
		{
			node n=(node)malloc(sizeof(node));
			n->key=sstring_copy(key);
			n->value =chunk_copy(val);
			dictionary d=dictionary_create ();
			d->father=dic;
			d->val=n;
			dic->left=d;
		}else{
			dictionary_set(dic->left,key,val);
		}
	}else if(sstring_compare(key,dic->val->key) > 0)
	{	
		if(dic->right==NULL)
		{
			node n=(node)malloc(sizeof(node));
			n->key=sstring_copy(key);
			n->value =chunk_copy(val);
			dictionary d=dictionary_create ();
			d->father=dic;
			d->val=n;
			dic->right=d;
		}else{
			dictionary_set(dic->right,key,val);
		}
	}// si la clé existe déjà on fait rien
}


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
				   sstring key )  {

	if(sstring_compare(key,dic->val->key) < 0)
	{
		return dictionary_get_copy(dic->left,key);
	}else if(sstring_compare(key,dic->val->key) > 0)
	{
		return dictionary_get_copy(dic->right,key);
	}else if(sstring_compare(key,dic->val->key) == 0){
		return chunk_copy(dic->val->value);
	}
	return NULL ;
 }


/*!
 * Destroy a \c dictionary and released associated resources.
 * All keys and values are destroyed.
 *
 * \param dic \c dictionary to destroy
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_destroy ( dictionary dic )  
{
	if (dic->left != NULL)
		dictionary_destroy(dic->left);
	if (dic->right != NULL)
		dictionary_destroy(dic->right);
	if (dic->left ==NULL && dic->right == NULL){
		chunk_destroy(dic->val->value);
		sstring_destroy(dic->val->key);
		free(dic->val);
		dic->father=NULL;
		free(dic);
  	}
}


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
			       FILE * f )  
{
	if (NULL!= dic){
	if(dic->left != NULL)
     		dictionary_print(dic->left,f);
    	chunk_print(dic->val->value,f);
    	if(dic->right != NULL)
    		dictionary_print(dic->right,f);
  }
}





