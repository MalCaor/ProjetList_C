/* ---------------------------------------------------------
   Test unitaire du module liste
	fonctions utilitaires
   ---------------------------------------------------------
   20/10/21	SR	creation
   ---------------------------------------------------------
*/
#include <stdio.h>
#include <string.h>
#include "list.h"


/* taille maximum de la chaine representant les valeurs dans la liste */
#define LIST_AS_A_STRING_MAX_SIZE 1024
/* taille maximum de la chaine representant la valeur d'un noeud de la liste */
#define NODE_AS_A_STRING_MAX_SIZE 100

/* -----------------------------------------------------------------
   Conversion d'un noeud pointant sur une donnée de type int
	en chaine de caracteres
   -----------------------------------------------------------------
   node		: pointeur sur le noeud à convertir
   buffer (PS) 	: tampon contenant la chaine obtenue
   size		: taille maximum de la chaine
   -----------------------------------------------------------------
   retourne 1 si la chaine converti est trop longue pour le tampon
	    0 sinon
   -----------------------------------------------------------------
*/
int node_int_to_string(s_node * node, char *buffer, int size) {
	int * value;
	int str_length;

	value=(int *)list_get_data(node);
	str_length=snprintf(buffer, size, "%d", *value);
	return (str_length >= size);  // voir man snprintf (return value)
}


/* -----------------------------------------------------------------
   Conversion des données d'une liste en chaine de caracteres
	Les donnéees sont separees par ->
   -----------------------------------------------------------------
   head		 : pointeur sur la tete de liste
   node_to_string: fonction pour convertir la valeur d'un noeud
   -----------------------------------------------------------------
   retourne  la chaine de caractères representant la liste
   -----------------------------------------------------------------
*/
char *  list_to_string (s_node * head,  
		       int (*node_to_string)(s_node * node, char * buffer, int buffer_size))  {

	static char list_as_a_string[LIST_AS_A_STRING_MAX_SIZE];
	static char buffer[NODE_AS_A_STRING_MAX_SIZE];
	int length = 0;

	int max_length = LIST_AS_A_STRING_MAX_SIZE - 5;	
		// 5 : reserve pour la fin de chaine (null ou ...)

	while( head && length<max_length-1 ) {
		int append;
	 	(void)(*node_to_string)(head, buffer, NODE_AS_A_STRING_MAX_SIZE-1);	

		append = snprintf( list_as_a_string+length , 
			            max_length - length, "%s", buffer); 
		if ( append >= max_length-length ) length = max_length-1;
		else			           length += append;	

		append = snprintf( list_as_a_string+length , 
			            max_length - length, " -> "); 
		if ( append >= max_length-length ) length = max_length-1;
		else			           length += append;	

		head=head->next;
	}
	if (length==max_length-1)
		sprintf( list_as_a_string+length , " ...");  
			// incomplet, tampon trop petit
	else
		sprintf( list_as_a_string+length , "null"); 
		
	return list_as_a_string;
}

/* -----------------------------------------------------------------
   Compare la chaine representant une liste a une chaine attendu 
   -----------------------------------------------------------------
   head		 : pointeur sur la tete de liste
   node_to_string: fonction pour convertir la valeur d'un noeud
   waited	 : chaine de caractère attendue
   -----------------------------------------------------------------
   retourne 1 si le résultat recu est celui attendu
            0 sinon
   -----------------------------------------------------------------
*/
int check_list_state(s_node * head,  int (*node_to_string)(s_node * node, char * buffer, int size),
		 const char * waited) {
	char * obtained=list_to_string(head, node_to_string);
	if ( strcmp(waited, obtained) != 0 ) {
		fprintf(stderr, "Attendu : %s \n Obtenu : %s\n", waited, obtained);
		return 0;
	}
	return 1;
}

void affiche_list(s_node * list){
	// test parcour   
	printf("##### LIST #####\n");
	s_node * list_parcour = list;
	while (list_parcour->data != NULL)
	{
		printf("%s\n", list_get_data(list_parcour));
		if(list_parcour->next == NULL){
			break;
		}
		list_parcour = list_parcour->next;
	} 
	printf("################\n");
}

void affiche_list_nombre(s_node * list){
	// test parcour   
	printf("##### LIST #####\n");
	s_node * list_parcour = list;
	while (list_parcour->data != NULL)
	{
		printf("%i\n", list_get_data(list_parcour));
		if(list_parcour->next == NULL){
			break;
		}
		list_parcour = list_parcour->next;
	}
	printf("################\n");
}

int compare(s_node * node_a_comparer, void * param){
	return (list_get_data(node_a_comparer) > param);
}

int node_to_string(s_node * node, char * buffer, int buffer_size){
	printf("%s\n", (char *)list_get_data(node));
	return strncpy(buffer, (char *)list_get_data(node), buffer_size);
}


/* ##### MAIN ##### */

int main(int argc, char const *argv[])
{
	/* init */
	s_node * list1 = list_create();
	char c[] = "Bonjour";
	list_set_data(list1, c);

	// affichage just 1
	affiche_list(list1);

	// test append 
	list_append(list1, "comment");
	list_append(list1, "aller");
	list_append(list1, "vous?");
	//*/

	// affichage ajout
	affiche_list(list1);

	// test insert 
	list1 = list_insert(list1, "Malcaor :");
	
	// affichage insert
	affiche_list(list1);

	// test remove
	list1 = list_remove(list1, "aller");

	// affichage remove
	affiche_list(list1);

	// test remove only one
	s_node * list2 = list_create();
	list_set_data(list2, "test");
	list2 = list_remove(list2, "test");

	// affichage vide
	affiche_list(list2);

	// test remove head
	list1 = list_headRemove(list1);

	// affichage head remove
	affiche_list(list1);

	// test destruction 
	list_destroy(list1);

	// affichage liste detruite
	affiche_list(list1);
	
	printf("## LISTE COMP ##\n");
	
	printf("## SEUL NORMA ##\n");
	list_set_data(list1, 1);
	// affiche liste incomplete
	affiche_list_nombre(list1);
	// add order
	list1 = list_orderedAppend(&list1, compare, 2);
	// afficher now compl
	affiche_list_nombre(list1);
	// destroy again
	list_destroy(list1);

	printf("## SEUL INVER ##\n");
	list_set_data(list1, 2);
	// affiche liste incomplete
	affiche_list_nombre(list1);
	// add order
	list1 = list_orderedAppend(&list1, compare, 1);
	// afficher now compl
	affiche_list_nombre(list1);
	// destroy again
	list_destroy(list1);

	
	printf("### PLUSIEUR ###\n");
	list_set_data(list1, 2);
	list_append(list1, 3);
	list_append(list1, 5);
	list_append(list1, 6);
	list_append(list1, 7);
	// affiche liste incomplete
	affiche_list_nombre(list1);

	list1 = list_orderedAppend(&list1, compare, 4);
	
	affiche_list_nombre(list1);

	printf("### ADD END ###\n");

	list1 = list_orderedAppend(&list1, compare, 8);
	
	affiche_list_nombre(list1);

	printf("### ADD START ###\n");

	list1 = list_orderedAppend(&list1, compare, 1);
	
	affiche_list_nombre(list1);

	// TEST FOURNIE

	printf("### TEST LIS ###\n");

	/*
	// conversion char[] -> list
	printf("### TEST COV ###\n");
	char * ctestCOV[50];
	s_node * nodetestCOV = list_create();
	int * i = 1;
	list_set_data(nodetestCOV, i);
	affiche_list_nombre(nodetestCOV);
	node_int_to_string(nodetestCOV, ctestCOV, 50);
	//printf("c : %s\n", ctestCOV);
	*/

	
	// Conversion des données d'une liste en chaine de caracteres Les donnéees sont separees par ->
	printf("### TEST CLC ###\n");
	s_node * nodetestCLC = list_create();
	list_set_data(nodetestCLC, "Bonjour");
	list_append(nodetestCLC, "comment");
	list_append(nodetestCLC, "aller");
	list_append(nodetestCLC, "vous?");
	affiche_list(nodetestCLC);
	printf("%s\n", list_to_string(nodetestCLC, node_to_string));
	
	// Compare la chaine representant une liste a une chaine attendu 
	printf("### TEST CCA ###\n");
	s_node * nodetestCCA = list_create();
	list_set_data(nodetestCCA, "Bonjour");
	char stringAttendu[] = "Bonjour";
	//affiche_list(nodetestCCA);
	int resCCA = check_list_state(nodetestCCA, node_to_string, stringAttendu);

	printf("%s\n", list_get_data(nodetestCCA));
	
	return 0;
}
