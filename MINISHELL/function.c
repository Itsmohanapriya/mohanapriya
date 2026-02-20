#include<unistd.h>          // for fork, exec, pipe, dup2
#include<string.h>          // for string operations
#include<fcntl.h>          // for open
#include<stdio.h>          // for printf, scanf
#include"main.h"           // user defined header

// Stores external commands from external.txt
char external_commands[ROW][COL];

// Count of external commands
int ext_cmd_count = 0;

// Stores last foreground process status
int status = 0;

// PID of current running foreground process
pid_t child_pid = -1;

// Head of job linked list
job_t *job_head = NULL;

// Job numbering counter
int job_count = 0;

// Main shell loop to read input and execute commands
void scan_input(char*prompt,char*inputstring)
{
	// Load external commands into array
	extract_external_commands(external_commands);
	while(1)
	{
		// Check background jobs completed
		check_background_jobs();
		// Print shell prompt
		printf(ANSI_COLOR_YELLOW"%s\n"ANSI_COLOR_RESET,prompt);
		// Read input from user
		scanf("%[^\n]",inputstring);
		//clear newline
		getchar();
		// Change shell prompt using PS1=
		if((strncmp(inputstring,"PS1=",4)) == 0)
		{
			if(inputstring[4] == ' ')
			{
				printf("Invaild prompt\n");
			}
			else
			{
				strcpy(prompt,inputstring+4);
			}
			
		continue;
		}
		// Get command from input and check type
		else
		{
			char *cmd = get_command(inputstring);
			int ret = check_command_type(cmd);
			// If builtin command → execute directly
			if(ret == BUILTIN)
				 execute_internal_commands(inputstring);
			// ELSE If external command → create child using fork
			else if(ret ==  EXTERNAL)
			{
			        child_pid = fork();
				// CHILD: restore default signals and execute
				if(child_pid == 0)
				{
					signal(SIGINT,SIG_DFL);
					signal(SIGTSTP, SIG_DFL);
					execute_external_commands(inputstring);
					exit(0);
				}
				// PARENT: wait and check if stopped using Ctrl+Z
				else if(child_pid > 0)
				{
					waitpid(child_pid,&status,WUNTRACED);
					if (WIFSTOPPED(status))
					{
                                         add_job(child_pid, inputstring);
                                         printf("\n[%d]+ Stopped\t%s\n", job_count, inputstring);
                                        }
					child_pid = -1;
				}
			}
			else
			printf("command is not found\n");
		}
	}
}
// Handles SIGINT (Ctrl+C)
void signal_handler(int sig_num)
{
     // If child running → send SIGINT to child
    // Else → print prompt again
    if(sig_num == SIGINT)
    {
        if(child_pid > 0)
        {
            kill(child_pid, SIGINT);   // kill running child
        }
        else
        {
	   printf(ANSI_COLOR_YELLOW"\n%s\n"ANSI_COLOR_RESET,shell_prompt);
            fflush(stdout);
        }
    }
}
// Handles SIGTSTP (Ctrl+Z)
void sigtstp_handler(int sig)
{
    // Stops foreground child process
    if (child_pid > 0)
        kill(child_pid, SIGTSTP);
}
// Extract first word from input string (command)
char *get_command(char*inputstring)
{
	static char str[10];
	int i;
	for(i=0;inputstring[i] != ' ' && inputstring[i] != '\0';i++)
	{
		if(inputstring[i] != ' ')
		{
			str[i] = inputstring[i];
		}
	}
	str[i] ='\0';
	return str;
}
// Checks whether command is BUILTIN, EXTERNAL or invalid
int check_command_type(char* cmd)
{
	for(int i=0;builtins[i] != NULL;i++)
	{
		if(strcmp(cmd,builtins[i]) == 0)
		return BUILTIN;
	}
	for(int i=0;i < ext_cmd_count;i++)
	{ 
		if(strcmp(cmd,external_commands[i]) == 0)
			return EXTERNAL;
	}
	return NO_COMMAND;
}
// Reads external.txt and stores commands into 2D array
void extract_external_commands(char external_commands[][COL])
{
	int fd = open("external.txt",O_RDONLY);
	if(fd < 0)
	{
		perror("open");
	}
	char ch;
	char buffer[COL];
	int buf_index = 0;
	while(read(fd,&ch,1) > 0)
	{
		if(ch == '\n')
		{
			buffer[buf_index] = '\0';
			strcpy(external_commands[ext_cmd_count],buffer);
			ext_cmd_count++;
			buf_index = 0;
			if(ext_cmd_count >= ROW)
				break;
		}
		else
		{
			if(buf_index < COL-1)
				buffer[buf_index++] = ch;
		}

	}
	close(fd);
}
// Executes builtin commands
void execute_internal_commands(char *input_string)
{
	if(strcmp(input_string,"pwd") == 0)
	{
		char buffer[100];
		int size = sizeof(buffer);
		getcwd(buffer,size);
		printf("%s\n",buffer);
	}
	else if(strncmp(input_string,"cd ",3) == 0)
	{
		char path[30];
		int j=0;
		for(int i=3;input_string[i] != '\0';i++)
		{
			path[j++] =input_string[i];
		}
		path[j] = '\0';
		chdir(path);
	}
	else if(strcmp(input_string,"exit")== 0)
	{
		exit(0);
	}
	else if(strncmp(input_string,"echo",4)== 0)
	{
		if(strcmp(input_string,"echo $$")== 0)
			printf("%d\n",getpid());
		else if(strcmp(input_string,"echo $?") == 0)
		{
			if(WIFEXITED(status))
				printf("%d\n",(WEXITSTATUS(status)));
			else
				printf("0\n");
		}
		else if(strcmp(input_string,"echo $SHELL")== 0)
		{
			char *shell = getenv("SHELL");
			printf("%s\n",shell);
		}

	}
	 else if (strcmp(input_string, "jobs") == 0)
        {
                job_t *temp = job_head;
                while (temp)
                {
                   printf("[%d]  Running   %s [%d]\n",
                   temp->job_id,
                   temp->command,
                   temp->pid);
                   temp = temp->next;
                }
       }
	 else if (strcmp(input_string, "fg") == 0)
	 {
		 do_fg();
	 }
	else if (strcmp(input_string, "bg") == 0)
	{
		do_bg();

	}


}
// Splits input into arguments for execvp
void execute_external_commands(char *input_string)
{
         char  argv[40][30];
	char *args[40];
	int row =0, col =0;
	for(int i=0;input_string[i] != '\0';i++)
	{
		if(input_string[i] == ' ')
		{
			argv[row][col] = '\0';
			row++;
			col = 0;
		}
		else
		{
			argv[row][col++] = input_string[i];
		}
	}
	argv[row][col] = '\0';
	row++;
	for(int i=0;i < row;i++)
	{
		args[i] = argv[i];
	}
	args[row] = NULL;
	// Count number of pipes in command
	int pipe_count = 0;
	for(int i=0;i < row;i++)
	{
		if(strcmp(argv[i],"|")==0)
			pipe_count++;
	}
	int fd[2];
        int in_fd = 0;
        int start = 0;
	// If no pipe → directly execvp
	if(pipe_count == 0)
	{
		execvp(args[0],args);
		perror("execvp");
		exit(1);
	}
	// If pipe present → create multiple processes and connect with pipe()
	else if(pipe_count > 0)
	{
		for(int i =0;i<= pipe_count;i++)
		{
			pipe(fd);
			int k =0;
			for(int j = start;j<row;j++)
			{
				if(strcmp(argv[j],"|")==0)
					break;
				args[k++] = argv[j];
			}
			args[k] = NULL;
			int pid = fork();
			if(pid == 0)
			{
				dup2(in_fd,0);
			        if(i < pipe_count)
				   dup2(fd[1],1);
			        close(fd[0]);
			        execvp(args[0],args);
			        perror("execvp");
			        exit(1);
			}
			else
			{
				wait(NULL);
				close(fd[1]);
				in_fd = fd[0];
			}

			start += k+1;
		}
	}
       

}
// Add stopped process into linked list as a job
void add_job(pid_t pid, char *cmd)
{
    // Stores job id, pid and command
    job_t *new = malloc(sizeof(job_t));
    new->job_id = ++job_count;
    new->pid = pid;
    strcpy(new->command, cmd);
    new->next = NULL;

    if (job_head == NULL)
    {
        job_head = new;
    }
    else
    {
        job_t *temp = job_head;
        while (temp->next)
            temp = temp->next;
        temp->next = new;
    }
}
// Returns the last added job from linked list
job_t* find_last_job()
{
	if(job_head == NULL)
		return NULL;
	job_t *temp = job_head;
	while (temp -> next)
		temp = temp->next;
	return temp;
}
// Bring last stopped job to foreground
void do_fg()
{
      // Continue the job using SIGCONT
     // Wait for it and delete from list if finished
    job_t *job = find_last_job();
    if (job == NULL)
    {
        printf("fg: no current job\n");
        return;
    }

    child_pid = job->pid;

    kill(job->pid, SIGCONT);
    waitpid(job->pid, &status, WUNTRACED);

    if (WIFEXITED(status) || WIFSIGNALED(status))
    {
        delete_last_job();   //REMOVE FROM LIST
    }

    child_pid = -1;
}
// Checks background jobs using waitpid with WNOHANG
void check_background_jobs()
{
    // If finished → print Done and remove from list
    job_t *temp = job_head;
    job_t *prev = NULL;

    while (temp)
    {
        int ret = waitpid(temp->pid, &status, WNOHANG);

        if (ret > 0)   // process finished
        {
            printf("\n[%d]+ Done\t%s\n", temp->job_id, temp->command);

            if (prev == NULL)
                job_head = temp->next;
            else
                prev->next = temp->next;

            job_t *del = temp;
            temp = temp->next;
            free(del);
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}
// Continue last stopped job in background using SIGCONT
void do_bg()
{
	job_t *job = find_last_job();
	if(job == NULL)
	{
		printf("bg: no currect job\n");
		return;
	}
	kill(job->pid,SIGCONT);
	printf("[%d] %d running in background\n",job->job_id,job->pid);

}
// Deletes last job from linked list
void delete_last_job()
{
    if (job_head == NULL)
        return;

    job_t *temp = job_head;
    job_t *prev = NULL;

    while (temp->next)
    {
        prev = temp;
        temp = temp->next;
    }

    if (prev == NULL)
        job_head = NULL;
    else
        prev->next = NULL;

    free(temp);
}

