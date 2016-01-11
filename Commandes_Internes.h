#ifndef COMINTERN_H
#define COMINTERN_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include "Shell.h"

static char* cmd_int[] = {
							"cd",
							"echo",
							"exit",
							"kill",
							"pwd",
							"date",
							"hostname"		
};

/**                          Conctantes                             **/

#define CD 0
#define ECHO 1
#define EXIT 2
#define KILL 3
#define PWD 4
#define DATE 5
#define HOSTNAME 6

/**                    Prototypes du commande interne                 **/

void my_cd(Expression *e);
void my_echo(Expression *e);
void my_exit();
void my__kill(Expression *e);
char* my_pwd();
void my_date();
void my_hostname();



void exec_cmd_int(int intCmd, Expression *e);
int isCmdInt(char* cmd);


#endif
