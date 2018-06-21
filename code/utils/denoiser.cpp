#include "denoiser.h"

string denoiser( string input, string exe, int pr, int sr, float vr, int tr, int frame ){

	int _fr = tr;
	vector <string> inputs;

	// agrega los frames anteriores
	int _frame = frame ;
	for (int i = 0; i < tr; ++i){
		_frame = _frame -1;

		string frame_path = getFrame( input, _frame );
		if ( frame_path == "none" ) _fr += 1;
		else inputs.push_back( frame_path );
	}
	reverse( inputs.begin(), inputs.end() ); 
	//-----------------------------------------

	// agrega los frames posteriores
	int _frame2 = frame ;
	for (int i = 0; i < _fr; ++i){
		_frame2 = _frame2 + 1;

		string frame_path = getFrame( input, _frame2 );
		if ( not ( frame_path == "none" ) ) 
			inputs.push_back( frame_path );
	}
	//------------------------------------------

	string inputs_list;
	for ( auto i : inputs ) inputs_list += " -i \"" + i + "\"";
	//---------------------------------------------------

	string first_frame = getFrame( input, frame );

	// tranform input to output
	string dirname = os::dirname(first_frame);
	string basename = os::basename(first_frame);
	string ext = split( basename, "." ).back();
	string pad_ext = split( basename, "_" ).back();
	string basename_without = replace( basename, pad_ext, "" );
	//-------------------------
	string output = dirname + "/" + basename_without + "noice_" + pad_ext;
	//-------------------------------------------------------

	string parameter = " -pr " + to_string(pr) + " -sr " + to_string(sr) + " -v " + to_string(vr);

	string cmd = "\"" + exe + "\"" + parameter +" -i \"" + first_frame + "\"" + inputs_list + " -o \"" + output + "\"";

	return os::sh(cmd);
}

string getFrame( string path, int frame ){

	string dirname = os::dirname(path);
	string basename = os::basename(path);

	string ext = split( basename, "." ).back();
	string pad_ext = split( basename, "_" ).back();
	string basename_without = replace( basename, pad_ext, "");

	string cero = "";
	string _frame;
	for (int i = 0; i < 10; ++i){

		_frame = dirname + "/" + basename_without + cero + to_string(frame) + "." + ext;
		cero += "0";

		if ( os::isfile(_frame) ) return _frame;

	}

	return "none";
}
