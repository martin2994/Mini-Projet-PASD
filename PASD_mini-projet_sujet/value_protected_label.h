# ifndef __VALUE_PROTECTED_LABEL_H
# define __VALUE_PROTECTED_LABEL_H

# include "value.h"
# include "sstring.h"

# include "macro_value.h"


/*!
 * \file 
 * \brief \c value to hold a constant label protected/inactivated.
 *
 * A label is a letter followed by a limited number of characters among: letters, digits and '_'.
 * A label may not be a keyword (or reserved word of the language).
 *
 * \c value_protected_label are read and printed in the following form: \c '\\' followed by the label.
 * For example: \c \Bob and \c\in_34
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */



basic_type value_protected_label_print(chunk const ch, va_list va);

basic_type value_protected_label_destroy(chunk const ch, va_list va);

basic_type value_protected_label_copy(chunk const ch, va_list va);

basic_type value_protected_label_get_value(chunk const ch, va_list va);


VALUE_DECLARE( protected_label , sstring ) 


# endif
