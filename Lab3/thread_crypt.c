// Layaal Khellah
// CS 333 Fall'23
// Lab 3
// Write a multi-threaded application that demonstrates 
// the performance improvement of using multiple threads
// An introduction to the use of crypt() and salted passwords


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "thread_crypt.h"

void print_help();
int main(int argc, char *argv[])
{
	int opt = 0;
	char * ifile = NULL;
	char * ofile = NULL;
	int algorithm = 0; // default is DES 
	int salt_length = 2; //default salt length corresponding to default algorithm
	int rounds = 5000;
	unsigned int seed = 0; //is 0 fine for default orrr
	int num_threads = 1; //default number of threads
	int verbose = 0;

	while ((opt = getopt(argc, argv, OPTIONS)) != -1)
	{
		switch(opt)
		{
			case 'i':
				ifile = optarg;
				break;

			case 'o':
				ofile = optarg;
				break;
	
			case 'a':
				algorithm = atoi(optarg);
				break;

			case 'l':
				salt_length = atoi(optarg);
				break;
			
			case 'r':
				if (rounds < 1000) 
					rounds = 1000;
				else if (rounds > 999999999) 
					rounds = 999999999;	
				break;
	
			case 'R':
				seed = atoi(optarg);
				break;

			case 't':
				num_threads = atoi(optarg);
				if (num_threads < 1 || num_threads > 20)
				{
					fprintf(stderr, "error: valid number of threads are 1-20. Using default: 1\n");
					num_threads = 1;
				}
				break;

			case 'v':
				verbose = 1;
				fprintf(stderr,"verbose mode enabled\n");
				break;

			case 'h':
				print_help();
				break;	
			default:
				fprintf(stderr, "invalid option\n");
				exit(EXIT_FAILURE);

		}

	}


	if (NULL != ofile) // write to file
	{
		int file_descriptor = open(ofile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (file_descriptor == -1)
		{
			perror("Error opening the output file");
			exit(EXIT_FAILURE);
		}
             
		// Write to the file using the file descriptor
		// ...

		close(file_descriptor); // Close the file when done
	}
	
	else //write to stdout 
		printf("Writing to stdout (layaal print)\n");

}


