#include "ioprof.h"


int main(char* procName) {
	IoProf ioprof;
	smPROCESSINFO ppi;
	std::vector<smPROCESSINFO> spi;
	
	printf("%d\n", ioprof.sm_EnableTokenPrivilege("SeDebugPrivilege"));
	DWORD dwPID = ioprof.findProcessId("sqlwriter.exe");
	ioprof.sm_GetNtProcessInfo(dwPID, &ppi);

	printf("PID\tParent\tRdOpsCnt\tWrOpsCnt\tOtherOpsCnt\tRdBytes\t\tWrBytes\t\tOtherBytes\n");
	printf("%lu\t%lu\t%llu\t\t%llu\t\t%llu\t\t%llu\t%llu\t%llu\n", ppi.dwPID, ppi.dwParentPID,
		ppi.ioCounters.ReadOperationCount, ppi.ioCounters.WriteOperationCount,
		ppi.ioCounters.OtherOperationCount,	ppi.ioCounters.ReadTransferCount,
		ppi.ioCounters.WriteTransferCount, ppi.ioCounters.OtherTransferCount);
	printf("ImgPath: %s\nCmdLine: %s\n", ppi.szImgPath, ppi.szCmdLine);


	//spi = ioprof.sm_GetNtProcessesInfo();
	//printf("%u\n\n", spi.size());
	//ppi = spi[27];

	system("pause");
	return 0;
}
