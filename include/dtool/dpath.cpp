#include"dpath.h"
#include<bits/stdc++.h>

namespace dag{
namespace tool{

	std::string getFileName(const std::source_location& loc){
		return std::filesystem::path(loc.file_name()).filename().string();
	}
	std::string getFile(const std::source_location& loc){
		return loc.file_name();
	}
	int getLine(const std::source_location& loc){
		return loc.line();
	}

}
}