#include <stdio.h>
#include "shellmemory.h"
#include "shell.h"
#include <string.h>

//bad commands:
int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens");
	return 2;
}

int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int badcommandMkdir(){
	printf("%s\n", "Bad command: my_mkdir");
	return 4;
}

int badcommandCd(){
	printf("%s\n", "Bad command: my_cd");
	return 5;
}

int badcommandSameFileName() {
	printf("%s\n", "Bad command: same file name");
	return 5;
}

struct PCB{
	int process_id;
	int position;
	int length;
	int len_score;
	int current_instruction;
	int executed; //keeps track of number of instructions executed
    int done;
	int doneLoading;
	int pageNum;
	int pageTable[333];
	int currentPage;
	int endReadyQ; //put it end of the ready Q
	FILE *filePointer;
};

struct PCB readyQ[3];

//needed in SJF
int compare_len(const void *a, const void *b) {
    const struct PCB *x = (const struct PCB *)a;
    const struct PCB *y = (const struct PCB *)b;
    if (x->length < y->length) {
        return -1;
    } else if (x->length > y->length) {
        return 1;
    } else {
        return 0;
    }
}

//needed in AGING
int compare_score(const void *a, const void *b) {
    const struct PCB *x = (const struct PCB *)a;
    const struct PCB *y = (const struct PCB *)b;
    if (x->len_score < y->len_score) {
        return -1;
    } else if (x->len_score > y->len_score) {
        return 1;
    } else {
        return 0;
    }
}

//needed in exec 
struct PCB loadNextPage(struct PCB pcb){
	FILE *p = pcb.filePointer;
	int position;
	char line[1000];
	memset(line, 0, sizeof(line));
	int pid = pcb.process_id;

	//get first line
	if(fgets(line,999,p) != NULL) {
		//get position in mem where first instruction is stored
		position = mem_load_process(line, -1, pid); 
		
		//
		//memory full, need to remove a page 
		if (position == -1) {
   			int removePosition = get_LRU();

			//Remove frame number from pageTable of the process' frame we are replacing 
			for(int i = 0; i < 3;i++){
				if(readyQ[i].process_id == get_frame_pid(removePosition)){
					for(int j = 0; j < readyQ[i].pageNum; j++){
						if(readyQ[i].pageTable[j] == removePosition){
							readyQ[i].pageTable[j] = -1;
							break;
						}
					}
					break;
				}
			}

			//Overriding the frame in frame_store
			position = removePosition;
			//print page fault 
			printf("Page fault! Victim page contents:\n \n");
			print_at_pos(removePosition, 3);
			printf("\nEnd of victim page contents.\n");
			//put instruction in spot in mem
			mem_load_process(line, position, pid);
			
		}

		//found empty slot (either memory not full, or we're overriding a frame) 
		pcb.pageTable[pcb.pageNum] = position;
		pcb.pageNum = pcb.pageNum + 1;
	} 

	//end of file reached 
	else {
		pcb.doneLoading = 1;
		return pcb;
	}

	for(int i = 0; i < 2; i++){
		
		memset(line, 0, sizeof(line));
		
		if(fgets(line,999,p) != NULL) {
			//put next instruction in next spot in mem
			position++;
			mem_load_process(line, position, pid);
		} else {
			pcb.doneLoading = 1;
			break;
		}		
	}
	return pcb;
	
}