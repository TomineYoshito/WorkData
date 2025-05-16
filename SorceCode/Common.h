#pragma once
#include <vector>
#include <string>

using namespace std;

class Common
{
public:
	Common() {}
	~Common() {}

	static vector<string> split(string str, char del);
};
