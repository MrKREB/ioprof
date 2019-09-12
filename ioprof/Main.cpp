#include "ConsoleInterface.h"


int main(int argc, char* argv[]) {
	ConsoleInterface consoleInterface;
	consoleInterface.parseCmd(argc, argv);

	system("pause");
	return 0;
}
