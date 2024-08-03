#include "Assert.h"
#include <cassert>
#include <Windows.h>
#include <iostream>

void lost::lassert(std::string text)
{
	std::cout << "\n=[Error!]=====================================================================================================\n\n >>> " << text << "\n\n=[Error!]=====================================================================================================\n" << std::endl;
	std::string error = "=[Error!]==================================\n" + text + "\n=[Error!]==================================";

	std::wstring wStrErrorText = std::wstring(error.begin(), error.end());

	MessageBox(NULL, wStrErrorText.c_str(), L"Error!", MB_ICONERROR | MB_OK);
	assert(false);
}