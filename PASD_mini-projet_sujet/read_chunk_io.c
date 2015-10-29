# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "read_chunk_io.h"

# include "value_boolean.h"
# include "value_sstring.h"
# include "value_double.h"
# include "value_int.h"
# include "value_block.h"
# include "value_protected_label.h"
# include "value_error.h"

# include "operator_addition.h"
# include "operator_division.h"
# include "operator_multiplication.h"
# include "operator_subtraction.h"
# include "operator_remainder.h"

# include "operator_less.h"
# include "operator_less_equal.h"

# include "operator_equal.h"
# include "operator_different.h"

# include "operator_and.h"
# include "operator_or.h"
# include "operator_not.h"

# include "operator_label.h"

# include "operator_creator_list.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Read \c chunk from a stream.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

/*!
 * Maximal length of a token or label.
 * Larger length labels or keywords are not allowed.
 */
# define TOKEN_KEYWORD_MAX_LEGTH 50


/*!
 * Read a token from a stream.
 *
 * The first (non-space) character is read and depending on it, only some \c chunk are possible.
 * More characters are read until:
 * \li chunk is fully read, and
 * \li there is no ambiguity  (\c"!" from \c"!=", minus operation from sign of number…).
 * If one (or more) \c char that could belong to another \c chunk are read to settle ambiguity, they are put back (with \c ungetc).
 *
 * Label (protected or not) are distinguished from keywords by using the table in \c operator_creator_list.
 *
 * Static functions are used to handle separately numbers (int or float), label or keyword, sstring, block…
 *
 * Following spaces are discarded.
 *
 * If the OEF is reached before anything is read, then a \c value_error with \c VALUE_ERROR_IO_EOF is returned.
 *
 * If the input cannot form a legal \c chunk, then a corresponding  \c value_error is returned.
 *
 * \param f \c stream to read from
 * \pre \c f is not \c NULL (assert-ed)
 * \return chunk read (should be a \c value or an \c operator) or a \c value_error if reading fail.
 */



chunk read_chunk_io ( FILE * f )  {

    assert(NULL != f);

    //Variables pour le fgetc, le char actuel et celui qui le suit
    int actual_c = fgetc(f);
    int next_c = 0;


    //Variable pour capturer un string en lecture
    char string_input[TOKEN_KEYWORD_MAX_LEGTH];

    //Variable pour la position dans
    //le fichier avant de lire le premier char :
    long pos_after_c;

    //Variables pour comparer les strings :
    int diff_string,
        diff_second_string,
        diff_third_string,
        diff_fourth_string = -2;

    //On met en abstraction tout les espaces et les retours à la liggne,
    //les tabulations et les retour chariot
    while(   actual_c == (int) ' '
          || actual_c == (int) '\n'
          || actual_c == (int) '\t'
          || actual_c == (int) '\r' ){

        actual_c = fgetc(f);

        if(feof(f)){
          printf( "End of File !" );
          break;
        }
    }

    //On switch sur le premier character rencontré,
    //il nous permettra de savoir vers quel value ou operator
    //doit-on agir.
    switch( actual_c ){

        /***************************************
        *              Operators               *
        ***************************************/
        //ADDITION
        case( (int) '+' ):
            //On retourne l'operateur operator_addition
            printf("\nOperator + created !\n");
            break;
        //EQUAL
        case( (int) '=' ):
            //On retourne l'operateur operator_equal
            printf("\nOperator = created !\n");
            break;
        //DIVISION

        case( (int) '/' ):
            //On retourne l'operateur operator_division
            printf("\nOperator / created !\n");
            break;

        //MULTIPLICATION
        case( (int)'*'):
            //On retourne l'operateur operator_multiplication
            printf("\nOperator * created !\n");
            break;

        //LESS - LESS EQUAL
        case( (int) '<'):
            next_c = fgetc(f);

            //LESS EQUAL
            if( (int) '=' == next_c ){
                //On retourne l'operateur operator_less_equal
                printf("\nOperator <= created !\n");
            }
            //LESS
            else {
                ungetc( next_c, f );
                //On retourne l'operateur operator_less
                printf("\nOperator < created !\n");
            }
            break;

        //NOT - DIFFERENT
        case( (int) '!' ):
            next_c = fgetc(f);

            //DIFFERENT
            if( (int) '=' == next_c ){
                //On retourne l'operateur operator_different
                printf("\nOperator != created !\n");

            //NOT
            } else {
                ungetc( next_c, f );
                //On retourne l'operateur operator_not
                printf("\nOperator ! created !\n");
            }
            break;

        //AND
        case( (int) '&' ):

            next_c = fgetc( f );

            if ((int) '&' == next_c){
                //return operator_and
                printf("\nOperator && created !\n");
            }
            else{
              ungetc(next_c, f);
            }
            break;

        //OR
        case( (int) '|' ):

            next_c = fgetc( f );

            if ((int) '|' == next_c){
                //return operator_or
                printf("\nOperator || created !\n");
            }
            else{
              ungetc(next_c, f);
            }
            break;

        //REMAINDER
        case( (int) '%' ):
            //On retourne l'operateur operator_remainder
            printf("\nOperator remainder created !\n");
            break;

        /***************************************
        *               Keywords               *
        ***************************************/

        //NOP
        case( (int) 'n' ):

            pos_after_c = ftell(f);
            ungetc(actual_c, f);


            fscanf(f, "%s", string_input);
            diff_string = strncmp(string_input, "nop", TOKEN_KEYWORD_MAX_LEGTH);
            if( 0 == diff_string ){
                //create operator_not
                printf("\nOperator_nop created !\n");
            }
            else{
                //On revient à la position initiale
                fseek(f, pos_after_c, SEEK_SET);
            }
            break;

        //POP - PRINT_STACK - PRINT_DICTIONARY - PRINT
        case( (int) 'p' ):

            pos_after_c  = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string         = strncmp(string_input, "pop", TOKEN_KEYWORD_MAX_LEGTH);
            diff_second_string  = strncmp(string_input, "print_stack", TOKEN_KEYWORD_MAX_LEGTH);
            diff_third_string   = strncmp(string_input, "print_dictionary", TOKEN_KEYWORD_MAX_LEGTH);
            diff_fourth_string  = strncmp(string_input, "print", TOKEN_KEYWORD_MAX_LEGTH);


            if( 0 == diff_string ){
                //create operator_pop
                printf("\nOperator_pop created !\n");
            }
            else
            if( 0 == diff_second_string ){
                //create operator_print_stack
                printf("\nOperator_print_stack created !\n");
            }
            else
            if( 0 == diff_third_string ){
                //create operator_print
                printf("\nOperator_print_dictionary created !\n");
            }
            else
            if( 0 == diff_fourth_string ){
                //create operator_print
                printf("\nOperator_print created !\n");
            }
            else{
                //Si aucun des mot clé de correspond on reviens à la position
                //après le premier char et on l'ignore
                fseek( f, pos_after_c, SEEK_SET );
            }
            break;

        //COPY
        case( (int) 'c' ):
            pos_after_c = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string = strncmp(string_input, "copy", TOKEN_KEYWORD_MAX_LEGTH);
            if( 0 == diff_string ){
                //create operator_copy
                printf("\nOperator_copy created !\n");
            }
            else{
                //On revient à la position initiale
                fseek(f, pos_after_c, SEEK_SET);
            }
            break;

        //WHILE
        case( (int) 'w' ):
            pos_after_c = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string = strncmp(string_input, "while", TOKEN_KEYWORD_MAX_LEGTH);
            if( 0 == diff_string ){
                //create operator_while
                printf("\nOperator_while created !\n");
            }
            else{
                //On revient à la position initiale
                fseek(f, pos_after_c, SEEK_SET);
            }
            break;

        //DEF
        case( (int) 'd' ):
            pos_after_c = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string = strncmp(string_input, "def", TOKEN_KEYWORD_MAX_LEGTH);
            if( 0 == diff_string ){
                //create operator_def
                printf("\nOperator_def created !\n");
            }
            else{
                //On revient à la position initiale
                fseek(f, pos_after_c, SEEK_SET);
            }
            break;

        //START_TRACE - STOP_TRACE
        case( (int) 's' ):
            pos_after_c  = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string        = strncmp(string_input, "start_trace", TOKEN_KEYWORD_MAX_LEGTH);
            diff_second_string = strncmp(string_input, "stop_trace",  TOKEN_KEYWORD_MAX_LEGTH);
            if ( 0 == diff_string ){
                //create operator_start_trace
                printf("\nOperator_start_trace created !\n");
            }
            else
            if ( 0 == diff_second_string ){
                //create operator_stop_trace
                printf("\nOperator_stop_trace!\n");
            }
            else{
                fseek(f, pos_after_c, SEEK_SET);
            }
            break;


        //IF - IF_ELSE
        case( (int) 'i' ):
            pos_after_c  = ftell(f);
            ungetc(actual_c, f);

            fscanf(f, "%s", string_input);
            diff_string        = strncmp(string_input, "if", TOKEN_KEYWORD_MAX_LEGTH);
            diff_second_string = strncmp(string_input, "if_else", TOKEN_KEYWORD_MAX_LEGTH);

            if( 0 == diff_string ){
                //create operator_if
                printf("\nOperator_if !\n");
            }
            else
            if ( 0 == diff_second_string ){
                //create operator_if_else
                printf("\nOperator_if_else !\n");
            }
            else{
                fseek( f, pos_after_c, SEEK_SET );
            }
            break;

        //END OF FILE
        case( EOF ):
            return value_error_create( VALUE_ERROR_IO_EOF );
            break;

        /***************************************
        *                Values                *
        ***************************************/


        //INT - FLOAT - MINUS
        case ((int)'-'):
            next_c = fgetc(f);
            if(isDigit(next_c) ){
                    //On remet à ça place notre chiffre
                    ungetc(next_c, f);
                    //Puis on récupère notre int d'un coup
                    //d'un seul.
                    fscanf(f, "%s", string_input);
                    //On crée ensuite notre chunk
                    //en appelant la fonction createNumber.
                    return createNumber(string_input, true);
                }
            else{
                printf("Ceci est un moins : '-'.\n");
                //"create operator_less"
            }
            break;

        //BOOLEAN
        case ((int)'t'):
        case ((int)'f'):
              pos_after_c = ftell(f);
              ungetc(actual_c, f);
              fscanf(f, "%s", string_input);

              int diff_t = strncmp(string_input, "true", TOKEN_KEYWORD_MAX_LEGTH);
              int diff_f = strncmp(string_input, "false", TOKEN_KEYWORD_MAX_LEGTH);

              if(0 == diff_t){
                  printf("\nTrue created\n\n");
                  return value_boolean_create(true);
              }
              else if ( 0 == diff_f){
                  printf("\nFalse created\n\n");
                  return value_boolean_create(false);
              }else{
                  fseek(f, pos_after_c, SEEK_SET);
              }
              break;

        //SSTRING
        case ( (int) '"' ):
            for (int i = 0; i < TOKEN_KEYWORD_MAX_LEGTH ;i++){
                next_c = fgetc(f);
                if( next_c == (int)'"' ){
                    string_input[i] = '\0';
                    ungetc(next_c, f);
                    break;
                }
                else{ string_input[i] = (char)next_c; }
            }
            assert( NULL != string_input );
            sstring new_sstring = sstring_create_string(string_input);
            printf("\nSstring created : %s\n\n", string_input);
            return value_sstring_create(new_sstring);
            break;
        //LABEL
        case ( 92 ):
        //ASCII 92 = '\' (backslash)
            fscanf(f, "%s", string_input);
            printf("\n");

            if(!isKeyWord(string_input)){
                new_sstring = sstring_create_string(string_input);
                printf("\nProtected label created : %s !\n", string_input);
                return value_protected_label_create(new_sstring);
            }else{

                printf("\nIt's a keyword : %s !\n\n", string_input);
            }
            break;

        //BLOCK


        default:
            //INT - FLOAT
            if( isDigit(actual_c) ){

                //On remet à ça place notre chiffre
                ungetc(actual_c, f);
                //Puis on récupère notre int d'un coup
                //d'un seul.
                fscanf(f, "%s", string_input);
                //On crée ensuite notre chunk
                //en appelant la fonction createNumber.
                return createNumber(string_input, false);
              }
            break;
    }
    return NULL ;
}

bool isDigit(int input){
    if((char)input == '0' ||
       (char)input == '1' ||
	     (char)input == '2' ||
       (char)input == '3' ||
       (char)input == '4' ||
	     (char)input == '5' ||
	     (char)input == '6' ||
       (char)input == '7' ||
       (char)input == '8' ||
       (char)input == '9'){
          return true;
        }
    return false;
}

bool isFloat(char * number){
  return (strstr(number, ".") != NULL) ? true : false;
}

bool isKeyWord(char * input){
    int diff_string;
    const char * keyword_list[13];
    keyword_list[0] = "nop";
    keyword_list[2] = "pop";
    keyword_list[3] = "print";
    keyword_list[4] = "copy";
    keyword_list[5] = "start_trace";
    keyword_list[6] = "stop_trace";
    keyword_list[7] = "print_stack";
    keyword_list[8] = "if";
    keyword_list[9] = "if_else";
    keyword_list[10] = "while";
    keyword_list[11] = "def";
    keyword_list[12] = "print_dictionary";

    for(int i=0; i<12; i++){
        diff_string = strncmp(input, keyword_list[i], TOKEN_KEYWORD_MAX_LEGTH);
        if(diff_string == 0){return true;}
    }
    return false;
}

chunk createNumber(char * string_input, bool isNegativ){

    long long int new_int;
    double        new_double;

    if(isFloat(string_input)){
      char * string_converter;
      new_double = strtod (string_input, &string_converter);

      if(new_double){
          if(isNegativ){ new_double = -new_double; }
          printf("\nFinal value : %f\n", new_double);
          return value_double_create(new_double);
      }
    }
    else{
        new_int = atoll(string_input);
        if(new_int){
            if(isNegativ){ new_int = -new_int; }
            printf("\nFinal value : %lld\n", new_int);
            return value_int_create(new_int);
        }
    }
    return NULL;
}
