// Layaal Khellah
// Fall 2023: CS 333
// Lab2: viktar


#include <unistd.h>
#include <viktar.h>

int main(int argc, char *agrv[])
{
	int action = 0; //used for x,c, t, or T because only one should be on command line at once
	int opt = 0;
	while((opt = getopt(argc, argv,  OPTIONS)) != -1 )
	{
		switch(opt)
		{
			case 'x':
				action = 1;
				break;	
			case '





		}



	}








}
