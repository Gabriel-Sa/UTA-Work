// Snack Library
#include "SnackLib.h"

bool ParseSnackLine(const std::string& SnackMachineLine, std::vector<std::string>& params)
{
	int x = 0, z = 0, i = 0;

	if (SnackMachineLine.length() == 0)
	{
		return false;
	}
	else
	{
		while (z < SnackMachineLine.length())
		{
			x = SnackMachineLine.find("|", z) - z;
			params[i++] = SnackMachineLine.substr(z, x);
			z += params[i-1].length() + 1;
		}
	}
	return true;
}