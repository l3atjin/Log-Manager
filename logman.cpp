// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "LogSearch.h"

using namespace std;


bool checkInput(string str, size_t in, bool didSearch)
{
	if (str[0] != 't' && str[0] != 'm' && str[0] != 'c' && str[0] != 'k'
		&& str[0] != 'a' && str[0] != 'r' && str[0] != 'd' && str[0] != 'b' 
		&& str[0] != 'e' && str[0] != 's' && str[0] != 'l' && str[0] != 'g' 
		&& str[0] != 'p' && str[0] != 'q' && str[0] != '#')
	{
		return false;
	}
	else if (str[0] == 't')
	{
		if (str.size() != 31)
		{
			return false;
		}
	}
	else if (str[0] == 'a' || str[0] == 'd' || str[0] == 'b' || str[0] == 'e')
	{
		int temp1 = static_cast<int>(in);
		if (stoi(str.substr(2)) > temp1 - 1 || stoi(str.substr(2)) < 0)
		{
			return false;
		}
	}
	else if (str[0] == 'g' || str[0] == 'r')
	{
		if (!didSearch)
		{
			return false;
		}
	}
	return true;
}


int main(int argc, char *argv[])
{
	//std::ios_base::sync_with_stdio(false);

	ifstream input(argv[1]);
	LogSearch loog;
	loog.read_in(input);
	//loog.print_log();
	int count = argc;
	count++;

	string cmd;
	do {
		cout << "% ";
		getline(cin, cmd);
		if (!checkInput(cmd, loog.logSize, loog.searched))
		{
			cerr << "Invalid input. Try again." << endl;
			continue;
		}
		loog.process_command(cmd);
	} while (cmd[0] != 'q');

}