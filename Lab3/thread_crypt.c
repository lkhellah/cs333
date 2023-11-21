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
#include <crypt.h> //use crypt_rn
#include "thread_crypt.h"

void print_help(void);
int get_next_word(void);
void *hash(void * arg, int algorithm);

int main(int argc, char *argv[])
{
	int opt = 0;
	char * ifile = NULL;
	char * ofile = NULL;
	FILE *output_file = stdout; //default output without -o
	FILE *input_file; 
	int algorithm = 0; // default is DES 
	int salt_length = 2; //default salt length corresponding to default algorithm
	int rounds = 5000;
	unsigned int seed = 0; 
	int num_threads = 1; //default number of threads
	int verbose = 0;
	struct stat st;
	pthread_t *threads = NULL;

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
			
			case 'r': // used with -l 5 or -l 6
				if (rounds < 1000) 
					rounds = 1000;
				else if (rounds > 999999999) 
					rounds = 999999999;	
				break;
	
			case 'R':
				seed = atoi(optarg);
				srand(seed);
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

	switch(algorithm)
	{
		case 0:
			salt_length = 2;
			break;
		case 1:
			if (salt_length > 8) salt_length = 8;
			break;
		case 5:
		case 6:
			if(salt_length > 16) salt_length = 16;
			break;
		default:
			fprintf(stderr, "invalid algorithm\n");
			exit(EXIT_FAILURE);
			break;
	}

	// INPUT
	// ifile: name of input file
	// input_file: FILE * for file stream //could use file descriptor 
	// file_content: dynamically allocated buffer to store content of input file
	if (NULL != ifile)
	{
		char * file_content;
		
		// use stat to figure out number of bytes in input file
		if (stat(ifile, &st) == 0)
		{
			//use malloc to allocate that amount
			file_content = (char *)malloc(st.st_size + 1);
			memset(file_content, 0, st.st_size + 1);
			//check malloc success
			if(file_content != NULL) 
			{
				// open the file for reading
				input_file = fopen(ifile, "r");
				if(input_file != NULL)
				{
					//is this where i call get next word 
					//fread reading whole file possibly, check file_content
					fread(file_content, 1, st.st_size, input_file);
					//use file_content to access input file content
					//
					
				}		
				else 
				{
					fprintf(stderr,"failed to open input file\n")
					exit(EXIT_FAILURE);
				}
				

			}
			else
			{
				perror("malloc error\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			perror("stat error\n");
			exit(EXIT_FAILURE);	
		}
	}
	else
	{
		fprintf(stderr, "must provide input file name\n");
		exit(EXIT_FAILURE);	
	}

	// OUTPUT
	if (NULL != ofile) // write to file
	{
		output_file = fopen(ofile, "w");
		if (output_file == NULL)
		{
			perror("Error opening the output file");
			exit(EXIT_FAILURE);
		}
             

	}
	
	/*for (int i = 0; i < num_threads; ++i)
	{
		pthread_create(//arg week 5 material)
	}*/
	//start multi thread: loop with pthread create and join, use mutex somewhere here 
	hash(file_content);
	fclose(output_file); // Close the file when done
	free(file_content);
}
void gen_salt(char * salt, int algorithm, int salt_length)
{
		static const char salt_chars[] = {SALT_CHARS};
		char s[CRYPT_OUTPUT_SIZE] = NULL;
		//salt stuff
		switch (algorithm) 
		{
			case 1:
			case 5:
			case 6:
				for(int i = 0, rand_value = 0; i < salt_length; ++i) {
					rand_value = rand();
					rand_value %= strlen(salt_chars);
					s[i] = salt_chars[rand_value];
				}
				sprintf(salt, "$%d$%s%s$", algorithm, (rounds ? rounds : ""), s);
			break;
			default:
				rand_value = rand();
				rand_value %= strlen(salt_chars);
				salt[0] = salt_chars[rand_value];
				rand_value = rand();
				rand_value %= strlen(salt_chars);
				salt[1] = salt_chars[rand_value];
				break;

}


void * hash(void * arg)
{ 
	//char * crypt_rn(const char *phrase, const char *setting, struct crypt_data *data, int size);
	char * text = (char *) arg;
	struct crypt_data data;
	char *hashed_password;
	hashed_password = crypt_rn(text, salt, 
	char salt[CRYPT_OUTPUT_SIZE]
	//char salt[CRYPT_OUTPUT_SIZE] = {'\0'}; 

	/*
	use memset to initalize all data fields of 'data'. memset(data, 0, sizeof(data))
	while loop, need var ex 'password'. while (passwird = getnextword(....) != NULL)
		within while loop, call gen_salt(empty salt variable)
		strncpy(from salt to data.setting, CRYPT_OUTPUT_SIZE) 
		strncpy(from word to data.input, CRYPT_MAX_PASSPHRASE_SIZE)
		call encryption: crypt_rn(salt, word, &data, sizeof(data))
		fprintf(salt)
	pthread_exit(EXIT_SUCCESS)
	*/
	
}

void print_help(void)
{
	fprintf(stderr, "./thread_crypt ...\n");
	fprintf(stderr, "	Options: i:o:hva:l:R:t:r:\n"
			"	-i file		input file name (required)\n"
			"	-o file		output file name (default stdout)\n"
			"	-a #		algorithm to use for hashing [0,1,5,6] (default 0 = DES)\n"
			"	-l #		length of salt (default 2 for DES, 8 for MD-5, 16 for SHA)\n"
			"	-r #		rounds to use for SHA-256, or SHA-512 (default 5000)\n"
			"	-R #		seed for rand() (default none)\n"
			"	-t #		number of threads to create (default 1)\n"
			"	-v		enable verbose mode\n"
			"	-h		helpful text\n");
}
