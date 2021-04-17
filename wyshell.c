#include"wyscanner.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
int main(int argc, char* argv[]) {
 char *tokens[]={ "QUOTE_ERROR", "ERROR_CHAR", "SYSTEM_ERROR",
                "EOL", "REDIR_OUT", "REDIR_IN", "APPEND_OUT",
                "REDIR_ERR", "APPEND_ERR", "REDIR_ERR_OUT", "SEMICOLON",
                "PIPE", "AMP", "WORD" };
 char msg[1024];
 char *rpt;
 int parseLineInt = 0;
 int first,end ,outputredir, inputredir, errorredir, amp,amploc, pipe, pipeloc, lastgo;

 
 while(1) {
  int i =0;
  first = 0;
  end = 0;
  outputredir = 0;
  inputredir = 0;
  errorredir = 0;
  amp = 0;
  amploc = 0;
  pipe = 0;
  pipeloc = 0;
  lastgo = 0;
  //end = 0;
  printf("$> ");
  rpt = fgets(msg,1024,stdin);
  //Error checking for user input
  if(rpt == NULL) {
   if(feof(stdin)) {
    return 0;
   }
    else {
     perror("fgets from stdin");
     return 1;
    }
   }
   //Parse the user input
   parseLineInt = parse_line(msg);
   
   while(parseLineInt !=  EOL && end != 1){ 
   i++;
      switch(parseLineInt) {
      	case APPEND_OUT:
      	//If there is no command before anything other then a ; give an error and stop reading
      	if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	  }
     	else {
          printf(" >> ");
          printf("\n");
          lastgo = 3;
        }
        break;
        case WORD:
           if(first == 0) {
            printf(":--: ");
            printf("%s", lexeme);
            printf("\n");
            first = 1;
            lastgo = 1;
          }
          else {
            printf(" --: ");
            printf("%s", lexeme);
            printf("\n");
            lastgo = 1;
          }
        break;      
        case PIPE:
          if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	  }
     	  else {
          printf(" |");
          printf("\n");
          pipe++;
          first = 0;
          lastgo = 2;
          pipeloc = i;
        }
        break;
        case SEMICOLON:
          printf(" ;");
          printf("\n");
          first = 0;
        break;
        case REDIR_IN:
          if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	  }  
     	  else if (inputredir != 1){
            printf(" <");
            printf("\n");
            inputredir = 1;
            lastgo = 3;
          }
          else {
            printf("Ambiguous input redirection \n");
            end = 1;
          }
        break;
        case REDIR_OUT:
        if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	}
     	else if(outputredir != 1){
          printf(" >");
          printf("\n");
          outputredir = 1;
          lastgo = 3;
        }
        else {
          printf("Ambiguous output redirection \n");
          end = 1;
        }
        break;
        case REDIR_ERR:
        if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	}
     	else if(errorredir != 1){
          printf(" 2>");
          printf("\n");
          errorredir = 1;
          lastgo = 3;
        }
        else {
          printf("Ambiguous error redirection \n");
          end = 1;
        }
        break;
        case APPEND_ERR:
        if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	}
     	else {
          printf(" 2>>");
          printf("\n");
          lastgo = 3;
        }
        break;
        case AMP:
        if(first == 0) {
   	    printf("No Command");
   	    printf("\n");
   	    break;
     	} 
     	else if (amp != 1){
          printf(" &");
          printf("\n");
          amp = 1;
          amploc = i;
        }
        else {
          printf("Only 1 background task \n");
          end = 1;
        }
        break;
        case SYSTEM_ERROR:
          perror("system error");
          return -1;
        break;
        case ERROR_CHAR:
          printf("%d: %s\t =%d\n",parseLineInt,tokens[parseLineInt%96],error_char);
        break;
        case QUOTE_ERROR:
          printf("%d: %s\t \n",parseLineInt,tokens[parseLineInt%96]);
        break;
        default:
          printf("%d: %s\n",parseLineInt,tokens[parseLineInt%96]); 
      }
      
      parseLineInt=parse_line(NULL);
    }
    //If the last thing in the string was a word its fine
    if(lastgo == 1) {
    
    }
    //If the last thing in the string was a | then return a semantic error
    else if(lastgo == 2) {
     printf("ERROR: Must have a command after | \n");
    }
    //If the last thing in the string was a redir then return a semantic error
    else if(lastgo == 3) {
     printf("ERROR: Must have a filename after a redir / append \n");
    }
    //If there was an & before the | return a semantic error
    if(amploc != 0 && pipeloc != 0 && amploc < pipeloc) {
     printf("ERROR: Background task must be the last task");
    }
    printf(" --: EOL");
    printf("\n");
 }
}
