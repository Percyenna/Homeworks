#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#define INPUT_STRING_SIZE 80
#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"
int cmd_quit(tok_t arg[]) {
	printf("Bye\n");
	exit(0);
	return 1;
	}
int cmd_cd(tok_t arg[]);
int cmd_help(tok_t arg[]);
/* Command Lookup table */

typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
cmd_fun_t *fun;
char *cmd;
char *doc;
} fun_desc_t;
	fun_desc_t cmd_table[] = {
	{cmd_help, "?", "show this help menu"},
	{cmd_quit, "quit", "quit the command shell"},
	{cmd_cd, "cd", "change directory"},
};

int cmd_help(tok_t arg[]) {
	int i;
	for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
	printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
	}
    return 1;
}
int cmd_cd(tok_t arg[]) {
char *dir = arg[0];
if (dir ==NULL || strcmp(dir,"~")==0){
	dir ="/";
}
	if ( chdir(dir) < 0){
	perror("Error");
}
return 1;
}
int lookup(char cmd[]) {
	int i;
	for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
	if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
	}
	return -1;
	}
void init_shell()
{
char cwd[1024];
	/* Check if we are running interactively */
	shell_terminal = STDIN_FILENO;
	/** Note that we cannot take control of the terminal if the shell
	is not interactive */
	shell_is_interactive = isatty(shell_terminal);
	if(shell_is_interactive){
	/* force into foreground */
	while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
	kill( - shell_pgid, SIGTTIN);
	shell_pgid = getpid();
	/* Put shell in its own process group */
	if(setpgid(shell_pgid, shell_pgid) < 0){
	perror("Couldn't put the shell in its own process group");
	exit(1);
	}
	/* Take control of the terminal */
	tcsetpgrp(shell_terminal, shell_pgid);
	tcgetattr(shell_terminal, &shell_tmodes);
	}
	if(getcwd(cwd,sizeof(cwd))!= NULL){
	fprintf(stdout,cwd);
	printf("\n");
	}
	}
	/**
	* Add a process to our process list
	*/
	void add_process(process* p)
	{
	/** YOUR CODE HERE */
	}
	/**
	* Creates a process given the inputString from stdin
	*/
	process* create_process(char* inputString)
	{
	/** YOUR CODE HERE */
	return NULL;
}
char* concat(char *s1, char *s2)
{
char *result = malloc(strlen(s1)+strlen(s2)+1);
strcpy(result,s1);
strcat(result,s2);
return result;
}
void path(tok_t *t){
char *paths = getenv("PATH"), *dope;
tok_t *pathes2 =getToks(paths);
int i;
for(i=0;i<MAXTOKS && pathes2[i];i++){
dope = concat("/",t[0]);
dope = concat(pathes2[i],dope);
if(access(dope,F_OK)!=-1){
execve(dope,t,NULL);
//exit(0);
} }
execv(*t,t);
perror("Error: ");
exit(0);
}


void envirowrite(tok_t *t, char *filename ,char * c){
int filedesc;

if(c ==">"){
if((filedesc =open(filename, O_CREAT|O_WRONLY|O_TRUNC,0644))<0){
perror("Error ");
exit(1);
}
dup2(filedesc,1);
close(filedesc);
}

path(t);

}


void enviroread(tok_t *t, char *filename,char * c){
int filedesc;

 if(c=="<"){
if((filedesc =open(filename, O_RDONLY,0644))<0){
perror("Error: ");
exit(1);
}
dup2(filedesc,0);
close(filedesc);
}
path(t);

}

int shell (int argc, char *argv[]) {
char cwd[1024];
char *s = malloc(INPUT_STRING_SIZE+1); /* user input string */
tok_t *t; /* tokens parsed from input */
int lineNum = 0;
int fundex = -1;
pid_t pid = getpid(); /* get current processes PID */
pid_t ppid = getppid(); /* get parents PID */
pid_t cpid, tcpid, cpgid;
init_shell();
if(getcwd(cwd,sizeof(cwd))!= NULL)
printf("%s running as PID %d under %d\n",argv[0],pid,ppid);
lineNum=0;
fprintf(stdout, "%d ", lineNum++);
printf(cwd);
printf(":");
while ((s = freadln(stdin))){
t = getToks(s); /* break the line into tokens */
fundex = lookup(t[0]); /* Is first token a shell literal */
if(fundex >= 0) cmd_table[fundex].fun(&t[1]);
else {
pid_t cpid = fork();
pid_t mypid;
if( cpid > 0 ) { // parent process
mypid = getpid();
int status;
pid_t tcpid = wait(&status);
}
else if( cpid == 0 ){ // child process

char *read =strstr(s,"<");
char *write =strstr(s,">");

char *rchar = "<";
char *wchar = ">";

for(int i=0;i < MAXTOKS && t[i];i++){

if(strcmp(t[i],">")==0){
t[i]=NULL;
envirowrite(t,t[i+1],">");
}

if(strcmp(t[i],rchar)==0){
t[i]=NULL;
enviroread(t,t[i+1],"<");
}

}
path(t);
}
else {
perror( "Fork failed" );
exit( EXIT_FAILURE );
}
wait(NULL);
}
if(getcwd(cwd,sizeof(cwd))!= NULL)
fprintf(stdout, "%d ", lineNum++);
fprintf(stdout,cwd);
printf(":");
}
return 0;
}

