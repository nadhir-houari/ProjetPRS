#ifndef _EVALUATION_H
#define _EVALUATION_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Shell.h"
#include "Commandes_Internes.h"

int evaluer_expr(Expression *e,int in,int out,int err,int bg);

#endif
