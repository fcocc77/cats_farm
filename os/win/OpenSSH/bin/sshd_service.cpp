#include <iostream>
using namespace std;
#include <unistd.h>// sleep usleep

string dirname( string file ){
	size_t found;
	found=file.find_last_of("/\\");
	return file.substr(0,found);
}

int main( int argc, char const *argv[] ){
	system("taskkill -f -im sshd.exe");
	string cmd = "\"" + dirname( argv[0] ) + "/sshd.exe\"";
	system( cmd.c_str() );

	while( 1 ) sleep(1);
} 