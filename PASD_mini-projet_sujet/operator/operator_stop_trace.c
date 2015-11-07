# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_stop_trace.h"
# include "macro_operator_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c stop_trace: the program is no more traced.
 *
 * If trace was on, then the end of tracing is traced.
 *
 * Nothing is modified.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

 static basic_type operator_stop_trace_evaluate( chunk ch, va_list va){

     assert( NULL != ch );
     interpretation_context inter_context = va_arg( va, interpretation_context );

     if ( inter_context->do_trace ){
         inter_context->do_trace = false;
     }
     return basic_type_void;

 }

 OPERATOR_BASIC_FULL( stop_trace, stop_trace )
