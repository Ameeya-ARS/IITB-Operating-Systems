#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER
/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line) {

    // tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
	for(i=0; i < strlen(line); i++){

		char readChar = line[i];

        // tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
				tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else {
			token[tokenIndex++] = readChar;
		}
	}
	
	free(token);
	tokens[tokenNo] = NULL ;
	return tokens;
}

void func1(char **tokens){
	int pid = fork();
	if(pid==-1){
		printf("Error while forking a child\n");
		return;
	}
	else if(pid==0){
		execvp(tokens[0],tokens);
		return;
	}
	else{
		wait(NULL);
		return;
	}
}

void func2(char **tokens){
	int i=0;
	while(tokens[i]!=NULL){i++;}
	if(i!=2){
		printf("Error! This command should be used with only 1 argument\n");
		return;
	}
	int pid = fork();
	if(pid==-1){
		printf("Error while forking a child\n");
		return;
	}
	else if(pid==0){
		if(chdir(tokens[1])==-1){
			printf("Error!! Could not execute the command\n");
			return;
		}
		return;
	}
	else{
		wait(NULL);
		return;
	}
}

// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK

int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;           

	// TODO: YOUR CODE HERE
	// INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE  
	// INITIALIZE OTHER VARIABLES AS NEEDED

	while(1) {	

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		// You can maintain boolean variables to check which type of command is given by user and then
        // conditionally execute depending on them

        // TODO: YOUR CODE HERE
		char *temp = "exit";
		if(strcmp(tokens[0],temp)==0){
			printf("Goodbye\n");
			exit(0);
		}

        temp = "cd";
		if(strcmp(tokens[0],temp)==0){
			func2(tokens);
		}
		else{
			func1(tokens);
		}
        // TODO: YOUR CODE HERE
    
        // freeing the memory
		for(int i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}

		free(tokens);

	}
	return 0;
}


