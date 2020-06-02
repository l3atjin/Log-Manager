// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <iterator>
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
		else if (l1.numDate == l2.numDate)
		{
			if (l1.cat < l2.cat)
			{
				return true;
			}
			else if (l1.cat == l2.cat)
			{
				return l1.entry < l2.entry;
			}
		}
		return false;
	}
};

class Compare
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

class ComparePTR
{
public:
	bool operator()(Log* l1, Log* l2) const
	{
		if (l1->numDate < l2->numDate)
		{
			return true;
		}
		else if (l1->numDate == l2->numDate)
		{
			if (l1->cat < l2->cat)
			{
				return true;
			}
			else if (l1->cat == l2->cat)
			{
				return l1->entry < l2->entry;
			}
		}
		return false;
	}
};

class LogSearch
{
private:
	vector<Log> masterList;
	//vector<Log*> sortedTS;
	deque<Log*> excerptList;
	deque<Log*> recentList;
	vector<Log>::iterator startRecent;
	vector<Log>::iterator endRecent;
	

public:

	CompareTS dateLess;
	Compare less;
	ComparePTR TSless;
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
			//sortedTS.push_back(&masterList[count]);
			count++;
		}
		sort(masterList.begin(), masterList.end(), less);
	}

	void process_command(string cmd)
	{
		if (cmd[0] == 'q')
		{
			cout << "This command does not produce any output." << '\n';
		}
		else if (cmd[0] == '#')
		{
			cout << "This command does not produce any output." << '\n';
		}
		else if (cmd[0] == 'a')
		{
			int index = stoi(cmd.substr(2));
			excerptList.push_back(&masterList[index]);
			cout << "log entry " << index << " appended" << '\n';
		}
		else if (cmd[0] == 'p')
		{
			print_excerpt();
		}
		else if (cmd[0] == 'c')
		{
			print_excerpt();
		}
		else if (cmd[0] == 'r')
		{
			int count = 0;
			while (startRecent != endRecent)
			{
				excerptList.push_back(&*startRecent);
				startRecent++;
				count++;
			}
			cout << count << " entries appended" << endl;
		}
		else if (cmd[0] == 'd')
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			excerptList.erase(excerptList.begin() + in);
			cout << "Deleted excerpt list entry " << in << endl;
		}
		else if (cmd[0] == 'b')
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			excerptList.push_front(excerptList[in]);
			excerptList.erase(excerptList.begin() + in + 1);
			cout << "Moved excerpt list entry " << in << endl;
		}
		else if (cmd[0] == 'e')
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			excerptList.push_back(excerptList[in]);
			excerptList.erase(excerptList.begin() + in);
			cout << "Moved excerpt list entry " << in << endl;
		}
		else if (cmd[0] == 's')
		{
			cout << "excerpt list sorted" << endl;
			cout << "previous ordering: " << endl;
			cout << 0 << '|' << excerptList[0]->entry << '|' 
				<< excerptList[0]->date << '|' << excerptList[0]->cat
				<< '|' << excerptList[0]->msg << '\n';
			cout << "..." << endl;
			cout << excerptList.size() - 1 << '|' << excerptList[excerptList.size() - 1]->entry << '|'
				<< excerptList[excerptList.size() - 1]->date << '|' << excerptList[excerptList.size() - 1]->cat
				<< '|' << excerptList[0]->msg << '\n';
			sort(excerptList.begin(), excerptList.end(), TSless);
			cout << "new ordering: " << endl;
			cout << 0 << '|' << excerptList[0]->entry << '|'
				<< excerptList[0]->date << '|' << excerptList[0]->cat
				<< '|' << excerptList[0]->msg << '\n';
			cout << "..." << endl;
			cout << excerptList.size() - 1 << '|' << excerptList[excerptList.size() - 1]->entry << '|'
				<< excerptList[excerptList.size() - 1]->date << '|' << excerptList[excerptList.size() - 1]->cat
				<< '|' << excerptList[0]->msg << '\n';
		}
		else if (cmd[0] == 'l')
		{
			cout << "excerpt list cleared" << endl;
			if (excerptList.empty())
			{
				cout << "(previously empty)" << endl;
				return;
			}
			cout << "previous contents:" << endl;
			cout << 0 << '|' << excerptList[0]->entry << '|'
				<< excerptList[0]->date << '|' << excerptList[0]->cat
				<< '|' << excerptList[0]->msg << '\n';
			cout << "..." << endl;
			cout << excerptList.size() - 1 << '|' << excerptList[excerptList.size() - 1]->entry << '|'
				<< excerptList[excerptList.size() - 1]->date << '|' << excerptList[excerptList.size() - 1]->cat
				<< '|' << excerptList[0]->msg << '\n';
			excerptList.clear();
		}
		else if (cmd[0] == 't')
		{
			// maybe fix it
			string str = cmd.substr(2, 2) + cmd.substr(5, 2) + cmd.substr(8, 2) + cmd.substr(11, 2) + cmd.substr(14, 2);
			string str1 = cmd.substr(17, 2) + cmd.substr(20, 2) + cmd.substr(23, 2) + cmd.substr(26, 2) + cmd.substr(29, 2);
			long long num = stoi(str);
			long long num1 = stoi(str1);
			Log temp;
			Log temp1;
			temp.numDate = num;
			temp1.numDate = num1;
			startRecent = lower_bound(masterList.begin(), masterList.end(), temp, less);
			endRecent = lower_bound(masterList.begin(), masterList.end(), temp1, less);
			if (endRecent->numDate == num1)
			{
				endRecent++;
			}
			//cout << startRecent->date << '\n';
			//cout << endRecent->date << '\n';
			cout << "Timestamps search: " << endRecent - startRecent << " entries found" << '\n';
		}
		else if (cmd[0] == 'm')
		{
			string str = cmd.substr(2, 2) + cmd.substr(5, 2) + cmd.substr(8, 2) + cmd.substr(11, 2) + cmd.substr(14, 2);
			long long num = stoi(str);
			Log temp;
			temp.numDate = num;
			startRecent = lower_bound(masterList.begin(), masterList.end(), temp, less);
			if (startRecent->date != str)
			{
				//return;
			}
			endRecent = upper_bound(masterList.begin(), masterList.end(), temp, less);
			
			cout << startRecent->date << '\n';
			cout << endRecent->date << '\n';
			cout << "Timestamp search: " << endRecent - startRecent << " entries found" << '\n';
		}
		else if (cmd[0] == 'g')
		{
			/*vector<Log> temp(startRecent, endRecent);
			sort(temp.begin(), temp.end(), less);
			for (size_t i = 0; i < temp.size(); i++)
			{
				cout << temp[i].entry << '|'
					<< temp[i].date << '|' << temp[i].cat
					<< '|' << temp[i].msg << '\n';
			}*/
			while (startRecent != endRecent)
			{
				cout << startRecent->entry << '|' << startRecent->entry << '|' 
					<< startRecent->date << '|' << startRecent->cat
					<< '|' << startRecent->msg << '\n';
				startRecent++;
			}
		}
	}
	
	void print_log()
	{
		for (size_t i = 0; i < masterList.size(); i++)
		{
			cout << masterList[i].date << masterList[i].cat << masterList[i].msg << '\n';
			//cout << masterList[i].numDate << '\n';
		}
	}

	void print_excerpt()
	{
		for (size_t i = 0; i < excerptList.size(); i++)
		{
			cout << i << '|' << excerptList[i]->entry << '|' << excerptList[i]->date << '|' << excerptList[i]->cat
				<< '|' << excerptList[i]->msg << '\n';
		}
	}

};
