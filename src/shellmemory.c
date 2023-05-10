#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <time.h>


int var_size = VAR_STORE_SIZE; 
int frame_size = FRAME_STORE_SIZE;
int TIME = 0;

struct memory_struct_var{
	char *var;
	char *value;
};
struct memory_struct_frame{
	int var;
	char *value;
	int accessTime;
};
struct memory_struct_var var_store[VAR_STORE_SIZE];
struct memory_struct_frame frame_store[FRAME_STORE_SIZE];


// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}


// Shell memory functions

void mem_init(){
	int i;
	for (i=0; i<VAR_STORE_SIZE; i++){		
		var_store[i].var = "none";
		var_store[i].value = "none";
	}

	i = 0;

	for (i=0; i<FRAME_STORE_SIZE; i++){		
		frame_store[i].var = -1;
		frame_store[i].value = "none";
		frame_store[i].accessTime = 0;

	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	int i;
	for (i=0; i<VAR_STORE_SIZE; i++){
		if (strcmp(var_store[i].var, var_in) == 0){
			var_store[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<VAR_STORE_SIZE; i++){
		if (strcmp(var_store[i].var, "none") == 0){
			var_store[i].var = strdup(var_in);
			var_store[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;
	for (i=0; i<VAR_STORE_SIZE; i++){
		if (strcmp(var_store[i].var, var_in) == 0){
			return strdup(var_store[i].value);
		} 
	}
	return NULL;

}

int mem_load_process(char *instruction, int pos, int pid){
	int i;
	TIME++;

	if (pos == -1){
		for (i=0; i<FRAME_STORE_SIZE; i = i + 3){
			
			if (frame_store[i].var == -1 && frame_store[i+1].var == -1 && frame_store[i+2].var == -1){
				frame_store[i].var = pid;
				frame_store[i].value = strdup(instruction);
				frame_store[i].accessTime = TIME;
				return i;
			}
		}
		return -1; //memory full
	}
	else{
		frame_store[pos].var = pid;
		frame_store[pos].value = strdup(instruction);
		frame_store[pos].accessTime = TIME;
	}
	return pos;

}

char *mem_get_pos(int pos){
	TIME++;
	frame_store[(pos/3)*3].accessTime = TIME;
	return frame_store[pos].value;


}

char *print_at_pos(int start, int length){
	for(int i = 0; i < length; i ++){
		printf("%s",frame_store[start + i].value);
	}
}
void mem_clear(int start, int length){
	for(int i = 0; i < length; i ++){
		frame_store[start + i].var = -1;
		frame_store[start + i].value = "none";
	}
}


int get_frame_pid(int pos){
	return frame_store[pos].var;
}

int get_LRU(){

	int earliest = 0;
	for(int i = 0; i < FRAME_STORE_SIZE; i = i + 3 ){

		if((frame_store[i].accessTime - frame_store[earliest].accessTime) < 0){
			//found an earlier time
			earliest = i;
		}

	}

	return earliest;

}