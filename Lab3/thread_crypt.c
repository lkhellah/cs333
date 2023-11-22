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
#include <crypt.h> 
#include <string.h>
#include <pthread.h>

#include "thread_crypt.h"

void print_help(void);
char * get_next_word(void);
void *hash(void *);
void gen_salt(char * salt);

int verbose = 0;
static int algorithm = 0; // default is DES 
static int salt_length = 2; //default salt length corresponding to default algorithm
static int rounds = 5000;
char * file_content;

int main(int argc, char *argv[])
{
	int opt = 0;
	char * ifile = NULL;
	char * ofile = NULL;
	FILE *output_file = stdout; //default output without -o
	FILE *input_file; 
	unsigned int seed = 0; 
	int num_threads = 1; //default number of threads
	struct stat st;
	pthread_t *threads = NULL;
	long tid = 0;
	
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
					//fread reading whole file
					fread(file_content, 1, st.st_size, input_file);
					//write(1, file_content, st.st_size);
				}		
				else 
				{
					fprintf(stderr,"failed to open input file\n");
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
	
	//start multi thread: loop with pthread create and join, use mutex somewhere here 
	threads = malloc(num_threads * sizeof(pthread_t));	
	for (tid = 0; tid < num_threads; tid++)
	{
		pthread_create(&threads[tid], NULL, hash, NULL);
	}
	for (tid = 0; tid < num_threads; tid++) {
		pthread_join(threads[tid], NULL);
	}
	fclose(output_file); // Close the file when done
	free(file_content);
}

char * get_next_word(void)
{
	char * word = NULL;
 	static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;		
	static int initialized = 0;
	pthread_mutex_lock(&lock);
	if(!initialized) {
		initialized = 1;
		word = strtok(file_content, "\n");
	}
	else {
		word = strtok(NULL, "\n");
	}
	pthread_mutex_unlock(&lock);
	return word;
}

void gen_salt(char * salt)
{
		static const char salt_chars[] = {SALT_CHARS};
		char s[CRYPT_OUTPUT_SIZE] = {'\0'};
		int rand_value = 0;
		switch (algorithm) 
		{
			case 1:
			case 5:
			case 6:
				for(int i = 0; i < salt_length; ++i) {
					rand_value = rand();
					rand_value %= strlen(salt_chars);
					s[i] = salt_chars[rand_value];
				}
				sprintf(salt, "$%d$%d%s$", algorithm, rounds, s);
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
}


void * hash(void * v)
{ 
	//char * crypt_rn(const char *phrase, const char *setting, struct crypt_data *data, int size);
	struct crypt_data data;
	char *word;
	char salt[CRYPT_OUTPUT_SIZE] = {'\0'}; 
	char * hashed_password;

	memset(&data, 0, sizeof(data));
	word = get_next_word();
	while (word != NULL) {
		gen_salt(salt);
		strncpy(data.setting, salt, CRYPT_OUTPUT_SIZE);
		strncpy(data.input,word , CRYPT_MAX_PASSPHRASE_SIZE);
		hashed_password = crypt_rn(word, salt, &data, sizeof(data));
		printf("%s:%s\n", word, hashed_password);
		word = get_next_word();
	}
	pthread_exit(EXIT_SUCCESS);
	
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
