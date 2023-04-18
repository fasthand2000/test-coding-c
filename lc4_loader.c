/************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"

/*
 * Comments here
 */
FILE* open_file(char* file_name) 
{
	FILE* src_file;
	src_file = fopen (file_name, "rb") ;
	return src_file;
}

/*
 * Comments here
 */
int parse_file(FILE* src_file, row_of_memory** memory) {
    unsigned char label[2];
    short unsigned int address;
    unsigned char n[2];
    int num_items;
    row_of_memory* head = *memory;
    while (1) {
        if (fread(label, 1, 2, src_file) < 2) {
            break;
        }

        /* Code header */
        if(label[0] == 0xCA && label[1] == 0xDE) {
            /* read in address */
            address = fgetc(src_file);
            address = (address<<8) + fgetc(src_file);
            /* read in number of instructions */
            fread(n, 1, 2, src_file);
            num_items = (n[0] << 8) + n[1];
            /* makes a new node for each instructions */
            int i;
            for(i = 0; i < num_items; i++) {
                short unsigned int instruction;
                instruction = fgetc(src_file);
                instruction = (instruction<<8) + fgetc(src_file);
                add_to_list(&head, address, instruction);
                address += 0x1;
            }
        }

        /* Data header */
        if(label[0] == 0xDA) {
            /* read in address */
            address = fgetc(src_file);
            address = (address<<8) + fgetc(src_file);
            /* read in number of instructions */
            fread(n, 1, 2, src_file);
            num_items = (n[0] << 8) + n[1];
            /* makes a new node for each instructions */
            int i;
            for(i = 0; i < num_items; i++) {
                short unsigned int instruction;
                instruction = fgetc(src_file);
                instruction = (instruction<<8) + fgetc(src_file);
                add_to_list(&head, address, instruction);
                address += 0x1;
            }
        }

        /* Symbol headers */
        if(label[0] == 0xC3) {
            /* read in address */
            address = fgetc(src_file);
            address = (address<<8) + fgetc(src_file);

            /* number of characters in the label read in as "num_items" */
            fread(n, 1, 2, src_file);
            num_items = (n[0] << 8) + n[1];

            /* create space for a char array of the size num_items and add 1 for the end sign */
            char * stuff = (char*)malloc(num_items + 1);

            /* read in num_items bytes, add NULL terminator */
            fread(stuff, 1, num_items, src_file);
            stuff[num_items] = '\0';

            /* create row_of_mem pointer to holder node returned from search, look for node with this address */
            row_of_memory * nodeToEdit = search_address(head, address);

            /* check for NULL, if there is not yet a node with that address in the list, create one */
            if(nodeToEdit == NULL) {
                add_to_list(&head, address, 0);
                nodeToEdit = search_address(head, address);
            }

            /* edit the label field of the node at the address */
            nodeToEdit->label = stuff;
        }
    }

    /* close file and check if closed properly */
    if(fclose(src_file) != 0) {
        fprintf(stderr, "Error occurred while closing file. Memory freed.");
        delete_list(memory);
        return 2;
    }
   
	*memory = head;
	return 0 ;
}
