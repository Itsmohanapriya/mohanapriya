#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND      3

#define ROW  160
#define COL   50

extern char external_commands[ROW][COL];
extern int ext_cmd_count;
extern char *builtins[];
extern int status;
extern pid_t child_pid;
extern char *shell_prompt;

typedef struct job
{
	int job_id;
	pid_t pid;
	char command[100];
	struct job *next;
}job_t;
extern  job_t *job_head;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char external_commands[][COL]);
void add_job(pid_t pid, char *cmd);
void sigtstp_handler(int sig);
void do_fg();
void do_bg();
void check_background_jobs();
void delete_last_job();
job_t* find_last_job();
#endif

