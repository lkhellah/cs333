// Layaal Khellah
// CS 333 Lab1 Part2

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void display_options(void);

int main(int argc, char *argv[])
{
	FILE *ifile = stdin; // Default input is stdin
	char *filename = NULL;
	char buf[BUFFER_SIZE] = {0};
	int count_chars = 0;
	int count_lines = 0;
	int count_words = 0;
	int total_chars = 0;
	int total_lines = 0;
	int total_words = 0;

	int opt = 0;
	while((opt = getopt(argc, argv, "hcwlf:")) != -1)
	{

		switch(opt)
		{
			case'h':
				display_options();
				exit(EXIT_SUCCESS);
				break;
			case'c':
				count_chars = 1;
				break;
			
			case'w':
				count_words = 1;
				break;

			case'l':
				count_lines = 1;
				break;

			case'f':

				filename = optarg;
                		ifile = fopen(filename, "r");
                		if (ifile == NULL) {
                    			fprintf(stderr, "./mywc: Cannot open file '%s'\n", filename);
                    			//exit(2);
					//fprintf(stderr, "Failed to open file %s\n", filename);
                    			//fprintf(stderr, "file open failed: No such file or directory\n");
					exit(2);
                		}
                		break;
			default:
				fprintf(stderr, "./mywc: invalid option -- '%c'\n", optopt);
				exit(EXIT_FAILURE);
		}
	}	

	while(fgets(buf, BUFFER_SIZE, ifile) != NULL) 
	{
		char * token;
		total_lines++;
        	total_chars += strlen(buf);
		
        	//if (count_words) {
            		token = strtok(buf, " \t\n");
            		while (token != NULL) {
                		total_words++;
                		token = strtok(NULL, " \t\n");
            		}
        	//}
    	}
    	
	if (!count_lines && !count_words && !count_chars) count_lines = count_words = count_chars = 1;

    	if (count_lines) {
        	printf("%d ", total_lines);
    	}

	if (count_words) {
		printf("%d ", total_words);
    	}

	if (count_chars) {
		printf("%d ", total_chars);
	}

    	if (ifile != stdin && filename!= NULL)
	{
		printf("%s", filename);
		fclose(ifile);
    	}

	printf("\n");	



	return 0;
}







void display_options(void)
{
	printf("./mywc");
	printf("\toptions: clwf:hv");
	printf("\t-c\t: display the number of characters in the input");
	printf("\t-l\t: display the number of lines in the input");
	printf("\t-w\t: display the number of words in the input");
	printf("\t-f file\t: use file as input, defaults to stdin");
	printf("\t-h\t: display a command options and exit");
}

