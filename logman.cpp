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



int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	ifstream input(argv[1]);
	LogSearch loog;
	loog.read_in(input);
	loog.print_log();
	int count = argc;
	count++;

	string cmd;
	do {
		cout << "% ";
		getline(cin, cmd);
		loog.process_command(cmd);
	} while (cmd[0] != 'q');

}