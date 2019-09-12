#pragma once

#include "pugixml.hpp"
#include "ntdllapi.h"

class FileWorker
{
public:
	FileWorker();
	~FileWorker();
	void writeFile(char* filename, std::vector<PROCESSINFO> arr);
	ProgCodes readFile(char* filename, std::vector<PROCESSINFO> *spi);
};

