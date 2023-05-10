void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
int mem_load_process(char *line, int pos, int pid);
char *mem_get_pos(int pos);
char *print_at_pos(int start, int length);
void mem_clear(int start, int length);
int get_frame_pid(int pos);
int get_LRU();