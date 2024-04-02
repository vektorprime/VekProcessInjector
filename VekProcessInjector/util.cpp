#include "util.h"

std::wstring convertToWstring(std::string s)
{
	std::wstring ws(s.begin(), s.end());
	return ws;
}