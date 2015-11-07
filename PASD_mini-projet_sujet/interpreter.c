# include <stdio.h>
# include <assert.h>


# include "value.h"
# include "operator.h"

# include "interpreter.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION




void interprete_chunk ( chunk ch ,
			interpretation_context ic )  
{
  if(ic->do_trace)
    {
      printf("==**== reading: ");
    }
  if(chunk_is_value(ch) )
    {
      linked_list_chunk_add_front ( ic->stack , ch ); //j'empile les value
      if(ic->do_trace)
	{
	  chunk_print(ch,stdout);
	   printf( " (value)\n");
	}
    }else if( chunk_is_operator ( ch ) )
    {
      
      basic_type bt=operator_evaluate (ch,ic);  //j'evalue l'operation
      chunk ch=(chunk)malloc(sizeof(struct chunk_struct));
      *(basic_type*)ch->state=bt;
      linked_list_chunk_add_front ( ic->stack , ch );//j'empile le res 
      if(ic->do_trace)
	{
	  chunk_print(ch,stdout);
	  printf(" (operator)\n");
	}	
    }
  if(ic->do_trace)
    {
      printf("vvvvvvvv stack  top  vvvvvvvvvv\n");
      linked_list_chunk_print(ic->stack,stdout);
      printf("^^^^^^^^ stack bottom ^^^^^^^^^\n");
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
  for (int i=0; i<4;i++){
  // while(read_chunk_io(f) != NULL){
    interprete_chunk(read_chunk_io(f),ic);
    }
  //afficher le dico et ce qui reste dans la pile a la fin de l'interpretation
  printf("======= dictionnary ==============\n");
  //dictionary_print (ic->dic,f);
  printf("======== final stack =============\n");
  linked_list_chunk_print(ic->stack,stdout);
}


