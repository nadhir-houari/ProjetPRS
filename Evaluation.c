#include "Evaluation.h"

int evaluer_expr(Expression *e,int in,int out,int err,int bg){
pid_t pid;
switch (e->type) 
{	 
	case BG :{ /**                  Background                    **/
				pid = fork();
			if(pid == 0){
				 evaluer_expr(e -> gauche,0,1,2,1);
				 exit(0);
				}
			else{
				  waitpid(pid,NULL,WNOHANG);
				}
			};
		break;
		
	case PIPE :{/**                  PIPE                    **/
		int tube[2];
		if(pipe(tube) == -1){
			perror("tube");
		}
		pid = fork();
		if(pid == -1){
			perror("fork");
		}
		else if(pid == 0){// Cas du fils
			 close(tube[0]); 
			 evaluer_expr(e -> gauche,0,tube[1],2,0);
		}
		else{ //Cas du pére 
			 close(tube[1]); 
			 evaluer_expr(e -> droite,tube[0],1,2,0);
			}
			
			};
		break;
	
	case SEQUENCE :{/**                  SEQUENCE                    **/					
			pid = fork();
			if(pid == 0){
				 evaluer_expr(e -> gauche,0,1,2,0);
				 exit(0);
				}
			else{
				  waitpid(pid,NULL,0);
				  evaluer_expr(e -> droite,0,1,2,0);
				  
				}
		};
		break;
	
	case SEQUENCE_ET : {/**                  SEQUENCE ET                    **/
			pid = fork();
			int status_sequence_et;
			if(pid == 0){
			 execvp(e -> gauche -> arguments[0], &e-> gauche-> arguments[0]);
			 perror(e -> gauche -> arguments[0]);
			 exit(1);
		}
		else{
			wait(&status_sequence_et);
			if(WEXITSTATUS(status_sequence_et) == 0){
				evaluer_expr(e -> droite,0,1,2,0);
			}
			}
		};
		break;
	
	case SEQUENCE_OU : {/**                  SEQUENCE OU                    **/
			pid = fork();
			if(pid == 0){
				 evaluer_expr(e -> gauche,0,1,2,0);
				 exit(0);
				}
			else{
				  waitpid(pid,NULL,0);
				  evaluer_expr(e -> droite,0,1,2,0);
				}
		};
		break;

	case REDIRECTION_I : {/**                  REDIRECTION INPUT                    **/
			int fdin = open(e->arguments[0], O_RDONLY, 0666);	
            if(fdin == -1){
				perror("open");
			}
			evaluer_expr(e->gauche,fdin,1,2,0);
			
		};
		break;	
		
	case REDIRECTION_O : {/**                  REDIRECTION OUTPUT                    **/
		
			int fdout = open(e->arguments[0],O_CREAT | O_TRUNC | O_WRONLY, 0666);	
            if(fdout == -1){
				perror("open");
			}
			evaluer_expr(e->gauche,0,fdout,2,0);
		};
		break;
		
	case REDIRECTION_A : {/**                  REDIRECTION OUTPUT APPEND                    **/
		int fdout = open(e->arguments[0],O_CREAT | O_APPEND | O_WRONLY, 0666);	
            if(fdout == -1){
				perror("open");
			}
			evaluer_expr(e->gauche,0,fdout,2,0);
		};
		break;	
		
	case REDIRECTION_E : {/**                  REDIRECTION STDERR                    **/
			int fderr = open(e->arguments[0],O_CREAT | O_TRUNC | O_WRONLY, 0666);	
            if(fderr == -1){
				perror("open");
		
			}
			evaluer_expr(e->gauche,0,1,fderr,0);	
		};
		break;	
	
	case REDIRECTION_EO : {/**                  REDIRECTION STDERR & OUTPUT                    **/
	    	int fdout = open(e->arguments[0],O_CREAT | O_WRONLY, 0666);
			int fderr = open(e->arguments[0],O_CREAT | O_TRUNC | O_WRONLY, 0666);	
            if((fderr == -1) ){
				perror("open");
			}
			 if((fdout == -1) ){
				perror("open");
			}
			evaluer_expr(e->gauche,0,fdout,fderr,0);
		};
		break;
	case VIDE :
			break;
	
		
	default : {/**                  COMMANDE SIMPLE & INTERNES                    **/
		
		int intCmd = isCmdInt(e->arguments[0]);
		if(intCmd != -1){
			exec_cmd_int(intCmd,e);
			break ; 
		}
		else {
			  pid_t pid ;
			  pid = fork();
			  if(pid == 0){ // fils
				
			  if(in != 0){
				dup2(in,0);
				close(in);
			  }
			  if(out != 1){
				 dup2(out,1);
				 close(out);
			  }
			  if(err!=2){
				  dup2(err,2);
				  close(err);
			  }
			 execvp(e->arguments[0], &e->arguments[0]);
			 perror(e->arguments[0]);
			 exit(-1);
			 
			 
			}else{ // pére
			 
			  if(in != 0){
				close(in);
				}
				 
			  if(out != 1){
				close(out);
			  }

			   if(err!=2){
				  close(err);
			  }
			  if(!bg){
					waitpid(pid, &status , 0);
					status = WIFEXITED(status)?WEXITSTATUS(status):128+WTERMSIG(status);
					return status;
					}
			  
			}
		  }
			
		};
		break;
	}
}


