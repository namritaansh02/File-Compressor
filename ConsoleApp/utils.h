#pragma once
#include <string>

using namespace std;

string intToString7(int a) {
	string ans = "";
	for (int i = 0; i < 7; i++) {
		if (a % 2 == 0) ans = '0' + ans;
		else ans = '1' + ans;
		a /= 2;
	}
	return ans;
}