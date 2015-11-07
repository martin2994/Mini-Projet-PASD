# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "operator_division.h"
# include "macro_operator_c.h"

# include "value_double.h"
# include "value_int.h"

# include "value_error.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief Operator \c ∕: replace the two \c value's on top of the stack by the quotient of their division.
 *
 * The \c value's must be integer or float.
 * If both values are integer, then this is the result of the integer division.
 * The \c value's are destroyed.
 *
 * If the stack is not deep enough or a wrong kind of \c value is found, then a \c basic_type_error is returned.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


OPERATOR_NUMBER( division , / )

static basic_type operator_division_evaluate( chunk ch, va_list va){
    return operator_division_operate( ch, va );
}

OPERATOR_BASIC_FULL( division, division )
