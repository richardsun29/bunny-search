#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <getopt.h>
#include <cstdlib>
using namespace std;

string title = "bunny-search";
string urlsFile = "urls.txt";
string outputFile = "stdout";
string templateFile = "template.html";
bool grid = false;
int imageCount = 9;


void printAll(istream* in);
void fileNotFound(string filename);

void printUrls();
void printTitle();
void printImgWidth();

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
				grid = true;
				break;
			case 'h':
				help();
				exit(0);
			case 'o':
				outputFile = optarg;
				break;
			case 's':
				templateFile = optarg;
				break;
			case 't':
				title = optarg;
				break;
			case 'u':
				urlsFile = optarg;
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
	if (outputFile != "stdout") 
	{
		if (outputFile == "stderr")
			cout.rdbuf(cerr.rdbuf());
		else
		{
			fout.open(outputFile.c_str());
			cout.rdbuf(fout.rdbuf());
		}
	}

	// Read template
	ifstream tmplt(templateFile.c_str());
	if (!tmplt.good())
		fileNotFound(templateFile);
	string line;
	while (getline(tmplt, line)) 
	{
		if (line == "===TITLE===")
			printTitle();
		else if (line == "===URLS===")
			printUrls();
		else if (line == "===IMG-WIDTH===")
			printImgWidth();
		else
			cout << line << endl;
	}
	tmplt.close();

	cout.rdbuf(coutbuff);
	fout.close();
}

void printTitle()
{
	cout << title << endl;
}

void printUrls()
{
	// Redirect input
	ifstream fin(urlsFile.c_str());
	if (!fin.good()) 
		fileNotFound(urlsFile);

	string url;
	while (getline(fin, url))
	{
		cout << "'" << url << "'," << endl;
	}
	fin.close();
}

void printImgWidth()
{
	cout << "width: " << (grid ? "33" : "50") << "%;" << endl;
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







