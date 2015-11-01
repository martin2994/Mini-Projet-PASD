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


struct node_struct{
    chunk val;
    sstring key;
    struct node_struct * father;
    struct node_struct * right;
    struct node_struct * left;
};


struct dictionary_struct{
    node root;
};

/*!
 * Generate an empty \c dictionary.
 *
 * \return an empty \c dictionary
 */
dictionary dictionary_create ( void )  {

	dictionary dic = (dictionary) malloc( sizeof( struct dictionary_struct ) );
  dic->root      = NULL;

	return dic ;
}

node node_create (chunk _val, sstring _key){
    if( NULL != _val &&
        NULL != _key ){
      node new_node    = malloc( sizeof( struct node_struct ));
      new_node->val    = chunk_copy( _val );
      new_node->key    = sstring_copy( _key );
      new_node->father = NULL;
      new_node->right  = NULL;
      new_node->left   = NULL;
      return new_node;
    }
    else {
      return NULL;
    }

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

    assert( NULL != dic );
    assert( NULL != key );

    node node_to_insert;

    if( NULL == dic->root ){
          dic->root = node_create ( val, key );
    }
    else
    {
        //On stocke notre racine dans une variable
        node arbreRacine  = dic->root;
        node arbreCourant = arbreRacine;
        node arbrePere    = NULL;
        //Notre variable de comparaison
        int compar = 0;
        while ( NULL != arbreCourant ){
            compar = sstring_compare( key, arbreCourant->key );
            //Si notre valeur est plus grande
            if ( compar >= 1 ){
                //On passe par le côté droit
                arbrePere            = arbreCourant;
                arbreCourant         = arbreCourant->right;
                //arbreCourant->father = arbrePere;

            }
            //Sinon on passe par le côté gauche
            else
            if( compar <= -1 ){

                arbrePere            = arbreCourant;
                arbreCourant         = arbreCourant->left;
                //arbreCourant->father = arbrePere;

            }
            else{



                if( NULL != val ){




                    chunk_destroy(arbreCourant->val);
                    sstring_destroy(arbreCourant->key);

                    arbreCourant->val = chunk_copy(val);
                    arbreCourant->key = sstring_copy(key);


                }

                return;
            }
            //Le cas où ils sont egaux ne nous interessent pas ici
        }

        //On crée un nouveau noeud en utilisant les caractéristique de notre arbre source

        node nouvelleArbre = node_create( val, key);

        //Si le père existe
        if( NULL != arbrePere ){
            //Alors on le place à gauche ou à droite de son père
            compar = sstring_compare( key, arbrePere->key );
            if (compar >= 1){
                arbrePere->right = nouvelleArbre;
            }else if (compar <= -1){
                arbrePere->left = nouvelleArbre;
            }
            //On n'oublie pas de donné à notre nouvel arbre un père
            nouvelleArbre->father= arbrePere;
        }
    }
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

    assert( NULL != dic );
    assert( NULL != key );


    //On stocke notre racine dans une variable
    node arbreCourant = dic->root;
    //Notre variable de comparaison
    int compar = 0;
    bool node_found = false;

    while ( NULL != arbreCourant ){

        compar = sstring_compare( key, arbreCourant->key );
        //Si notre valeur est plus grande
        if ( compar >= 1 ){
            //On passe par le côté droit
            arbreCourant = arbreCourant->right;
        }
        //Sinon on passe par le côté gauche
        else
        if( compar <= -1 ){
            arbreCourant = arbreCourant->left;
        }
        //Si compar vaut 0
        else
        {
            //On a notre égalité !
            //On retourne la valeur
            node_found = true;
            break;
        }
    }

    if ( node_found ){
        return chunk_copy(arbreCourant->val);
    }

    //Si notre arbre ne contient pas notre valeur on renvoi NULL
    return NULL;
}



void node_destroy ( node node_to_destroy ){

    //Si le fils gauche existe
    if( NULL != node_to_destroy->left ){
        //On fait un appel recurisif
        node_destroy( node_to_destroy->left );
    }
    //Si le fils droit existe
    if( NULL != node_to_destroy->right ){
        //On fait un appel recurisif
        node_destroy( node_to_destroy->right );
    }
    //On libère le noeud après que ses
    //successeurs ont été supprimé.

    if ( NULL != node_to_destroy->val ) chunk_destroy( node_to_destroy->val );
    if ( NULL != node_to_destroy->key ) sstring_destroy( node_to_destroy->key );
    //node_to_destroy->father = NULL;
    free(node_to_destroy);
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
    assert( NULL != dic );
    assert( NULL != dic->root );

    node_destroy( dic->root );
    dic->root = NULL;
    free(dic);
}


void node_print ( node node_to_print,
                  FILE * f ){

    if ( NULL != node_to_print ){

        if( NULL != node_to_print->left ){
            node_print( node_to_print->left , f);
        }

        fprintf( f,"\"" );
        sstring_print( node_to_print->key, f );
        fprintf( f,"\" => " );
        if( NULL != node_to_print ){
             chunk_print( node_to_print->val, f );
        }

        fprintf( f," \n" );

        if( NULL != node_to_print->right ){
            node_print( node_to_print->right , f );
        }
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
    assert( NULL != dic );
    assert( NULL != dic->root );
    assert( NULL != f );

    node_print( dic->root, f );

}
