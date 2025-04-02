#pragma once

#include<bits/stdc++.h>

namespace dag{
namespace tool{

	std::string getFileName(const std::source_location& loc = std::source_location::current());
	std::string getFile(const std::source_location& loc = std::source_location::current());
	int getLine(const std::source_location& loc = std::source_location::current());
}
}