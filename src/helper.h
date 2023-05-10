#ifndef HELPER_H
#define HELPER_H

//bad commands:
int badcommand();
int badcommandTooManyTokens();
int badcommandFileDoesNotExist();
int badcommandMkdir();
int badcommandCd();
int badcommandSameFileName();


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

extern struct PCB readyQ[3];

int compare_len(const void *a, const void *b);
int compare_score(const void *a, const void *b);
struct PCB loadNextPage(struct PCB pcb);

#endif /* HELPER_H */