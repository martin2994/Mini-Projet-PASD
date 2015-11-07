# ifndef __MACRO_OPERATOR_C_H
# define __MACRO_OPERATOR_C_H


# include "value_error.h"

# undef OPERATOR_DECLARE

# define OPERATOR_DECLARE( type_name )				\
  chunk operator_ ## type_name ## _create () {			\
    return NULL;						\
  }								\
  bool operator_is_ ## type_name ( chunk const ch ) {		\
    return true ;						\
  }


/*
 * Except for operator label, all are identifcal.
 * This is simplified into making only one static instance of the operator for each kind.
 * Creation and destruction are thus handled regularly as any global variable.
 */

# define OPERATOR_BASIC_FULL( op_name , op )			                            	\
                                                                                \
                                                                                \
  static basic_type operator_ ## op_name ## _print ( chunk const ch ,	          \
						     va_list va ) {\
    FILE * io = va_arg( va, FILE*);\
    fprintf( io,"%s", #op );                               \
                                                                   \
    return basic_type_void ;						                                      \
  }									                                                            \
									\
  static basic_type operator_ ## op_name ## _destroy ( chunk const ch ,	\
						       va_list va ) {	\
    return basic_type_void ;						\
  }									\
									\
  static basic_type operator_ ## op_name ## _copy ( chunk const ch ,	\
						    va_list va ) {	\
    return basic_type_pointer(ch) ;						\
  }									\
									\
  static const message_action operator_ ## op_name ## _reactions [] = {	\
    MESSAGE_ACTION__BASIC_OPERATOR( op_name ) ,				\
    { NULL, NULL }							\
  } ;									\
									\
  static chunk_struct operator_ ## op_name ## _instance = {		\
    .state = NULL ,							\
    .reactions = operator_ ## op_name ## _reactions  } ;		\
  									\
  chunk operator_ ## op_name ## _create () {				\
    chunk ch = &operator_ ## op_name ## _instance;\
    return ch;   \
  }									\
									\
  bool operator_is_ ## op_name ( chunk const ch ) {		\
    return ch->reactions == operator_ ## op_name ## _reactions;  	\
  }



# define OPERATOR_NUMBER( op_name , op )				\
  static basic_type operator_ ## op_name ## _operate ( chunk const ch ,	\
						       va_list va ) {	\
       interpretation_context inter_context = va_arg( va, interpretation_context );\
       linked_list_chunk              stack = inter_context->stack;\
       basic_type return_value;\
       return_value = basic_type_error ;\
       \
       if (! linked_list_chunk_is_empty( stack ) ){\
            chunk first_number_chunk = linked_list_chunk_pop_front( stack );\
            if (! linked_list_chunk_is_empty( stack ) ){\
                chunk second_number_chunk = linked_list_chunk_pop_front( stack );\
                \
                if( chunk_is_value( first_number_chunk ) &&\
                    value_is_int( first_number_chunk ) &&\
                    \
                    chunk_is_value( second_number_chunk ) &&\
                    value_is_int( second_number_chunk ) ){\
                    \
                    long long int lli1  = basic_type_get_long_long_int(value_get_value( first_number_chunk ));\
                    long long int lli2  = basic_type_get_long_long_int(value_get_value( second_number_chunk ));\
                    \
                    long long int new_int = lli1 op lli2;\
\
                    linked_list_chunk_add_front( stack , value_int_create( new_int ) );\
                    return_value =  basic_type_void;\
                }\
                else\
                if( chunk_is_value( first_number_chunk ) &&\
                    value_is_double( first_number_chunk ) &&\
                    \
                    chunk_is_value( second_number_chunk ) &&\
                    value_is_double( second_number_chunk ) ){\
                    \
                    long double ld1 = basic_type_get_long_double(value_get_value( first_number_chunk ));\
                    long double ld2 = basic_type_get_long_double(value_get_value( second_number_chunk ));\
                    \
                    long double new_double  = ld1 op ld2;\
\
                    linked_list_chunk_add_front( stack , value_double_create( new_double ) );\
                    return_value =  basic_type_void;\
                }\
                \
                chunk_destroy( second_number_chunk );\
            }\
            chunk_destroy( first_number_chunk );\
       }\
       return return_value;\
  }

# define OPERATOR_BOOLEAN( op_name , op )				\
  static basic_type operator_ ## op_name ## _operate ( chunk const ch ,	\
						       va_list va ) {	\
       assert( NULL != ch );\
       \
       interpretation_context inter_context = va_arg( va, interpretation_context );\
       linked_list_chunk              stack = inter_context->stack;\
       \
       \
       if (! linked_list_chunk_is_empty ( stack ) ){\
           chunk first_bool_chunk = linked_list_chunk_pop_front ( stack );\
           if (! linked_list_chunk_is_empty ( stack ) ){\
              chunk second_bool_chunk = linked_list_chunk_pop_front ( stack );\
              \
              if( NULL != first_bool_chunk             &&\
                  chunk_is_value( first_bool_chunk )   &&\
                  value_is_boolean( first_bool_chunk ) &&\
                  \
                  NULL != second_bool_chunk             &&\
                  chunk_is_value( second_bool_chunk )   &&\
                  value_is_boolean( second_bool_chunk ) )\
              {\
                    bool first_value_bool = basic_type_get_boolean(value_get_value(first_bool_chunk));\
                    bool second_value_bool = basic_type_get_boolean(value_get_value(second_bool_chunk));\
                    bool and_value_bool = first_value_bool op second_value_bool;\
                    \
                    chunk_destroy(first_bool_chunk);\
                    chunk_destroy(second_bool_chunk);\
                    \
                    linked_list_chunk_add_front( stack , value_boolean_create( and_value_bool ) );\
                    return basic_type_void;\
              }\
           }\
       }\
       return basic_type_error;\
  }									\




# define OPERATOR_COMPARATOR( op_name , op )				\
  static basic_type operator_ ## op_name ## _operate ( chunk const ch ,	\
						       va_list va ) {	\
       assert( NULL != ch );\
       \
       interpretation_context inter_context = va_arg( va, interpretation_context );\
       linked_list_chunk              stack = inter_context->stack;\
       basic_type              return_value = basic_type_error;\
 \
 \
       if (! linked_list_chunk_is_empty ( stack ) ){\
           chunk  first_chunk = linked_list_chunk_pop_front ( stack );\
 \
           if (! linked_list_chunk_is_empty ( stack ) ){\
              chunk second_chunk = linked_list_chunk_pop_front ( stack );\
              \
             if( NULL != first_chunk             &&\
                 chunk_is_value( first_chunk )   &&\
                 value_is_boolean( first_chunk ) &&\
\
                 NULL != second_chunk            &&\
                 chunk_is_value( second_chunk )  &&\
                 value_is_boolean( second_chunk ) )\
             {\
                 bool first_value_bool  = basic_type_get_boolean(value_get_value(  first_chunk));\
                 bool second_value_bool = basic_type_get_boolean(value_get_value(second_chunk));\
                 bool equal_value_bool  =  (first_value_bool op second_value_bool);\
\
                 if( 1 op 1 ){\
                     if(equal_value_bool){\
                         return_value =  basic_type_boolean( equal_value_bool );\
                     }else{\
                         return_value = basic_type_void;\
                     }\
                }\
                else {\
                    return_value = basic_type_void;\
                }\
             }\
 \
              if( NULL != first_chunk        &&\
                  chunk_is_value( first_chunk )   &&\
                  value_is_int( first_chunk )     &&\
 \
                  NULL != second_chunk            &&\
                  chunk_is_value( second_chunk )  &&\
                  value_is_int( second_chunk ) )\
              {\
                long long int first_value_int  = basic_type_get_long_long_int( value_get_value( first_chunk ) );\
                long long int second_value_int = basic_type_get_long_long_int( value_get_value( second_chunk ) );\
 \
                bool equal_value_bool          =  (first_value_int op second_value_int);\
 \
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
              }\
 \
              else if( NULL != first_chunk        &&\
                  chunk_is_value( first_chunk )   &&\
                  value_is_double( first_chunk )  &&\
 \
                  NULL != second_chunk            &&\
                  chunk_is_value( second_chunk )  &&\
                  value_is_double( second_chunk ) )\
              {\
                 long double first_value_double  = basic_type_get_long_double( value_get_value( first_chunk ) );\
                 long double second_value_double = basic_type_get_long_double( value_get_value( second_chunk ) );\
 \
                 bool equal_value_bool  = (first_value_double op second_value_double);\
 \
                 if(equal_value_bool){\
                     linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                 }\
                 return_value = basic_type_void;\
                \
              }\
 \
              else if( NULL != first_chunk        &&\
                  chunk_is_value( first_chunk )   &&\
                  value_is_sstring( first_chunk ) &&\
 \
                  NULL != second_chunk            &&\
                  chunk_is_value( second_chunk )  &&\
                  value_is_int( second_chunk ) )\
              {\
                sstring first_value_sstring  = basic_type_get_pointer( value_get_value( first_chunk  ) );\
                sstring second_value_sstring = basic_type_get_pointer( value_get_value( second_chunk ) );\
 \
                bool equal_value_bool  = (sstring_compare(first_value_sstring, second_value_sstring) op 0);\
 \
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
              }\
           chunk_destroy( second_chunk );\
           }\
       chunk_destroy( first_chunk );\
       }\
       return return_value;\
  }\


# define OPERATOR_EQUALITY( op_name , op )				\
  static basic_type operator_ ## op_name ## _operate ( chunk const ch ,	\
                                                       va_list va ){\
      assert( NULL != ch );\
      \
      interpretation_context inter_context = va_arg( va, interpretation_context );\
      linked_list_chunk              stack = inter_context->stack;\
      basic_type              return_value = basic_type_error;\
\
\
      if (! linked_list_chunk_is_empty ( stack ) ){\
          chunk  first_chunk = linked_list_chunk_pop_front ( stack );\
\
          if (! linked_list_chunk_is_empty ( stack ) ){\
             chunk second_chunk = linked_list_chunk_pop_front ( stack );\
\
             if( NULL != first_chunk             &&\
                 chunk_is_value( first_chunk )   &&\
                 value_is_boolean( first_chunk ) &&\
\
                 NULL != second_chunk            &&\
                 chunk_is_value( second_chunk )  &&\
                 value_is_boolean( second_chunk ) )\
             {\
                 bool first_value_bool  = basic_type_get_boolean(value_get_value(  first_chunk));\
                 bool second_value_bool = basic_type_get_boolean(value_get_value(second_chunk));\
                 bool equal_value_bool  =  (first_value_bool op second_value_bool);\
\
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
             }\
\
             else if( NULL != first_chunk        &&\
                 chunk_is_value( first_chunk )   &&\
                 value_is_int( first_chunk )     &&\
\
                 NULL != second_chunk            &&\
                 chunk_is_value( second_chunk )  &&\
                 value_is_int( second_chunk ) )\
             {\
               long long int first_value_int  = basic_type_get_long_long_int( value_get_value( first_chunk ) );\
               long long int second_value_int = basic_type_get_long_long_int( value_get_value( second_chunk ) );\
\
               bool equal_value_bool          =  (first_value_int op second_value_int);\
\
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
             }\
\
             else if( NULL != first_chunk        &&\
                 chunk_is_value( first_chunk )   &&\
                 value_is_double( first_chunk )  &&\
\
                 NULL != second_chunk            &&\
                 chunk_is_value( second_chunk )  &&\
                 value_is_double( second_chunk ) )\
             {\
               long double first_value_double  = basic_type_get_long_double( value_get_value( first_chunk ) );\
               long double second_value_double = basic_type_get_long_double( value_get_value( second_chunk ) );\
\
               bool equal_value_bool  = (first_value_double op second_value_double);\
\
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
             }\
\
             else if( NULL != first_chunk        &&\
                 chunk_is_value( first_chunk )   &&\
                 value_is_sstring( first_chunk ) &&\
\
                 NULL != second_chunk            &&\
                 chunk_is_value( second_chunk )  &&\
                 value_is_int( second_chunk ) )\
             {\
               sstring first_value_sstring  = basic_type_get_pointer( value_get_value( first_chunk  ) );\
               sstring second_value_sstring = basic_type_get_pointer( value_get_value( second_chunk ) );\
\
               bool equal_value_bool  = ( 0 op sstring_compare(first_value_sstring, second_value_sstring) );\
\
                if(equal_value_bool){\
                    linked_list_chunk_add_front( stack , value_boolean_create( equal_value_bool ) );\
                }\
                return_value = basic_type_void;\
             }\
          chunk_destroy( second_chunk );\
          }\
      chunk_destroy( first_chunk );\
      }\
      return return_value;\
  }

# endif
