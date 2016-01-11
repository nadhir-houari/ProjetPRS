#include "Commandes_Internes.h"
#include "Shell.h"

void my_echo(Expression *e ){	
	char ** string = &e-> arguments[1];
	if (string != NULL){
		while (*string != NULL){
			printf("%s ", *string);
			string++;
		}
	}
	printf("\n");
}

void my_hostname(){
	char hostname[255];
	hostname[254] = '\0';
	gethostname(hostname, 254);
	printf("%s\n", hostname);
	
}
void my_exit(){
	exit(EXIT_SUCCESS);
}

void my_kill(Expression *e){
	int pid = atoi(e->arguments[1]);
	int sig = atoi(e->arguments[2]);
	if (kill(pid,sig) == -1 ){
		perror("kill");
		//exit(-1);
	}
}

void my_date(){
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	printf("%s", asctime(tm));
}

void my_cd(Expression *e){
	if(e->arguments[1] != NULL){
		if(chdir(e->arguments[1])==-1)	
		perror("cd");
	}else{
		chdir(getenv("HOME"));
	}
	  
}

char* my_pwd(){
	return getcwd(NULL, pathconf(".", _PC_PATH_MAX));
}

void exec_cmd_int(int intCmd, Expression *e){
	switch(intCmd){

		case CD:
		my_cd(e);
		break;
		case EXIT:
		my_exit();
		break;
		case ECHO:
		my_echo(e);
		break;
		case KILL:
		my_kill(e);
		break;
		case PWD:
		printf("%s\n", my_pwd());
		break;
		case DATE:
		my_date();
		break;
		case HOSTNAME : 
			my_hostname();
		break;
		default:
		break;
	}
}
int isCmdInt(char* cmd){
	//cmd_int -> tableau contien la list de notre commande interne
	for(int i =0;i<7;i++){
		if (strcmp(cmd, cmd_int[i]) == 0)
			return i;
			}

	return -1;
}

