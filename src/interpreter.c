#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <time.h>
#include "shellmemory.h"
#include "shell.h"
#include "helper.h"
#include "policies.h"


int MAX_ARGS_SIZE = 7;
int lastProcessID = 0;

int help();
int quit();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int echo(char* var);
int my_ls();
int my_mkdir(char* dirname);
int my_touch(char* filename);
int my_cd(char* dirname);
int exec (char *progs[], int prog_size, char *policy);

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	if (args_size < 1){
		return badcommand();
	}

	if (args_size > MAX_ARGS_SIZE){
		return badcommandTooManyTokens();
	}

	for (int i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();	
		int total_len = 0;
		for(int i=2; i<args_size; i++){
			total_len+=strlen(command_args[i])+1;
		}
		char *value = (char*) calloc(1, total_len);
		char spaceChar = ' ';
		for(int i = 2; i < args_size; i++){
			strncat(value, command_args[i], strlen(command_args[i]));
			if(i < args_size-1){
				strncat(value, &spaceChar, 1);
			}
		}
		int errCode = set(command_args[1], value);
		free(value);
		return errCode;
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "echo")==0){
		if (args_size > 2) return badcommand();
		return echo(command_args[1]);

	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size > 1) return badcommand();
		return my_ls();
	
	} else if (strcmp(command_args[0], "my_mkdir")==0) {
		if (args_size > 2) return badcommand();
		return my_mkdir(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_touch")==0) {
		if (args_size > 2) return badcommand();
		return my_touch(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_cd")==0) {
		if (args_size > 2) return badcommand();
		return my_cd(command_args[1]);

	} else if (strcmp(command_args[0], "exec")==0) {
		if (args_size > 5) return badcommand();
		char *args[args_size - 2];
		for(int i = 1; i < args_size - 1; i ++){
			args[i-1] = command_args[i];
		}

    	return exec(args, args_size - 2, command_args[args_size - 1]);
	
	} else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	system("rm -rf backing_store");
	printf("%s\n", "Bye!");
	exit(0);
}

int set(char* var, char* value){
	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);
	mem_set_value(var, value);
	return 0;

}

int print(char* var){
	char *value = mem_get_value(var);
	if(value == NULL) value = "\n";
	printf("%s\n", value); 
	return 0;
}


int run(char* script){
	char *prog[1]={script};
	exec(prog, 1, "RR");
}


int echo(char* var){
	if(var[0] == '$') print(++var);
	else printf("%s\n", var); 
	return 0; 
}

int my_ls(){
	int errCode = system("ls | sort");
	return errCode;
}

int my_mkdir(char *dirname){
	char *dir = dirname;
	if(dirname[0] == '$'){
		char *value = mem_get_value(++dirname);
		if(value == NULL || strchr(value, ' ') != NULL){
			return badcommandMkdir();
		}
		dir = value;
	}
	int namelen = strlen(dir);
	char* command = (char*) calloc(1, 7+namelen); 
	strncat(command, "mkdir ", 7);
	strncat(command, dir, namelen);
	int errCode = system(command);
	free(command);
	return errCode;
}

int my_touch(char* filename){
	int namelen = strlen(filename);
	char* command = (char*) calloc(1, 7+namelen); 
	strncat(command, "touch ", 7);
	strncat(command, filename, namelen);
	int errCode = system(command);
	free(command);
	return errCode;
}

int my_cd(char* dirname){
	struct stat info;
	if(stat(dirname, &info) == 0 && S_ISDIR(info.st_mode)) {
		//the object with dirname must exist and is a directory
		int errCode = chdir(dirname);
		return errCode;
	}
	return badcommandCd();
}


int exec (char *progs[], int prog_size, char *policy){
	mem_init();//reset memory
	//STEP 1: Storing in memory:
	int errcode;
	int size = 0;
	//check if the files exist. If one doesn't, return from exec
	for(int i = 0; i < prog_size; i++){
		char *prog = progs[i];
		if (access(prog, F_OK) == -1) {
        	printf("Error: File does not exist\n");
        	return 1;
    	}
	}

	for(int i = 0; i < prog_size; i++){
		//create a pcb
		int processID = lastProcessID + 1;
		lastProcessID++;
		struct PCB newPCB;
		newPCB.process_id = processID;
		newPCB.length = 0;
		newPCB.len_score = 0;
		newPCB.current_instruction = 0;
		newPCB.executed = 0;
		newPCB.done = 0;
		newPCB.doneLoading = 0;
		newPCB.pageNum = 0;
		newPCB.currentPage = 0;
		newPCB.endReadyQ = 1;
		memset(newPCB.pageTable, -1, sizeof(newPCB.pageTable));

		//create the file 
		
		char command[1000];
		sprintf(command, "touch backing_store/%d.txt", newPCB.process_id);
		system(command); //creates the file in backing_store

		//calculate length + append in newly created file

		//from now on, we access the processes from the backing store
		char newfile[1000];
		sprintf(newfile, "backing_store/%d.txt", newPCB.process_id);
		
		//count one-liners as different commands
		int len = 0;
		char line[1000];
		FILE *cp = fopen(progs[i],"rt"); 
		FILE *pa = fopen(newfile,"a");  // accessing the program from the backing store
		if(cp == NULL) return badcommandFileDoesNotExist();

		
		char *token;
		while(1){
			memset(line, 0, sizeof(line));
			if(feof(cp)) break;
			fgets(line,999,cp); 
			token = strtok(line, ";"); 
			// Walk through other commands
			while (token != NULL) {
				len++;
				//append to file
				fseek(pa, 0, SEEK_END);
				int l = strlen(token);
    			if (token[l-1] == '\n') {
        			token[l-1] = '\0';
    			}
    			fprintf(pa, "%s\n", token);
				token = strtok(NULL, ";");
			}		
	}

	newPCB.length = len;
    fclose(cp);
	fclose(pa);

	FILE *p = fopen(newfile,"rt");
	newPCB.filePointer = p;
	//load first 2 pages  
	for (int j = 0; j < 2; j++) {
		if(newPCB.doneLoading == 1) break;
		newPCB = loadNextPage(newPCB);
	}

	//add pcb to q
	readyQ[i] = newPCB;
	}

	
	//STEP 2: Running the processes according to the policy
	if (strcmp(policy, "FCFS") == 0) {
		errcode = fcfs(prog_size, errcode);
	}

	if (strcmp(policy, "SJF") == 0) {
		errcode = sjf(prog_size, errcode);
	}

	if (strcmp(policy, "RR") == 0) {
		errcode = round_robin(2, prog_size, errcode);
	}

	if (strcmp(policy, "RR30") == 0) {
		errcode = round_robin(30, prog_size, errcode);
	}

	if (strcmp(policy, "AGING") == 0){
		errcode = aging(prog_size, errcode);
	}
}