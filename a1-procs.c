//////-------------------------------------------
////// NAME: Kajal Tomar
////// STUDENT NUMBER: 7793306
////// COURSE: COMP 3430, SECTION: A01
////// INSTRUCTOR: Franklin Bristow
////// ASSIGNMENT: assignment 1, QUESTION: question 2
//////
////// REMARKS: What is the purpose of this program?
//////
//////-----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>

int read_config();
void create_workers(int amount); 

void do_work(); 

int worker_amount;
pid_t child; 
pid_t parent_id; 

void handle_SIGINT(int sig){
	(void) sig;
	
	if(getppid() == parent_id){
		printf("<%i>is being killed.\n", getpid());
		printf("<%i>is being exiting.\n", getpid());
		exit(EXIT_SUCCESS);
	}
}

void handle_SIGHUP(int sig){
	(void) sig;

	int new_workers = read_config();
	int difference = new_workers - worker_amount;
	worker_amount = new_workers;	
	if(new_workers > 0){
		// need that many more
		printf("<%d> Must create %i new worker(s).\n", getpid(), difference);
		create_workers(difference);
		do_work();
	}
	else if(new_workers < 0){
		// need that many less 
	}

	
}

int main(void){
	parent_id = getpid();	
	worker_amount =	read_config();
	
	signal(SIGINT, handle_SIGINT);
	signal(SIGHUP, handle_SIGHUP);
	
	printf("<%d>I am running\n", getpid());	
			
	create_workers(worker_amount); 
	do_work();
		
	printf("<%d>I am back in main\n", getpid());	
	return EXIT_SUCCESS;

}

int read_config(){
	FILE *config_file  = fopen("config.txt", "r"); // read only 
	int amount_read;
	assert(config_file != NULL); 
	printf("Reading config file.\n");
	fscanf(config_file, "%i", &amount_read);
	printf("Changing setting to: %i\n", amount_read);

	fclose(config_file);
	return amount_read;

}

void create_workers(int amount){
	for(int i = 0; i < amount; i++){
		if(getpid() == parent_id){
			child = fork();
		}
		else{
			break;								         
		}
	}

}

void do_work(){
	if(child == 0){
		printf("<%d> is starting\n", getpid());
		fflush(stdout);
		while(1) {}
	}
	else{
		wait(NULL);
	}
}

