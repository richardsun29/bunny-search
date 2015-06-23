#include <iostream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

string title;
bool allImages = false;
int imageCount = 9;


static struct option options[] =
	{
		{"all",   no_argument,       0, 'a'},
		{"title", required_argument, 0, 't'}
	};

void print_urls()
{

}

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while((c = getopt_long (argc, argv, "t:", options, &option_index)) != -1)
	{
		switch(c)
		{
			case 'a': 
				allImages = true;
				break;
			case 't':
				title = optarg;
				break;
			default:
				printf("Unknown option %c\n", c);
				exit(1);
				break;
		}
	}


}
