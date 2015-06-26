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
bool allImages = false;
int imageCount = 9;

static struct option options[] =
	{
		{"all",    no_argument,       0, 'a'},
		{"help",   no_argument,       0, 'h'},
		{"output", required_argument, 0, 'o'},
		{"source", required_argument, 0, 's'},
		{"title",  required_argument, 0, 't'},
		{"urls",   required_argument, 0, 'u'}
	};

// Read and print the input in a stream
void printAll(istream* in)
{
	string line;
	while (getline(*in, line)) 
		cout << line << endl;
}

void help()
{
	cout << "to-html: Generate a html page that shows random images" << endl
		 << endl
		 << "  -a, --all            Generate a grid of 9 images" << endl
		 << "  -h, --help           Show help" << endl
		 << "  -o, --output=FILE    Output destination (default: stdout)" << endl
		 << "  -s, --source=FILE    Source for the html template (default: 'template.html')" << endl
		 << "  -t, --title=STRING   Title of the html document (default: 'bunny-search')" << endl
		 << "  -u, --urls=FILE      Source of the image URLs (default: 'urls.txt')" << endl;
}

int main(int argc, char* argv[])
{
	int c;
	int option_index = 0;
	while ((c = getopt_long (argc, argv, "aho:s:t:u:", 
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
	string line;
	while (getline(tmplt, line)) 
	{
		if (line == "===TITLE===")
		{
			cout << title << endl;
		}
		else if (line == "===URLS===")
		{
			// Redirect input
			ifstream fin(urlsFile.c_str());
			if (!fin.good()) 
			{
				cerr << "to-html: " << urlsFile << ": No such file" << endl;
				exit(1);
			}

			string url;
			while (getline(fin, url))
			{
				cout << "'" << url << "'," << endl;
			}
			fin.close();
		}
		else
		{
			cout << line << endl;
		}
	}
	tmplt.close();

	cout.rdbuf(coutbuff);
	fout.close();
}
