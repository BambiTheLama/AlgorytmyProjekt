#include <Windows.h>
#include <iostream>
#include <fstream>
#include "ErrorHandler.h"

void ErrorHandler::ShowErrorMsgBox(const char* title, const char* description)
{
	MessageBoxA(
		NULL,
		description,
		title,
		MB_ICONERROR
	);
}

void ErrorHandler::LogErrorToFile(const char* fileLocation, const char* message)
{
	time_t systemTime;
	time(&systemTime);

	tm convertedTime;
	localtime_s(&convertedTime, &systemTime);

	std::fstream OutputFile(fileLocation, std::ios_base::app);

	char timeString[20];
	strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &convertedTime);

	OutputFile << "Data: " << timeString << ", tresc bledu: " << message << std::endl;
	std::cout << "Data: " << timeString << ", tresc bledu: " << message << std::endl;

	OutputFile.close();
}