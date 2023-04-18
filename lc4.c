/************************************************************************/
/* File Name : lc4.c 													*/
/* Purpose   : This file contains the main() for this project			*/
/*             main() will call the loader and disassembler functions	*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"

/* program to mimic pennsim loader and disassemble object files */

int main (int argc, char** argv) {

/* leave plenty of room for the filename */

	char filename[100];

	/**
	 * main() holds the linked list &
	 * only calls functions in other files
	 */

	/* step 1: create head pointer to linked list: memory 	*/
	/* do not change this line - there should no be malloc calls in main() */
	
	row_of_memory* memory = NULL ;
	
	// check if argc has arguments, if it does print "filename" to the argument
	if  (argc > 1){
		sprintf(filename, "%s", argv[1]);
	}
	// if no arguments, print error statement
	else{
		fprintf(stderr, "error1: usage: ./lc4 <object_file.obj> \n");
		return(1); // for errors

	}

	/* step 2: determine filename, then open it		*/
	/*   TODO: extract filename from argv, pass it to open_file() */

	FILE *src_file = open_file(filename);
	if (src_file == NULL){
		fprintf(stderr, "Error opening the file.") ;
		return 1;
	}
	
	/* step 3: call function: parse_file() in lc4_loader.c 	*/
	/*   TODO: call function & check for errors		*/

	if (parse_file(src_file, &memory) != 0){
		fprintf(stderr, "Error while parsing the file.");
		return 1;
	}

	/* step 4: call function: reverse_assemble() in lc4_disassembler.c */
	/*   TODO: call function & check for errors		*/

	int reverse_return_value = reverse_assemble(memory);
	if (reverse_return_value != 0){
		printf("Error reverse assembling the linked list." );
		return 1;
	}

	/* step 5: call function: print_list() in lc4_memory.c 	*/
	/*   TODO: call function 				*/

	print_list(memory);

	/* step 6: call function: delete_list() in lc4_memory.c */
	/*   TODO: call function & check for errors		*/

	int delete_return_value = delete_list(&memory);
	if(delete_return_value){
		printf("Error deleting the linked list.");
		return 1;
	}

	/* only return 0 if everything works properly */
	return 0 ;
}
