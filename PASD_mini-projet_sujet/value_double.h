# ifndef __VALUE_DOUBLE_H
# define __VALUE_DOUBLE_H

# include "value.h"

# include "macro_value.h"


/*!
 * \file 
 * \brief \c value used to hold a \c double.
 *
 * For I/O these are just numbers with decimal point like 78.0 0.75 -568.58.
 * No exponential form is supported.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


VALUE_DECLARE( double , long double ) 

basic_type value_double_print(chunk const ch, va_list va);

basic_type value_double_destroy(chunk const ch, va_list va);

basic_type value_double_copy(chunk const ch, va_list va);

basic_type value_double_get_value(chunk const ch, va_list va);


# endif
