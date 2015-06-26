#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

string TITLE = "bunny-search";
string URLS_FILE = "urls.txt";
string OUTPUT_FILE = "stdout";
string TEMPLATE_FILE = "template.html";
bool GRID = false;
int GRIDSIZE = 9;


void printAll(istream* in);
void fileNotFound(string filename);

void printUrls();
void printTitle();
void printImgWidth();
void printImgTags();

void help()
{
	cout << "to-html: Generate a html page that shows random images" << endl
		 << endl
		 << "  -g, --grid           Generate a grid of 9 images" << endl
		 << "  -h, --help           Show help" << endl
		 << "  -o, --output=FILE    Output destination (default: stdout)" << endl
		 << "  -s, --source=FILE    Source for the html template (default: 'template.html')" << endl
		 << "  -t, --title=STRING   Title of the html document (default: 'bunny-search')" << endl
		 << "  -u, --urls=FILE      Source of the image URLs (default: 'urls.txt')" << endl;
}

static struct option options[] =
	{
		{"grid",    no_argument,      0, 'g'},
		{"help",   no_argument,       0, 'h'},
		{"output", required_argument, 0, 'o'},
		{"source", required_argument, 0, 's'},
		{"title",  required_argument, 0, 't'},
		{"urls",   required_argument, 0, 'u'}
	};

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while ((c = getopt_long (argc, argv, "gho:s:t:u:", 
				options, &option_index)) != -1)
	{
		switch (c)
		{
			case 'g': 
				GRID = true;
				break;
			case 'h':
				help();
				exit(0);
			case 'o':
				OUTPUT_FILE = optarg;
				break;
			case 's':
				TEMPLATE_FILE = optarg;
				break;
			case 't':
				TITLE = optarg;
				break;
			case 'u':
				URLS_FILE = optarg;
				break;
			default:
				cerr << "Unkown option -" << c << endl
					 << "Try 'to-html --help' for more information" << endl;
				exit(1);
		}
	}


	ios::sync_with_stdio(false);
	// Redirect output
	ofstream fout;
	streambuf * coutbuff = cout.rdbuf();
	if (OUTPUT_FILE != "stdout") 
	{
		if (OUTPUT_FILE == "stderr")
			cout.rdbuf(cerr.rdbuf());
		else
		{
			fout.open(OUTPUT_FILE.c_str());
			cout.rdbuf(fout.rdbuf());
		}
	}

	// Read template
	ifstream tmplt(TEMPLATE_FILE.c_str());
	if (!tmplt.good())
		fileNotFound(TEMPLATE_FILE);
	string line;
	while (getline(tmplt, line)) 
	{
		if (line == "===TITLE===")
			printTitle();
		else if (line == "===URLS===")
			printUrls();
		else if (line == "===IMG-WIDTH===")
			printImgWidth();
		else if (line == "===IMG-TAGS===")
			printImgTags();
		else
			cout << line << endl;
	}
	tmplt.close();

	cout.rdbuf(coutbuff);
	fout.close();
}

void printTitle()
{
	cout << TITLE << endl;
}

void printUrls()
{
	// Redirect input
	ifstream fin(URLS_FILE.c_str());
	if (!fin.good()) 
		fileNotFound(URLS_FILE);

	string url;
	while (getline(fin, url))
	{
		cout << "'" << url << "'," << endl;
	}
	fin.close();
}

void printImgWidth()
{
	cout << "width: " << (GRID ? "33" : "50") << "%;" << endl;
}

void printImgTags()
{
	int imgCount = GRID ? GRIDSIZE : 1;
	for (int k = 0; k < imgCount; k++)
		cout << "<img src='' onclick='refreshImage(this)' onerror='removeImage(this)'>" << endl;
}

void fileNotFound(string filename)
{
	cerr << "to-html: " << filename << ": No such file" << endl;
	exit(1);
}

// Read and print the input in a stream
void printAll(istream* in)
{
	string line;
	while (getline(*in, line)) 
		cout << line << endl;
}







