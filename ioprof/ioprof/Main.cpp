#include "ioprof.h"


int main(int argc, char* argv[]) {
	IoProf ioprof;
	PROCESSINFO ppi;
	std::vector<PROCESSINFO> spi;
	
	if (argc < 2) {
		printf("Not enough arguments\n");
		system("pause");
		return 0;
	}

	DWORD dwPID = ioprof.findProcessId(argv[1]);
	ioprof.getNtProcessInfo(dwPID, &ppi);

	printf("PID\tParent\tRdOpsCnt\tWrOpsCnt\tOtherOpsCnt\tRdBytes\t\tWrBytes\t\tOtherBytes\n");
	printf("%lu\t%lu\t%llu\t\t%llu\t\t%llu\t\t%llu\t%llu\t%llu\n", ppi.dwPID, ppi.dwParentPID,
		ppi.ioCounters.ReadOperationCount, ppi.ioCounters.WriteOperationCount,
		ppi.ioCounters.OtherOperationCount,	ppi.ioCounters.ReadTransferCount,
		ppi.ioCounters.WriteTransferCount, ppi.ioCounters.OtherTransferCount);
	printf("ImgPath: %s\nCmdLine: %s\n", ppi.szImgPath, ppi.szCmdLine);

	system("pause");
	return 0;
}
