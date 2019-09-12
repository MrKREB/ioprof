#include "ConsoleInterface.h"
#include <algorithm>

ConsoleInterface::ConsoleInterface()
{
}


ConsoleInterface::~ConsoleInterface()
{
}


// Parse command line arguments, estabilish instructions and run comand
void ConsoleInterface::parseCmd(int argc, char* cmd[]) {
	if (argc < 3) {
		printError(wrongArguments);
		return;
	}
	int i = 1;

	while (i < argc) {
		if (!strcmp(cmd[i], "-m")) {
			i++;

			if (i >= argc || cmd[i][0] == '-' ||
				strcmp(cmd[i], "r") && strcmp(cmd[i], "p")) {
				printError(wrongArguments);
				return;
			}

			if (!strcmp(cmd[i], "p"))
				profile = true;
			else if (!strcmp(cmd[i], "r"))
				readFile = true;
		} else if (!strcmp(cmd[i], "-p")) {
			i++;

			if (i >= argc || cmd[i][0] == '-' || readFile) {
				printError(wrongArguments);
				return;
			}

			procName = cmd[i];
		} else if (!strcmp(cmd[i], "-f")) {
			if (profile) {
				saveToFile = true;
			}

			i++;

			if (i < argc && cmd[i][0] != '-') {
				filename = cmd[i];
			} else if (profile) {
				continue;
			} else {
				printError(wrongArguments);
				return;
			}
		} else if (!strcmp(cmd[i], "-n")) {
			i++;

			if (i < argc && cmd[i][0] != '-') {
				topN = atoi(cmd[i]);
			} else {
				topN = 10;
				continue;
			}
		} else if (!strcmp(cmd[i], "-c")) {
			conseal = true;
		} else if (!strcmp(cmd[i], "-h")) {
			printHelp();
			return;
		} else {
			printError(wrongArguments);
			return;
		}

		i++;
	}

	runCmd();
}


// Run command with estabilished instructions
void ConsoleInterface::runCmd() {
	std::vector<PROCESSINFO> spi;
	ProgCodes code;

	if (profile) {
		if (procName == "") {
			code = ioprof.getNtProcessesInfo(&spi);
		}
		else {
			DWORD dwPID = ioprof.findProcessId(procName);
			spi.emplace_back();
			code = ioprof.getNtProcessInfo(dwPID, &spi[0]);
			spi[0].ImageName = procName;
		}
	} else if (readFile) {
		code = worker.readFile(filename, &spi);
	}

	if (code != success) printError(code);
	else {
		if (!conseal) printRes(spi);
		if (saveToFile) worker.writeFile(filename, spi);
	}
}


void ConsoleInterface::printError(ProgCodes err) {
	switch (err) {
	case procNotFound:
		printf("Process not found.\n");
		break;
	case unableToGetInfo:
		printf("Error while getting information.\n");
		break;
	case unableToOpenFIle:
		printf("File not found.\n");
		break;
	case wrongArguments:
		printf("Wrong command.\n");
		printHelp();
		break;
	}
}


// Compare metod for sorting
bool compare(PROCESSINFO arg1, PROCESSINFO arg2) {
	return arg1.AllBytes > arg2.AllBytes;
}


void ConsoleInterface::printRes(std::vector<PROCESSINFO> res) {
	int cycleNum;

	if (topN == 0 || topN > res.size()) cycleNum = res.size();
	else cycleNum = topN;
	std::sort(res.begin(), res.end(), compare);

	printf("  PID             ProcName   AllOps   RdOps   WrOps  OtherOps    AllBytes     RdBytes     WrBytes  OtherBytes\n");

	for (int i = 0; i < cycleNum; i++) {
		printf("%5lu %20s %8llu %7llu %7llu %9llu %11llu %11llu %11llu %11llu\n", res[i].dwPID,
			res[i].ImageName.substr(0, 20).c_str(), res[i].AllOpsCnt, res[i].ioCounters.ReadOperationCount,
			res[i].ioCounters.WriteOperationCount, res[i].ioCounters.OtherOperationCount,
			res[i].AllBytes, res[i].ioCounters.ReadTransferCount,
			res[i].ioCounters.WriteTransferCount, res[i].ioCounters.OtherTransferCount);
	}
}


void ConsoleInterface::printHelp() {
	printf("Command format:\n");
	printf("ioprof -m mode(p/r) [-p proc_name] [-f filename] [-n top_n_lines] [-c] [-h]\n");
	printf("-p: process name\n");
	printf("-m: work mode (profiling/reading file)\n");
	printf("-f: save to file/read from file. By default file name dump.xml\n");
	printf("-n: show top n processes (10 by default)\n");
	printf("-c: don`t show results in command line\n");
	printf("-h: print help\n");
}