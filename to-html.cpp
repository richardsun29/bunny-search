#include <iostream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

static struct option options[] =
	{
		{"title", required_argument, 0, 't'}
	};

int main(int argc, char* argv[])
{
	string title;
	int c;
	int option_index = 0;
	while((c = getopt_long (argc, argv, "t:", 
					options, &option_index)) != -1)
	{
		switch(c)
		{
			case 't':
				title = optarg;
				break;
			default:
				printf("Unknown option %c\n", c);
				exit(1);
				break;
		}
	}

	printf("\
<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<title>%s</title>\n\
		<style>\n\
		img {\n\
			width: 33%%;\n\
		}\n\
		</style>\n\
	</head>\n\
	<body>\n\
", title.c_str());

	string url;
	for(int k = 0; k < 9; k++) {
		getline(cin, url);
		printf("\t\t<img src=\"%s\" />\n", url.c_str());
	}

	puts("\
	</body>\n\
</html>\n\
");
}
