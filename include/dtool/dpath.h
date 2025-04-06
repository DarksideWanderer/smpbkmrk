#pragma once

#include<bits/stdc++.h>

namespace dag{
namespace tool{
	std::filesystem::path getFileName(const std::source_location& loc = std::source_location::current());
	std::filesystem::path getFile(const std::source_location& loc = std::source_location::current());
	std::filesystem::path getPath(const std::source_location& loc = std::source_location::current());
	int getLine(const std::source_location& loc = std::source_location::current());
}
}