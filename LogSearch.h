// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <getopt.h>

using namespace std;



struct Log
{
	string date;
	string cat;
	string msg;
	int entry;
	long long numDate;
};


class CompareTS
{
public:
	bool operator()(const Log &l1, const Log &l2) const
	{
		if (l1.numDate < l2.numDate)
		{
			return true;
		}
		return false;
	}
};

class LogSearch
{
private:
	vector<Log> masterList;
	vector<Log*> sortedTS;
	deque<Log*> excerptList;
	deque<Log*> recentList;
	

public:

	CompareTS dateLess;
	LogSearch() {}

	void read_in(ifstream &input)
	{
		string ts, cat, msg;
		int count = 0;
		while (getline(input, ts, '|'))
		{
			getline(input, cat, '|');
			getline(input, msg);
			Log temp;
			temp.date = ts;
			string str = ts.substr(0, 2) + ts.substr(3, 2) + ts.substr(6, 2) + ts.substr(9, 2) + ts.substr(12, 2);
			temp.numDate = stoi(str);
			temp.cat = cat;
			temp.msg = msg;
			temp.entry = count;
			masterList.push_back(temp);
			sortedTS.push_back(&masterList[count]);
			count++;
		}
		sort(masterList.begin(), masterList.end(), dateLess);
	}

	void process_command(string cmd)
	{
		if (cmd[0] == 'q')
		{
			cout << "This command does not produce any output." << endl;
		}
		else if (cmd[0] == '#')
		{
			cout << "This command does not produce any output." << endl;
		}
		else if (cmd[0] == 'a')
		{
			int index = stoi(cmd.substr(2));
			excerptList.push_back(&masterList[index]);
			cout << "log entry " << index << " appended" << endl;
		}
		else if (cmd[0] == 'p')
		{
			print_excerpt();
		}
		else if (cmd[0] == 't')
		{
			// maybe fix it
			string str = cmd.substr(2, 2) + cmd.substr(5, 2) + cmd.substr(8, 2) + cmd.substr(11, 2) + cmd.substr(14, 2);
			string str1 = cmd.substr(17, 2) + cmd.substr(20, 2) + cmd.substr(23, 2) + cmd.substr(26, 2) + cmd.substr(29, 2);
			cout << str << endl;
			cout << str1 << endl;
			long long num = stoi(str);
			long long num1 = stoi(str1);
			cout << num << endl;
			cout << num1 << endl;
			Log temp;
			Log temp1;
			//temp->numDate = num;
			// can use iter
			temp1.numDate = num1;
			auto start = lower_bound(sortedTS.begin(), sortedTS.end(), temp, dateLess);
			auto end = lower_bound(sortedTS.begin(), sortedTS.end(), temp1, dateLess);
			cout << "Timestamps search: " << end - start << " entries found" << endl;
		}
	}
	asdfasdfasd

	void print_log()
	{
		for (size_t i = 0; i < sortedTS.size(); i++)
		{
			cout << sortedTS[i]->date << sortedTS[i]->cat << sortedTS[i]->msg << endl;
			//cout << masterList[i].numDate << endl;
		}
	}

	void print_excerpt()
	{
		for (size_t i = 0; i < excerptList.size(); i++)
		{
			cout << i << '|' << excerptList[i]->entry << '|' << excerptList[i]->date << '|' << excerptList[i]->cat
				<< '|' << excerptList[i]->msg << endl;
		}
	}

};
