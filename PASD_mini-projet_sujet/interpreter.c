# include <stdio.h>
# include <assert.h>


# include "value.h"
# include "operator.h"

# include "interpreter.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION



//quoi faire pour la trace
void interprete_chunk ( chunk ch ,
			interpretation_context ic )  
{
	if(chunk_is_value(ch) )
	{
		linked_list_chunk_add_front ( ic->stack , ch ); //j'empile les value
	}else if( chunk_is_operator ( ch ) )
	{
		
		basic_type bt=operator_evaluate (ch,ic);  //j'evalue l'operation
		chunk ch=(chunk)malloc(sizeof(struct chunk_struct));
		*(basic_type*)ch->state=bt;
		linked_list_chunk_add_front ( ic->stack , ch );//j'empile le res 
	}
}

void interprete_chunk_list ( linked_list_chunk ch ,
			     interpretation_context ic )  
{
	linked_list_chunk llctmp=linked_list_chunk_create ();
	while(!linked_list_chunk_is_empty (ch) )//transfert de ch à llctmp en interpretant le chunk
	{
		chunk chtmp=linked_list_chunk_pop_front (ch);
		interprete_chunk(chtmp,ic);
		linked_list_chunk_add_back(llctmp,chtmp);
	}while(!linked_list_chunk_is_empty (llctmp) ) //retransfert de llctmp à ch
	{
		linked_list_chunk_add_back(ch,linked_list_chunk_pop_front (llctmp)); 
	}
}
/*tant que le fichier est pas vide on lit et on en fait des chunk(ou une liste) qu'on interprete*/
void interprete ( FILE * f ,
		  bool do_trace )  
{
	interpretation_context ic=(interpretation_context)malloc(sizeof(struct interpretation_context_struct));
	ic->stack=linked_list_chunk_create();
	ic->dic=dictionary_create();
	ic->do_trace=do_trace;
	do{
		interprete_chunk(read_chunk_io(f),ic);
	}while(read_chunk_io (f) != END OF FILE);
}


