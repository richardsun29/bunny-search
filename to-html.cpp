#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	puts("\
<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<title></title>\n\
		<style>\n\
		img {\n\
			width: 33%;\n\
		}\n\
		</style>\n\
	</head>\n\
	<body>\
");

	string url;
	for(int k = 0; k < 10; k++) {
		getline(cin, url);
		cout << "\t\t<img src=\"" << url << "\" />\n";
	}

	puts("\
	</body>\n\
</html>\n\
");
}
