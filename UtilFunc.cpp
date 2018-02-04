#include "UtilFunc.h"

#include <sstream>
#include <unordered_set>
using namespace std;

string intSetToStr(const unordered_set<int>& set){
	ostringstream strOut;
	strOut << "( ";
	for(auto it = set.begin(); it!=set.end(); ++it) {
		strOut << (*it) << " ";
	}
	strOut << ")";
	return strOut.str();
}

string itos(int n) {
	ostringstream ss;
	ss << n;
	return ss.str();
}
