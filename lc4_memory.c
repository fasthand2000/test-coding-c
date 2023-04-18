/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"


/*
 * adds a new node to a linked list pointed to by head
 */
int add_to_list(row_of_memory** head, 
				short unsigned int address,
				short unsigned int contents) {

	if (*head == NULL){
		*head = (row_of_memory*)malloc(sizeof(row_of_memory));
		(*head)->address = address;
		(*head)->contents = contents;
		(*head)->assembly = NULL;
		(*head)->label = NULL;
		(*head)->next = NULL;
		return 0;
	}
	
	row_of_memory* new_node = (row_of_memory*)malloc(sizeof(row_of_memory));
	if (new_node == NULL){
		printf("Error allocating  memory for a new node.");
		return 1;
	}

	new_node->address = address;
	new_node->contents = contents;
	new_node->assembly = NULL;
	new_node->label = NULL;
	new_node->next = NULL;

	if((*head)->next == NULL){
		if ((*head)->address < new_node->address){
			(*head)->next = new_node;
			return 0;
		}
		if ((*head)->address > new_node->address){
			new_node->next = (*head);
			*head = new_node;
			return 0;
		}
    }
	/* Ordering the list by address */
	row_of_memory* current_node = (*head);
	while (current_node != NULL){
		if ((current_node->address < new_node->address) && (current_node->next == NULL || (new_node->address < current_node->next->address))){
			new_node->next = current_node->next;
			current_node->next = new_node;
			return 0;
		}
		current_node = current_node->next;
	}

	return 1;   
}

/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head, 
				      short unsigned int address ) {

	row_of_memory* current_node = head;
	while (current_node != NULL){
		if (current_node->address == address){
			/* return pointer to node in the list if item is found */
			return current_node;
		}
		current_node = current_node->next;
	}
	return NULL ;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode(row_of_memory* head, short unsigned int opcode){
    /* opcode parameter is in the least significant 4 bits of the short int and ranges from 0-15 */
	/* see assignment instructions for a detailed description */

	row_of_memory* search_node = head;
	while (search_node->next != NULL){
		if ((((search_node->contents) & (0b1111000000000000)) == (opcode<<12)) && (search_node->assembly == NULL)){
			return search_node;
		}
		search_node = search_node -> next;
	}
	return NULL ;
}

/*
 * print entire linked list
 */
void print_list (row_of_memory* head ) {
	/* make sure head isn't NULL */
	/* print out a header */
    /* don't print assembly directives for non opcode 1 instructions if you are doing extra credit */
	/* traverse linked list, print contents of each node */	
	
	row_of_memory* current_node = head;
	printf("%-25s %-14s %-15s %s\n","<label>","<address>","<contents>","<assembly>");
	while (current_node != NULL){
		printf("%-25s %04X%-11s %04X%-11s %s\n", current_node->label, current_node->address, "",current_node->contents, "", current_node->assembly );
		current_node = current_node->next;
	}

	return;
}

/*
 * delete entire linked list
 */
int delete_list(row_of_memory** head) {

    row_of_memory* current_node = *head;
    row_of_memory* next;

    while (current_node != NULL) {
        next = current_node->next;
        if (current_node->label != NULL) {
            free(current_node->label);
        }
        if (current_node->assembly != NULL) {
            free(current_node->assembly);
        }
        free(current_node);
        current_node = next;
    }

    *head = NULL;
    return 0;
}
