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

void print_script()
{
	printf("\
		<script src=\"lib/jquery.js\"></script>\n\
		<script>\n\
		var urls = [\n\
");

	string url;
	while(getline(cin, url))
		printf("\t\t\t\t\"%s\",\n", url.c_str());

	printf("\
			];\n\
			$(document).ready(function() {\n\
				var rand = Math.floor(Math.random() * urls.length);\n\
				$('img').replaceWith(\"<img src='\" + urls[rand] + \"'>\");\n\
			});\n\
		</script>\n\
");
}

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while((c = getopt_long (argc, argv, "t:", 
					options, &option_index)) != -1)
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
", title.c_str());

	if(!allImages)
		print_script();

	printf("\
	</head>\n\
	<body>\n\
");

	string url;
	if(allImages){
		for(int k = 0; k < imageCount; k++) {
			getline(cin, url);
			printf("\t\t<img src=\"%s\" />\n", url.c_str());
		}
	} else {
		printf("\t\t<img src=\"random\">\n");
	}

	puts("\
	</body>\n\
</html>\n\
");
}
