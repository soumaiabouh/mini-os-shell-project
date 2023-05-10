#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shellmemory.h"
#include "shell.h"
#include "helper.h"


int fcfs(int prog_size, int errcode) {
    //execute elements in q by order and clear memory in between
		for (int i = 0; i < prog_size; i++) {
			struct PCB toexec = readyQ[i];

			for (int i = 0; i < toexec.length; i++) {
				char* instr = mem_get_pos(toexec.current_instruction);

				int len = strlen(instr); // get the length of the string
				char arr[len + 1]; // allocate an array of chars
				memset(arr, 0, sizeof(char));
				// copy the string to the array
				for (int i = 0; i < len + 1; i++) {
					arr[i] = instr[i];
				}

				errcode = parseInput(arr);

				toexec.current_instruction++;
			}
			mem_clear(toexec.position, toexec.length);
		}
        return errcode;
}

int sjf(int prog_size, int errcode) {
    //sort readyQ by job length
    size_t n = sizeof(readyQ) / sizeof(readyQ[0]);
    qsort(readyQ, n, sizeof(readyQ[0]), compare_len);
    //execute elements in q by order and clear memory in between
    for (int i = 0; i < prog_size; i++) {
        struct PCB toexec = readyQ[i];

        for (int i = 0; i < toexec.length; i++) {
            char* instr = mem_get_pos(toexec.current_instruction);

            int len = strlen(instr); // get the length of the string
            char arr[len + 1]; // allocate an array of chars
            memset(arr, 0, sizeof(char));
            // copy the string to the array
            for (int i = 0; i < len + 1; i++) {
                arr[i] = instr[i];
            }

            errcode = parseInput(arr);

            toexec.current_instruction++;
        }
        mem_clear(toexec.position, toexec.length);
    }
    return errcode;
}

int round_robin(int time_slice, int prog_size, int errcode) {
    //instantiate a time slice, each process will run for n number of instructions
    int tslice = time_slice;

    //Need to loop through the readyQ, run each for tslice, then switch to the next
    //notDone variable -> run the while loop until all 3 processes are done running, in which case we set done to 0

    int notDone = 1;
    int doneProcesses = 0; //keep track of number of processes done 
    
    while (notDone) {
        if (doneProcesses == prog_size) {
            break;
        }

        for (int i = 0; i < prog_size; i++) { //&& (readyQ[i].done == 0)
            struct PCB toexec = readyQ[i];
            toexec.endReadyQ = 1;

            for (int j = 0; j < tslice; j++) { //each runs for 2 instructions

                if (toexec.endReadyQ == 0) {
                    break;
                }

                if (toexec.executed < toexec.length) { //run if not all instructions were executed

                //translate page address to abs address
                    if(toexec.pageTable[toexec.currentPage] < 0){
                        //load page assuming free memory in frame_store
                        toexec = loadNextPage(toexec);
                        toexec.endReadyQ = 0;
                        readyQ[i] = toexec;
                        break;
                        //load page when memory full -> call removePage 
                    }
                    int c = toexec.pageTable[toexec.currentPage] + toexec.current_instruction;
                    toexec.current_instruction++;
                    if(toexec.current_instruction>2){
                        toexec.currentPage++;
                        toexec.current_instruction = 0;
                    }
                    char* instr = mem_get_pos(c);

                    int len = strlen(instr); // get the length of the string
                    char arr[len + 1]; // allocate an array of charsn
                    memset(arr, 0, sizeof(char));

                    // copy the string to the array
                    for (int i = 0; i < len + 1; i++) {
                        arr[i] = instr[i];
                    }

                    errcode = parseInput(arr);
                    toexec.executed++;
                    readyQ[i] = toexec;
                }

                else {
                    //This should only run once for each process [p1->notDone, p2->Done, p3->notDone] doneProcesses = 1
                    if(toexec.done == 0) {
                        doneProcesses++; 
                        toexec.done = 1;
                        readyQ[i] = toexec;
                        fclose(toexec.filePointer);
                        }

                    break;
                }
            }
            
        }
    }
    return errcode;
}

int aging(int prog_size, int errcode) {
    
    for(int i = 0; i < prog_size; i++){
			readyQ[i].len_score = readyQ[i].length;
		}


		int doneProcesses = 0;
		while (1)
		{
			if (doneProcesses == prog_size) {
				break;
			}

			//sort readyQ by job length
			size_t n = sizeof(readyQ) / sizeof(readyQ[0]);
			qsort(readyQ, n, sizeof(readyQ[0]), compare_score);

			//execute one instr in head of Q
			struct PCB toexec = readyQ[0];
			if (toexec.done == 0){
			//run if not all instructions were executed
				char* instr = mem_get_pos(toexec.current_instruction);

				int len = strlen(instr); // get the length of the string
				char arr[len + 1]; // allocate an array of charsn
				memset(arr, 0, sizeof(char));

				// copy the string to the array
				for (int i = 0; i < len + 1; i++) {
					arr[i] = instr[i];
				}

				errcode = parseInput(arr);
				toexec.executed++;
				toexec.current_instruction++;
				readyQ[0] = toexec;

				if(toexec.done == 0 && toexec.executed == toexec.length) {
					doneProcesses++; 
					mem_clear(toexec.position, toexec.length);
					toexec.done = 1;
					toexec.len_score = __INT_MAX__;
					readyQ[0] = toexec;
					}
			
		
				//Age
				for(int i = 1; i < prog_size; i++){
				
					if (readyQ[i].len_score > 0 && readyQ[i].done == 0) {readyQ[i].len_score--;}
				}

			}
		
		}
    return errcode;
}