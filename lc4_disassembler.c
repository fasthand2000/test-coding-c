/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"


int reverse_assemble (row_of_memory* memory) {
  /* binary constants should be proceeded by a 0b as in 0b011 for decimal 3 */

	row_of_memory* head = memory;
	while(head != NULL){
		row_of_memory *returned_node = search_opcode(head, 0001);
		if (returned_node == NULL){
			break;
		}
		char assemblyInfo[50];
		short unsigned int return_register = (returned_node->contents & 0b0000111000000000)>>9;
		short unsigned int rs_register = (returned_node->contents & 0b0000000111000000)>>6;
		short unsigned int operation_type = returned_node->contents & 0b0000000000111000;
		short unsigned int rt_register = returned_node->contents & 0b0000000000000111;
		short unsigned int potential_Imm = returned_node->contents & 0b0000000000011111;

		switch (operation_type) {
			case 0b0000000000000000:
				returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "ADD R%d, R%d, R%d", return_register, rs_register, rt_register)+1);
				strcpy(returned_node->assembly, assemblyInfo);
				break;
			case 0b0000000000001000:
				returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "MUL R%d, R%d, R%d", return_register, rs_register, rt_register)+1);
				strcpy(returned_node->assembly, assemblyInfo);
				break;
			case 0b0000000000010000:
				returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "SUB R%d, R%d, R%d", return_register, rs_register, rt_register)+1);
				strcpy(returned_node->assembly, assemblyInfo);
				break;
			case 0b0000000000011000:
				returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "DIV R%d, R%d, R%d", return_register, rs_register, rt_register)+1);
				strcpy(returned_node->assembly, assemblyInfo);
				break;
			default:
				if (potential_Imm <= 15){
					returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "ADD R%d, R%d, %d", return_register, rs_register, potential_Imm)+1);
					strcpy(returned_node->assembly, assemblyInfo);
					break;
				}
				else{
					potential_Imm = 16 - (potential_Imm & 0b0000000000001111);
					returned_node->assembly = (char*)malloc(sprintf(assemblyInfo, "ADD R%d, R%d, -%d", return_register, rs_register, potential_Imm)+1);
					strcpy(returned_node->assembly, assemblyInfo);
					break;
				}
		}
		head = head->next;
	}	
	return 0 ;
}
