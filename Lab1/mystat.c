/*
Layaal Khellah
CS 333 Fall 2023
Lab 1
Part 1: display the inode meta data for
each file given on the command line
*/
//gcc -Wall mystat.c -o mystat

#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <pwd.h>
#include <libgen.h>
#include <grp.h>
#include <time.h>

void symb_rep(mode_t mode);

int main(int argc, char *argv[])
{
	struct stat sb;
	//char * bname, *basec;

	if (argc <  2) {
		fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
   	}

	for(int i = 1; i < argc; i++)
	{
		
		struct passwd *pwd;
		struct group *grp;
		char atime_str[80], mtime_str[80], ctime_str[80];
		
		if (lstat(argv[i], &sb) == -1) {
			perror("lstat");
	  	}	

		//basec = strdup(argv[i]);
		//bname = basename(basec);
		//printf("File: %s\n", bname);
		printf("File: %s\n", argv[i]);
	
		printf("  File type:                ");
		switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:
	  	{		
			struct stat fsb;
			printf("Symbolic link");
			if (stat(argv[i], &fsb) < 0) {
				printf(" - with dangling destination\n");
	    		}	
	    		else {
			char buf[1024];
		      	ssize_t len = readlink(argv[i], buf, sizeof(buf)-1);
			if (len != -1)
				buf[len] = '\0';
			printf(" -> %s\n", buf);
	    		}
	  	}
		break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
		}


		if(sb.st_mode & S_ISGID) {
			printf("  Special bits set:         set-GID\n");		
		}
		if(sb.st_mode & S_ISUID) {
			printf("  Special bits set:         set-UID\n");
		}
	
		//printf("  Device ID number:         %jxh/%jxd\n", (uintmax_t) major(sb.st_dev), (uintmax_t) minor(sb.st_dev));		
		printf("  Device ID number:         %lxh/%ldd\n", sb.st_dev, sb.st_dev);
		printf("  I-node number:            %ju\n", (uintmax_t) sb.st_ino);	


		//function call to mode symbolic representation
		symb_rep(sb.st_mode);
		

		printf("  Link count:               %ju\n", (uintmax_t) sb.st_nlink);


		//struct passwd *pwd = getpwuid(sb.st_uid);
		pwd = getpwuid(sb.st_uid);
		if (pwd != NULL) {
    			printf("  Owner Id:                 %s           (UID = %ju)\n", pwd->pw_name, (uintmax_t) sb.st_uid);
		} else {
    			perror("getpwuid");
		}
		

		//struct group *grp;
		grp = getgrgid(sb.st_gid);
		if (grp != NULL) {
		    printf("  Group Id:                 %s              (GID = %ju)\n", grp->gr_name, (uintmax_t)sb.st_gid);
		} else {
		    printf("  Group Id:                 Unknown (GID = %ju)\n", (uintmax_t)sb.st_gid);
		}

		printf("  Preferred I/O block size: %jd bytes\n", (intmax_t) sb.st_blksize);
		printf("  File size:                %jd bytes\n", (intmax_t) sb.st_size);
		printf("  Blocks allocated:         %jd\n", (intmax_t) sb.st_blocks);
		


		//char atime_str[80], mtime_str[80], ctime_str[80];
		strftime(atime_str, sizeof(atime_str), "%Y-%m-%d %H:%M:%S %z (%Z) %a", localtime(&sb.st_atime));
		strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%d %H:%M:%S %z (%Z) %a", localtime(&sb.st_mtime));
		strftime(ctime_str, sizeof(ctime_str), "%Y-%m-%d %H:%M:%S %z (%Z) %a", localtime(&sb.st_ctime));
		printf("  Last file access:         %s (local)\n", atime_str);
		printf("  Last file modification:   %s (local)\n", mtime_str);
		printf("  Last status change:       %s (local)\n", ctime_str);


		
	}	
	return EXIT_SUCCESS;

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

    	printf("  Mode:                     %s       (%03o in octal)\n", symbolicMode, mode & 0777u);

}
