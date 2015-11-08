# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_start_trace.h"
# include "macro_operator_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c start_trace: the program is traced from now on.
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

static basic_type operator_start_trace_evaluate( chunk ch, va_list va){

    assert( NULL != ch );
    interpretation_context inter_context = va_arg( va, interpretation_context );

    if (! inter_context->do_trace ){
        inter_context->do_trace = true;
    }
    return basic_type_void;

}

OPERATOR_BASIC_FULL( start_trace, start_trace )
