/*
NAME : K.MOHANAPRIYA
DATE : 03.02.2026
PROJECT NAME : MINISHELL
*/

#include<stdio.h>                                  //header file
#include<string.h>                                 //header file
#include <stdlib.h>                                // for system()
#include "main.h"                                  // for scan_input
char *shell_prompt;                                //gobel variable
int main()   
{
	system("clear");                           //clear the screen
	signal(SIGINT, signal_handler);            //signal for sigint
        signal(SIGTSTP, sigtstp_handler);          //signal for sigtstp
	char prompt[100] = "minishell$";           //string for  prompt
	shell_prompt = prompt;                     //copy the prompt to shell_prompt
	char inputstring[100];                     //its string store the input 
	scan_input(prompt,inputstring);            //call the function of the 
}
