#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

string title;
string inputFile = "urls.txt";
string outputFile = "stdout";
bool allImages = false;
int imageCount = 9;


static struct option options[] =
	{
		{"all",    no_argument,       0, 'a'},
		{"input",  required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"title",  required_argument, 0, 't'}
	};

void printAll()
{
	string line;
	while(getline(cin, line)) cout << line << endl;
}

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while((c = getopt_long (argc, argv, "ai:o:t:", options, &option_index)) != -1)
	{
		switch(c)
		{
			case 'a': 
				allImages = true;
				break;
			case 'i':
				inputFile = optarg;
				break;
			case 'o':
				outputFile = optarg;
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

	// Redirect input
	ifstream fin(inputFile.c_str());
	if(!fin.good()) {
		printf("to-html: %s: No such file\n", inputFile.c_str());
		exit(1);
	}
	cin.rdbuf(fin.rdbuf());
	
	// Redirect output
	ofstream fout;
	if(outputFile != "stdout") {
		fout.open(outputFile.c_str());
		cout.rdbuf(fout.rdbuf());
	}

	printAll();
}
