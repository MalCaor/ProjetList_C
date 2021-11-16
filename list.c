#include "list.h"
#include <stdio.h>
#include <stdlib.h>

s_node * list_create(void)
{
    // creation d'une nouvelle liste vide
    s_node * snode_retour = (s_node *) malloc(sizeof(s_node));
    return snode_retour;
}
void * list_get_data(s_node * node)
{
    // lire la donnee d'un noeud
    if (node == NULL)
    {
        return EXIT_FAILURE;
    }
    if (node->data == NULL)
    {
        return EXIT_FAILURE;
    }
    
    return node->data;
}
void list_set_data(s_node * node, void * data)
{
    // ecrire la donnee d'un noeud
    if(node == NULL)
    {
        return EXIT_FAILURE;
    }
    node->data = data;
}
s_node * list_insert(s_node * head, void * data)
{
    // creation et insertion d'un noeud en tete de liste
    //   retourne la tete de liste
    s_node * node_tete = list_create();
    list_set_data(node_tete, data);
    node_tete->next = head;
    return node_tete; 
}
s_node * list_append(s_node * head, void * data)
{
    // creation et ajout d'un noeud en queue de liste
    //   retourne la tete de liste
    if (head == NULL)
    {
        return EXIT_FAILURE;
    }
    // c'est un node seul
    if (head->next == NULL)
    {
        s_node * new_node = list_create();
        list_set_data(new_node, data);
        head->next = new_node;
        return head;
    }
    // c'est une liste
    s_node * node = head;
    while (node->next != NULL)
    {
        node = node->next;
    }
    s_node * new_node = list_create();
    list_set_data(new_node, data);
    node->next = new_node;
    return head;
}
s_node * list_orderedAppend(s_node ** head, int (*compare)(s_node *, void *), void *param)
{
    // ajout d'un noeud dans une liste ordonnee
    // selon le resulat de "compare" ;
    // si la donnee existe deja, elle n'est pas ajoutee
    
    s_node * h = *(head);
    
    if(h == NULL){
        return EXIT_FAILURE;
    }

    if (h->next == NULL)
    {
        // il n'y en a qu'un seul
        if(list_get_data(h) == param){
            return h;
        }

        if(compare(h, param)){
            return list_insert(h, param);
        } else {
            s_node * new = list_create();
            list_set_data(new, param);
            new->next = h->next;
            h->next = new;
            return h;
        }
    }
    
    // il y en a plusieur
    s_node * list_parcour = h;
    s_node * pre = h;
    while (list_parcour->data != NULL)
	{
        if(list_get_data(list_parcour) == param){
            return h;
        }

        if(compare(list_parcour, param)){
            s_node * new = list_create();
            list_set_data(new, param);
            new->next = list_parcour;
            if (pre != list_parcour){
                pre->next = new;
                return h;
            } else {
                return new;
            }
        }
		
		if(list_parcour->next == NULL){
			s_node * new = list_create();
            list_set_data(new, param);
            list_parcour->next = new;
            return h;
		}
        pre = list_parcour;
		list_parcour = list_parcour->next;
	}
    
}       
s_node * list_remove(s_node * head, void * data)
{
    // suppression de la premiere instance d'une
    // donnee dans la liste, retourne la tete de liste
    
    // remove only one
    if(head->next == NULL && head->data == data){
        return list_create();
    }

    s_node * node = head;
    s_node * prev;
    while (node->data != NULL)
    {
        if (node->data == data)
        {
            prev->next = node->next;
            node = NULL;
            return head;
        }
        if(node->next == NULL){
			break;
		}

        prev = node;
        node = node->next;
    }
    return head;
}
s_node * list_headRemove(s_node * head)
{
    // suppression de la tete de liste
    // retourne la nouvelle tete de liste
    s_node * new_head = head->next;
    head = NULL;
    return new_head;
}
void list_destroy(s_node * head)
{        
    // destruction d'une liste
    //  (La liberation des donnees n'est pas prise en charge)

    // if only one
    if (head->next == NULL)
    {
        head->data = NULL;
        return;
    }
    

    s_node * node = head;
    s_node * prev;
    while (node->next != NULL)
    {
        prev = node;
        node = node->next;
        prev->data = NULL;
        prev->next = NULL;
    }
}