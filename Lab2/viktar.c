// Layaal Khellah
// Fall 2023: CS 333
// Lab2: viktar


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "viktar.h"


void display_options(void);

int main(int argc, char *argv[])
{
	int action = 0; //used for x,c, t, or T because only one should be on command line at once
	int opt = 0;
	int file = 0;
	char *filename = NULL;
	display_options();
	

	
	while((opt = getopt(argc, argv,  OPTIONS)) != -1 )
	{
		switch(opt)
		{
			case 'x':
				action = 1;
				break;	
			case 'c':
				action = 2;
				break;
			case 't':
				action = 3;
				break;
			case 'T':
				action = 4;
				break;
			case 'f':
				filename = optarg;
				break;
			case 'h':
				display_options();
				exit(EXIT_SUCCESS);
				break;				
			case'v':
				verbose = 1;
				fprintf(stderr,"verbose level: %d\n", verbose);
				break;
			default:
				printf("./viktar: invalid option -- '%c'", optopt); //NOT OPTOPT FIX
				printf("\noopsie - unrecognized command line option FIX THIS"\n);
				break; //not exit since it says "The program should ignore the wayward option and continue (if possible)"
		}


	}
	if (action == 0) {
		exit(EXIT_SUCCESS);
	}
	switch(action) 
	{
		case 1: //extract
		break;
		case 2: //create
		break;
		case 3: //short table
		case 4: //long table
			toc(filename, action);
		break;


	}
	//if(extract && filename != NULL) //archive filename given
	//else {} //read archive file from stdin
	//if(create && filename != NULL) //write archive to stdout

}

void display_options(void)
{

	printf("help text");
	printf("\n\t./viktar");
	printf("\n\tOptions: xctTf:hv");
	printf("\n\t\t-x\t\textract file/files from archive");
	printf("\n\t\t-c\t\tcreate an archive file");
	printf("\n\t\t-t\t\tdisplay a short table of contents of the archive file");
	printf("\n\t\t-T\t\tdisplay a long table of contents of the archive file");
	printf("\n\t\tOnly one of xctT can be specified");	
	printf("\n\t\t-f filename\tuse filename as the archive file");
	printf("\n\t\t-v\t\tgive verbose diagnostic messages");
	printf("\n\t\t-h\t\tdisplay this AMAZING help message\n");


}

void toc(char * filename, int action)
{
	int ifd = STDIN_FILENO;
	char buf[100];
	viktar_header_t header;

	if (filename != NULL) 
	{
		ifd = open(filename, O_RDONLY);
		//validate that its opened  if open fails, if have time
	}
	read(ifd, buf, strlen(VIKTAR_NAME));
	if (strncmp(buf, VIKTAR_NAME, strlen(VIKTAR_NAME)) != 0)
	{
		//invalid viktar file
		//snarky message 
		//exit
	}
	
	while(read(ifd, &header, sizeof(header)) > 0) 
	{
		if(action == 3) //small table t
		{
			memset(buf, 0, 100);
			strncpy(buf, header.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
			printf("%s\n", buf) //double check formatting !!!

		}
		else //action = 4, aka big table T
		{

		}
		lseek(ifd, header.st_size, SEEK_CUR); //jmps to next header	

	}
	
	if(filename != NULL)
	{
		close(ifd);
	}


}
