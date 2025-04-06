#include"dpath.h"
#include<bits/stdc++.h>

namespace dag{
namespace tool{
	std::filesystem::path getFileName(const std::source_location& loc){
		return std::filesystem::path(loc.file_name()).filename();
	} 
	std::filesystem::path getFile(const std::source_location& loc){
		return loc.file_name();
	}
	std::filesystem::path getPath(const std::source_location& loc){
		return std::filesystem::path(loc.file_name()).parent_path();
	}
	int getLine(const std::source_location& loc){
		return loc.line();
	}
}
}