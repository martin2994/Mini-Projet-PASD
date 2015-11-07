# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_nop.h"
# include "macro_operator_c.h"
# include "operator.h"
# include "chunk.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c nop: does nothing.
 *
 * It is just discarded.
 * Nothing is modified.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

static basic_type operator_nop_evaluate( chunk ch, va_list va){
    return basic_type_void;
}

OPERATOR_BASIC_FULL(nop, nop)
