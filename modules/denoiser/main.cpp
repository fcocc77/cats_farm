#include "denoiser.h"

int main(int argc, char const *argv[]){

	string project = argv[1];
	string exe = argv[2];
	int pr = stoi( argv[3] );
	int sr = stoi( argv[4] );
	float vr = stof( argv[5] );
	int tr = stoi( argv[6] );
	int first_frame = stoi( argv[7] );
	int last_frame = stoi( argv[8] );
	
	for (int i = first_frame; i <= last_frame; ++i )
		denoiser( project, exe, pr, sr, vr, tr, i );
	
}