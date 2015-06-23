#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

string title = "bunny-search";
string inputFile = "urls.txt";
string outputFile = "stdout";
string templateFile = "template.html";
bool allImages = false;
int imageCount = 9;

static struct option options[] =
	{
		{"all",    no_argument,       0, 'a'},
		{"help",   no_argument,       0, 'h'},
		{"input",  required_argument, 0, 'i'},
		{"output", required_argument, 0, 'o'},
		{"source", required_argument, 0, 's'},
		{"title",  required_argument, 0, 't'}
	};

// Just prints the input as-is
void printAll()
{
	string line;
	while (getline(cin, line)) 
		cout << line << endl;
}

void help()
{
	cout << "to-html: Generate a html page that shows random images" << endl
		 << endl
		 << "  -a, --all            Generate a grid of 9 images" << endl
		 << "  -h, --help           Show help" << endl
		 << "  -i, --input=FILE     Source of the image URLs (default: 'urls.txt')" << endl
		 << "  -o, --output=FILE    Output destination (default: stdout)" << endl
		 << "  -s, --source=FILE    Source for the html template (default: 'template.html')" << endl
		 << "  -t, --title=STRING   Title of the html document (default: 'bunny-search')" << endl;
}

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while ((c = getopt_long (argc, argv, "ahi:o:t:", 
				options, &option_index)) != -1)
	{
		switch (c)
		{
			case 'a': 
				allImages = true;
				break;
			case 'h':
				help();
				exit(0);
			case 'i':
				inputFile = optarg;
				break;
			case 'o':
				outputFile = optarg;
				break;
			case 's':
				templateFile = optarg;
				break;
			case 't':
				title = optarg;
				break;
			default:
				cerr << "Unkown option -" << c << endl
					 << "Try 'to-html --help' for more information" << endl;
				exit(1);
				break;
		}
	}

	ios::sync_with_stdio(false);
	// Redirect input
	ifstream fin(inputFile.c_str());
	if (!fin.good()) 
	{
		cerr << "to-html: " << inputFile << ": No such file" << endl;
		exit(1);
	}
	cin.rdbuf(fin.rdbuf());
	
	// Redirect output
	ofstream fout;
	if (outputFile != "stdout") 
	{
		fout.open(outputFile.c_str());
		cout.rdbuf(fout.rdbuf());
	}
	//printAll();
	

	// Read template
	ifstream tmplt(templateFile.c_str());
	if(!tmplt.good()) cerr << "bad";
	string line;
	while (getline(tmplt, line)) 
	{
		if (line == "===TITLE===")
		{
			cout << title << endl;
		}
		else if (line == "===URLS===")
		{
			string url;
			while (getline(cin, url))
				cout << "'" << url << "'," << endl;
		}
		else
		{
			cout << line << endl;
		}
	}

	fin.close();
	fout.close();
	tmplt.close();
}
