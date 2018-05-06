#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define NUM_THREADS 100

pthread_t threads[NUM_THREADS];
long t;
int rc;
int i;
void *sysexc(void * command ){

  char *p=NULL;

  p = (char *)command;
  if(strcmp(command," ")!=0){
  printf("command => %s\n", p);
}
	system(p);
	pthread_exit(NULL);
}

char **split(char *line){

	int size=64;
	char **parts=malloc(size*sizeof(char*));
	char *part=NULL;
  i=0;

	part=malloc(size*sizeof(char));
	part=strtok(line,"|");

	while(part!=NULL){

		parts[i++]=part;

		if(i>=size){
			size+=size;
			parts=realloc(parts,size*sizeof	(char*));
		}

		part=malloc(size*sizeof(char));
		part=strtok(NULL,"|");

	}
	parts[i]=NULL;

	return parts;
}

int exc(char** parts){
	int size=64;
	int ret=0;
	char *prc=NULL;
  t=0;
	for(int j=0;j<i;j++){

		if(strstr(parts[j],";")==NULL && strcmp(parts[j],"quit")!=0){

			rc=pthread_create(&threads[t],NULL,sysexc,(void*)parts[j]);
      t++;

		}

		else{

			prc=(char*)malloc(size*sizeof(char));
			prc=strtok(parts[j],";");

      if(prc==NULL){
        printf("\n");
      }
      else{
        if(strstr(prc,"quit")==NULL){
          printf("Starting thread %ld\n", t);
  				rc=pthread_create(&threads[t],NULL,sysexc,(void*)prc);

          t++;
  			}
  			else{
  				ret=1;
  			}
      }
			while(prc!=NULL){

				prc=(char*)malloc(size*sizeof(char));
				prc=strtok(NULL,";");//only reason of this rewrite -.-
        if(prc==NULL){
          printf("\n");
        }
        else{
          if(strstr(prc,"quit")==NULL){
            printf("Starting thread %ld\n", t);
          	rc=pthread_create(&threads[t],NULL,sysexc,(void*)prc);

            t++;
          }
          else{
    				ret=1;
    			}
        }
			}
		}
	}
  for (int a = 0; a < t; a++){
       pthread_join(threads[a], NULL);
  }
  //TODO THREAD JOİN
	return ret;
}

void interactive(){

  char input[50];
  char command[50];
  char *inputptr=NULL;
  char **sline=NULL;
  int i=0;
  inputptr = input;
  int check=0;

  do{
  	printf("myshell:>");
  	gets(inputptr);
  	if(inputptr!=""){
  		sline=split(inputptr);
			check=exc(sline);
	}
	//printf("did I come here\n");
}while(check==0);

  exit(0);

}
void batchfile(char filename[50]){
  FILE *fptr;
  char inputline[200];
  int check=0;;
  char **sline=NULL;
  char* inputptr;

  fptr = fopen(filename, "r");
  if (fptr == NULL)
  {
    printf("Unable to find config file.\n");
  }
  else
  {
    while (fgets(inputline, sizeof(inputline), fptr) != NULL)
    {

      inputptr=inputline;
      sline=split(inputptr);
			check=exc(sline);
      if (check == 1)
      {
        exit(0);
      }
    }
  }
  printf("the end\n");
}
int main(int argc, char *argv[]){

  if (argc == 1)
  {
    interactive();
  }
  else if (argc == 2)
  {
    batchfile(argv[1]);
  }
  else
    return 0;
}
