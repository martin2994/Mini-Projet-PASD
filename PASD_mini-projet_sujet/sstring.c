# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "sstring.h"

# undef NDEBUG    // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief This module provides a « safer » string.
 * 
 * These are not C-string but a \c struct that holds the length of the string and a pointer to the actual char sequence.
 * Please note that there is no \c '\0' to mark the end of the \c sstring.
 *
 * Empty string is encoded by 0 \c length and \c NULL \c pointer.
 *
 * assert is enforced.
 *
 * \note Cela ressemble au TDM 1, même s'il y a des différences.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

struct sstring_struct{
  int length;
  char * charsequence;
};

/*! \c sstring is a pointer to a hidden structure. */


/*!
 * Generate an empty \c sstring.
 *
 * \return an empty \c sstring
 */
sstring sstring_create_empty ( void )  { 
  sstring ss=(sstring)malloc(sizeof(struct sstring_struct));
  ss->length=0;
  ss->charsequence=NULL;
  return ss ;
 }


/*!
 * Generate a \c sstring with the same \c char sequence as a C-string.
 *
 * \param st C-string
 * \pre st is not \c NULL (assert-ed)
 * \return a sstring corresponding to st
 */
sstring sstring_create_string ( char const * const st )  { 
  sstring ss=(sstring)malloc(sizeof(struct sstring_struct));
  int length=strlen(st);
  char * charsequence=(char*)malloc(length*sizeof(char));
  for(int i=0;i<length;i++){
    charsequence[i]=st[i];
  }
  ss->length=length;
  ss->charsequence=charsequence;
  return ss ;
 }


/*!
 * Destroy a \c sstring and release related resources.
 *
 * \param ss C-string to destroy
 * \pre ss is a valid \c sstring (assert-ed)
 */
void sstring_destroy ( sstring ss )  {
    free(ss->charsequence);
	ss->charsequence=NULL;
    free(ss);
	ss=NULL;
}


/*!
 * Print a \c sstring to a stream.
 * There is no line return (unless it is contained in \c ss).
 *
 * \param ss \c sstring to print
 * \param f stream to print to
 * \pre ss is a valid \c sstring (assert-ed)
 * \pre f is not \c NULL (assert-ed)
 */
void sstring_print ( sstring ss ,
			    FILE * f )  {
  for(int i=0;i<ss->length;i++){
    fprintf(f,"%c",ss->charsequence[i]);
  }
}

/*!
 * Concatenate a \c sstring at the end of another.
 *
 * \param ss1 \c sstring to be modified
 * \param ss2 \c sstring to concatenate to \c ss1
 * \pre \c ss1 and \c ss2 are valid \c sstring (assert-ed)
 */
void sstring_concatenate ( sstring ss1,
			   sstring ss2 )  {
  if(!sstring_is_empty(ss1) && !sstring_is_empty(ss2)){
    int length=ss1->length+ss2->length;
    char *c=malloc(length*sizeof(char));
    for(int i=0;i<ss1->length;i++){
      for(int j=0;j<ss2->length;j++){
	c[i]=ss1->charsequence[i];
	c[j+ss1->length]=ss2->charsequence[j];
      }
    }
    free(ss1->charsequence);
    ss1->charsequence=c;
    ss1->length=length;
  }else{
    if(sstring_is_empty(ss1)){
	free(ss1->charsequence);
      char*c=malloc(ss2->length*sizeof(char));
      for(int i=0;i<ss2->length;i++){
	c[i]=ss2->charsequence[i];
      }
      ss1->charsequence=c;
      ss1->length=ss2->length;
    }
  }
}

/*!
 * Provide a copy of a string.
 *
 * \param ss \c sstring to copy
 * \pre ss is a valid \c sstring (assert-ed)
 * \return an independant copy of \c ss
 */
sstring sstring_copy ( sstring ss )  { 
	sstring cpy=NULL;
	if(!sstring_is_empty(ss)){
		cpy=sstring_create_string(ss->charsequence);
	}else{
		cpy=sstring_create_empty();
	}
	return cpy ;
 }	
	

/*!
 * Indicate how two \c sstring are ordered alphabetically.
 *
 * \param ss1 \c sstring 
 * \param ss2 \c sstring 
 * \pre ss1 and ss2 are valid \c sstring (assert-ed)
 * \return 
 * \li 0 if ss1 == ss2
 * \li -1 if ss1 < ss2
 * \li 1 otherwise
 */
int sstring_compare ( sstring ss1 ,
			     sstring ss2 )  {
  int length=0;
  if(ss1->length<ss2->length){
    length=ss2->length;
  }else{
    length=ss1->length;
  }
  for (int i=0;i<length;i++){
    if (ss1->charsequence[i]<ss2->charsequence[i]){
      return -1;
    }
    if(ss2->charsequence[i]<ss1->charsequence[i]){
      return 1;
    }
  }
  return 0 ; 
}


/*!
 * Indicate whether a string is empty.
 *
 * This function has no side effect and can be safely used in asserts.
 *
 * \param ss \c sstring to test
 * \pre ss is a valid \c sstring (assert-ed)
 * \return true ssi \c ss is empty
 */
bool sstring_is_empty ( sstring ss )  { 
  return (ss->length == 0) ;
 }


