# include <stdlib.h>
# include <stdio.h>

# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "chunk.h"

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


int main (){

	FILE * f = fopen("./test_read_chunk.txt", "r");

	if(NULL == f){
		perror("Erreur lors de l'ouverture du fichier.\n");
		return 0;
	}

	printf("============================================================\n");
	printf("\nRead chunk io\n");
	printf("\n=============================================================\n\n\n");

	chunk ch;
	for (int i = 0; i < 12; i++){
			ch = read_chunk_io(f);
	}


  printf("\n\n\n============================================================\n");
	fclose(f);
	return 0;
}
