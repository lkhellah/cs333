// Layaal Khellah
// Fall 2023: CS 333
// Lab2: viktar


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <grp.h>
#include <inttypes.h>
#include <sys/time.h>

#include "viktar.h"

void extract(char * filename, char ** string);
void create(char * filename, char *argv[], int argc);
void toc(char * filename, int action);
void display_options(void);
void symb_rep(mode_t mode);
int verbose = 0;

int main(int argc, char *argv[])
{
	int action = 0; //used for x,c, t, or T because only one should be on command line at once
	int opt = 0;
	char *filename = NULL;

	
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
				//printf("./viktar: invalid option -- '%c'", optopt); //NOT OPTOPT FIX
				printf("oopsie - unrecognized command line option\n");
				break; //not exit since it says "The program should ignore the wayward option and continue (if possible)"
		}


	}
	if (action == 0) {
		exit(EXIT_SUCCESS);
	}
	switch(action) 
	{
		case 1: //extract
			extract(filename, &argv[optind]); //&argv[optind] gives address of list of files  
		break;
		case 2: //create
			create(filename, &argv[optind], argc);
		break;
		case 3: //short table
		case 4: //long table
			toc(filename, action);
		break;


	}

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
		if(ifd < 0)
		{
			fprintf(stderr, "failed to open input archive file %s", filename);
			printf("exiting...\n");
		}
	}	
	else
	{
		printf("archive from stdin\n");
	}
	read(ifd, buf, strlen(VIKTAR_FILE));
	if (strncmp(buf, VIKTAR_FILE, strlen(VIKTAR_FILE)) != 0) 
	{
		printf("invalid viktar file\n"); //FIX STATEMENT  ./viktar -t -f 01-.txt, IS THIS LINES 115-116 TOO
		exit(EXIT_SUCCESS);
	}

	printf("Contents of viktar file: \"%s\"\n", filename);
	while(read(ifd, &header, sizeof(header)) > 0) 
	{
		
		memset(buf, 0, 100);
		strncpy(buf, header.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);
		printf("\tfile name: %s\n", buf); 
		if(action == 4) //action = 4, aka big table T //mode, time, uid, gid 
		{
			
			struct passwd *pwd;
			struct group *grp;
			char mtime_str[30];
			char atime_str[30];
			struct tm mtime_tm;
			struct tm atime_tm;
			//print mode
			symb_rep(header.st_mode); 
			//print user
			pwd = getpwuid(header.st_uid);
			printf("\t\tuser: %s\n", pwd->pw_name); 
			//print group
			grp = getgrgid(header.st_gid);
			printf("\t\tgroup: %s\n", grp->gr_name);
			//print size
			printf("\t\tfile size: %jd\n", (intmax_t) header.st_size);
			//print time
			localtime_r(&header.st_mtim.tv_sec, &mtime_tm);
			strftime(mtime_str, sizeof(mtime_str), "mtime: %Y-%m-%d %H:%M:%S %Z", &mtime_tm);
			localtime_r(&header.st_atim.tv_sec, &atime_tm);
			strftime(atime_str, sizeof(atime_str), "atime: %Y-%m-%d %H:%M:%S %Z", &atime_tm);
			printf("%s\n%s\n", mtime_str, atime_str);
			
		}
		lseek(ifd, header.st_size, SEEK_CUR); //jmps to next header	

	}
	
	if(filename != NULL)
	{
		close(ifd);
	}


}

void symb_rep(mode_t mode)
{
         //order: file, user, group, other
         char symbolicMode[11]; //10 for characters, 11 null


         memset(symbolicMode, '-', sizeof(symbolicMode)-1);
 
 
         //first character is file type
         if (S_ISDIR(mode)) symbolicMode[0] = 'd';
         else if (S_ISLNK(mode)) symbolicMode[0] = 'l';
         else if (S_ISFIFO(mode)) symbolicMode[0] = 'p';
         else if (S_ISSOCK(mode)) symbolicMode[0] = 's';
         else if (S_ISCHR(mode)) symbolicMode[0] = 'c';
         else if (S_ISBLK(mode)) symbolicMode[0] = 'b';
         else symbolicMode[0] = '-';  // Default to '-' for regular files
 
         //next three are user permissions
         if (mode & S_IRUSR) symbolicMode[1] = 'r';
         if (mode & S_IWUSR) symbolicMode[2] = 'w';
         if (mode & S_IXUSR) symbolicMode[3] = 'x';
 
         //following three are group permissions
         if (mode & S_IRGRP) symbolicMode[4] = 'r';
         if (mode & S_IWGRP) symbolicMode[5] = 'w';
         if (mode & S_IXGRP) symbolicMode[6] = 'x';
 
         //last thre are other permissions
         if (mode & S_IROTH) symbolicMode[7] = 'r';
         if (mode & S_IWOTH) symbolicMode[8] = 'w';
         if (mode & S_IXOTH) symbolicMode[9] = 'x';
 
         //checking S_ISGID for the special bits set:
         if (mode & S_ISUID) {
                 symbolicMode[3] = 'S'; // Set user permission to "s" for S_ISUID
         }
 
         if (mode & S_ISGID) {
                 symbolicMode[6] = 'S'; // Set group permission to "s" for S_ISGID
         }
         symbolicMode[10] = ' ';  
         symbolicMode[11] = '\0';
 
         printf("\t\tmode: %s\n", symbolicMode);
 
 }
void extract(char * filename, char ** argv)
{

	int ifd = STDIN_FILENO;
	int ofd;
	viktar_header_t header;
	char viktar_signature[100];
	mode_t old_umask;

	if (filename != NULL) 
	{
		if(verbose) {
			fprintf(stderr, "reading archive file: %s", filename);
		}
		ifd = open(filename, O_RDONLY);
		if(ifd < 0)
		{
			fprintf(stderr, "failed to open input archive file %s", filename);
			printf("exiting...\n");
			exit(EXIT_FAILURE);
		}
	}	
	else
	{
		printf("archive from stdin\n");
	}
	//verifyig header
	read(ifd, viktar_signature, strlen(VIKTAR_FILE));
	if (strncmp(viktar_signature, VIKTAR_FILE, strlen(VIKTAR_FILE)) != 0) 
	{
		printf("invalid viktar file\n"); //FIX STATEMENT  ./viktar -t -f 01-.txt, IS THIS LINES 115-116 TOO
		exit(EXIT_SUCCESS);
	}

	// Set umask to 0 to ensure correct permissions
    	old_umask = umask(0);
	
    	while (read(ifd, &header, sizeof(header)) > 0) {
		char buf[100]; 
		char * file_data;
		struct timeval times[2];
        	int should_extract = 0; // Initialize to not extract
		
		memset(buf, 0, sizeof(buf));
		strncpy(buf, header.viktar_name, VIKTAR_MAX_FILE_NAME_LEN);	
		
		// Check if the current file should be extracted

        	for (int i = optind; argv[i] != NULL; i++) {
            		if (strcmp(buf, argv[i]) == 0) {
                		should_extract = 1;
                		break;
            		}
        	}
		
		if (should_extract)
		{
			// Open a new file with the same name as in the archive and specified permissions
			ofd = open(buf, O_WRONLY | O_CREAT, header.st_mode);
			if (ofd < 0) {
				fprintf(stderr, "Failed to open output file %s\n", buf);
				continue; // Skip to the next file
			}

			// If verbose, print a message about extracting the file
			if (verbose) {
			printf("Extracting file: %s\n", buf);
			}

			// Allocate a buffer for the file's data, read the data, and write it to the new file
			file_data = (char *)malloc(header.st_size);
			if (read(ifd, file_data, header.st_size) < 0) {
				fprintf(stderr, "Failed to read file data for %s\n", buf);
				close(ofd);
				free(file_data);
				continue; // Skip to the next file
			}
			write(ofd, file_data, header.st_size);
			close(ofd);
			free(file_data);

			// Set the file's permissions to match the information from the archive
			fchmod(ofd, header.st_mode);

			// If the restore time flag is set, set the file's access and modification times
			times[0].tv_sec = header.st_atim.tv_sec;
			times[0].tv_usec = header.st_atim.tv_nsec / 1000;
			times[1].tv_sec = header.st_mtim.tv_sec;
			times[1].tv_usec = header.st_mtim.tv_nsec / 1000;
			futimes(ofd, times);
		}
		else
		{
			// Skip to the next file in the archive
			lseek(ifd, header.st_size, SEEK_CUR);
		}
	}
	// Reset the umask to its previous value
	umask(old_umask);
	if (filename != NULL) {
		close(ifd);
    	}




}


void create(char *filename, char *argv[], int argc) {
    int ofd;  // Output file descriptor
    viktar_header_t header;
    char *archive_filename;

    // Determine the output file descriptor
    if (filename != NULL) {
        ofd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Use specified filename and 0644 permissions
        if (ofd < 0) {
            fprintf(stderr, "Failed to open output archive file %s\n", filename);
            exit(EXIT_FAILURE);
        }
        archive_filename = filename;
    } else {
        // Output to stdout
        ofd = STDOUT_FILENO;
        archive_filename = "stdout";
    }

    if (argc > 1) 
    {
        // Archive contains files, write the viktar header
        if (write(ofd, VIKTAR_FILE, strlen(VIKTAR_FILE)) < 0) {
            fprintf(stderr, "Failed to write viktar header to %s\n", archive_filename);
            exit(EXIT_FAILURE);
        }
        
        // Process each file specified on the command line
        for (int i = 1; i < argc; i++) 
	{
            char *file = argv[i]; // Renamed variable
            struct stat st;
	    int ifd;
	    char buffer[1000];
            ssize_t bytes_read;
            // Set the viktar header information for the current file
            memset(&header, 0, sizeof(viktar_header_t));
            strncpy(header.viktar_name, file, VIKTAR_MAX_FILE_NAME_LEN);

            // Use stat to populate the header fields
            if (stat(file, &st) == 0) {
                header.st_mode = st.st_mode;
                header.st_uid = st.st_uid;
                header.st_gid = st.st_gid;
                header.st_size = st.st_size;
                header.st_atim = st.st_atim;
                header.st_mtim = st.st_mtim;
                header.st_ctim = st.st_ctim;
            } else {
                perror("Failed to stat source file");
                continue;
            }

            // Write the viktar header for the current file
            if (write(ofd, &header, sizeof(header)) < 0) {
                fprintf(stderr, "Failed to write viktar header for %s to %s\n", file, archive_filename);
                exit(EXIT_FAILURE);
            }

            // Open and write the content of the current file to the archive
            ifd = open(file, O_RDONLY);
            if (ifd < 0) {
                fprintf(stderr, "Failed to open file %s for archiving\n", file);
                continue;
            }


            while ((bytes_read = read(ifd, buffer, sizeof(buffer))) > 0) {
                if (write(ofd, buffer, bytes_read) < 0) {
                    fprintf(stderr, "Failed to write file content for %s to %s\n", file, archive_filename);
                    break;
                }
            }

            close(ifd);
        }
    } else {
        // No files specified, create an empty viktar archive with just the header
        if (write(ofd, VIKTAR_FILE, strlen(VIKTAR_FILE)) < 0) {
            fprintf(stderr, "Failed to write viktar header to %s\n", archive_filename);
            exit(EXIT_FAILURE);
        }
    }

    if (filename != NULL) {
        close(ofd);
    }

}



