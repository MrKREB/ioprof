#pragma once

#include "ioprof.h"
#include "FileWorker.h"

class ConsoleInterface
{
private:
	char* procName = (char*) "";
	char* filename = (char*) "dump.xml";
	int topN = 0;
	bool saveToFile = false;
	bool conseal = false;
	bool readFile = false;
	bool profile = false;
	IoProf ioprof;
	FileWorker worker;

public:
	ConsoleInterface();
	~ConsoleInterface();
	void parseCmd(int argc, char* cmd[]);
	void runCmd();
	void printError(ProgCodes err);
	void printRes(std::vector<PROCESSINFO> res);
	void printHelp();
};

