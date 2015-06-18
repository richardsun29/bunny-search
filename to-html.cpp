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
	</head>\n\
	<body>\n\
");

	string url;
	for(int k = 0; k < 10; k++) {
		getline(cin, url);
		cout << "<img src=\"" << url << "\" />\n";
	}

	puts("\
	</body>\n\
</html>\n\
");
}
