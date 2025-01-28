#include "lz78.h"

int main(int argc, char** argv){
	{
		int n = 1;
		if(*(char *)&n != 1) {
			std::cout << "Currently big endian systems are not supported" << std::endl;
			return 0;
		}
	}
	
	#ifdef DEBUG
	tests();
	#endif
	
	std::string in_filename = "";
	std::string out_filename = "";
	
	compression_dir dir = compress;
	
	if (argc < 2){
		std::cout << "No input, try --help" << std::endl;
		
		return 0;
	}
	
	try {
		int o = 0, d = 0;
		for (int i = 1; i < argc; i++){
			if (o == 1){
				if (*argv[i] == '-'){
					throw argv[i];
				}
				out_filename = argv[i];
				o++;
			}
			else if (strncmp(argv[i], "-o", 2) == 0) {
				o++;
				if (o != 1){
					throw argv[i];
				}
			}
			else if (strncmp(argv[i], "-d", 2) == 0) {
				d++;
				if (d != 1){
					throw argv[i];
				}
				dir = decompress;
			}
			else if (strncmp(argv[i], "--help", 6) == 0 || strncmp(argv[i], "-h", 2) == 0){
				std::cout << 
					"How to use\n\
					Give a filename to compress/decompress and change some options with flags\n\
					listed below! By default the program is in compressing mode.\n\
					Options\n\
					\t-d decompress the given file\n\
					\t-o set the name of the output (the name comes right after -o)\n\
					\t-h --help show how to\n";
			}
			else {
				if (in_filename != ""){
					throw argv[i];
				}
				in_filename = argv[i];
			}
		}
	}
	catch (const char* const err) {
		std::cout << "Can't parse arguments, try --help. Don't know what to do with \"" << err << "\"." << std::endl;
		return 0;
	}
	
	if (in_filename == ""){
		std::cout << "No input file given, try --help." << std::endl;
		return 0;
	}
	
	std::ifstream in_file(in_filename, std::ios::binary);
	if (!in_file.is_open()){
		std::cout << "Input not found!" << std::endl;
		return 0;
	}
	
	if (out_filename == ""){
		if (dir == compress) out_filename = in_filename + ".lz78";
		else if (strncmp(in_filename.c_str() + in_filename.size() - 5, ".lz78", 5) == 0){
			out_filename = in_filename;
			out_filename.erase(out_filename.size() - 5);
		}
		else {
			out_filename = "out_" + in_filename;
		}
	}
	
	#ifdef DEBUG
	std::cout << in_filename << " " << out_filename << "\n";
	#endif
	
	std::ofstream out_file(out_filename, std::ios::binary);
	if (!out_file.is_open()){
		std::cout << "Can't make output file: \"" << out_filename << "\"" << std::endl;
		return 0;
	}
	
	if (dir == compress){
		compressor::compress(in_file, out_file);
	}
	
	in_file.close();
	out_file.close();
	
	return 0;
}