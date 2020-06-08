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
#include <unordered_map>
#include <stdio.h>
#include <ctype.h>
#include <sstream>
#include <set>
#include <iterator>

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
		return l1.numDate < l2.numDate;
	}
};

class CompareMaster
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
private:
	// make the vector a reference
	vector<Log> &vec;
public:

	Compare(vector<Log> &vec_in)
		: vec(vec_in) {}

	bool operator()(int lhs, int rhs) const
	{
		if (vec[lhs].numDate < vec[rhs].numDate)
		{
			return true;
		}
		else if (vec[lhs].numDate == vec[rhs].numDate)
		{
			if (vec[lhs].cat < vec[rhs].cat)
			{
				return true;
			}
			else if (vec[lhs].cat == vec[rhs].cat)
			{
				return vec[lhs].entry < vec[rhs].entry;
			}
		}
		return false;
	}
};

class CompareIndex
{
private:
	// make the vector a reference
	vector<Log> &vec;
public:
	CompareIndex(vector<Log> &vec_in)
		: vec(vec_in) {}

	bool operator()(int lhs, long long rhs) const
	{
		if (vec[lhs].numDate < rhs)
		{
			return true;
		}
		return false;
	}
	bool operator()(long long rhs, int lhs) const
	{
		if (vec[lhs].numDate < rhs)
		{
			return true;
		}
		return false;
	}
};


class LogSearch
{
private:
	unordered_map<string, vector<size_t>> searchCat;
	unordered_map<string, vector<size_t>> searchKey;
	vector<Log> masterList;
	vector<size_t> indexList;
	deque<size_t> excerptList;
	vector<size_t> resultKey;
	vector<Log>::iterator startRecent;
	vector<Log>::iterator endRecent;
	vector<size_t>::iterator startCK;
	vector<size_t>::iterator endCK;
	
	bool isCat = false;
	bool isKey = false;
	bool didCK = false;
	
public:
	size_t logSize;
	bool searched = false;
	CompareTS dateLess;
	CompareMaster compFull;
	LogSearch() {}

	string lowercase(string str)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (isalnum(str[i]))
			{
				str[i] = char(tolower(str[i]));
			}
		}
		return str;
	}

	string parse(string &str)
	{
		string temp = str;
		str = "";
		bool spaced = true;
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (isalnum(temp[i]))
			{
				temp[i] = char(tolower(temp[i]));
				str += temp[i];
				spaced = false;
			}
			else if (!spaced)
			{
				str += ' ';
				spaced = true;
			}
		}
		return str;
	}

	void read_in(ifstream &input)
	{
		string ts, cat, msg;
		int count = 0;
		while (getline(input, ts, '|')) // O(n)
		{
			getline(input, cat, '|');
			getline(input, msg);
			Log temp;
			temp.date = ts;
			string str = ts.substr(0, 2) + ts.substr(3, 2) + ts.substr(6, 2) + ts.substr(9, 2) + ts.substr(12, 2);
			temp.numDate = stoll(str);
			temp.cat = cat;
			temp.msg = msg;
			temp.entry = count;
			masterList.push_back(temp);
			count++;
		}
		cout << count << " entries read" << '\n';
		sort(masterList.begin(), masterList.end(), compFull); // O(nlogn)
		// prolly inefficient
		logSize = masterList.size();
		indexList.resize(masterList.size());
		for (size_t i = 0; i < masterList.size(); i++) // O(n)
		{
			indexList[masterList[i].entry] = i;
		}
	}

	void process_command(string cmd)
	{
		if (cmd[0] == 'a') // O(1)
		{
			int index = stoi(cmd.substr(2));
			excerptList.push_back(indexList[index]);
			cout << "log entry " << index << " appended" << '\n';
		}
		else if (cmd[0] == 'p') // O(n)
		{
			print_excerpt();
		} 
		else if (cmd[0] == 'c') // need hash
		{
			if (!isCat)
			{
				for (size_t i = 0; i < masterList.size(); i++)
				{
					searchCat[lowercase(masterList[i].cat)].push_back(i);
				}
				isCat = true;
			}
			string cat = cmd.substr(2);
			auto it = searchCat.find(lowercase(cat));
			if (it == searchCat.end())
			{
				vector<size_t> hmm;
				startCK = hmm.begin();
				endCK = hmm.end();
				didCK = true;
				cout << "Category search: " << 0 << " entries found" << '\n';
				return;
			}
			startCK = it->second.begin();
			endCK = it->second.end();
			didCK = true;
			searched = true;
			cout << "Category search: " << it->second.size() << " entries found" << '\n';
			
		}
		else if (cmd[0] == 'k') // need hash
		{
			if (!isKey)
			{
				for (size_t i = 0; i < masterList.size(); i++)
				{
					//cout << masterList[i].cat << '\n';
					stringstream s1(masterList[i].cat);
					string word1;
					while (s1 >> word1)
					{
						//cout << "hi" << '\n';
						parse(word1);
						stringstream s2(word1);
						string keyword;
						while (s2 >> keyword)
						{
							if (searchKey[keyword].empty())
							{
								searchKey[keyword].push_back(i);
							}
							else if (searchKey[keyword].back() != i)
							{
								searchKey[keyword].push_back(i);
							}
						}
					}
					stringstream s(masterList[i].msg);
					string word;
					//cout << masterList[i].msg << '\n';
					while (s >> word)
					{
						parse(word);
						string keyword;
						stringstream s2(word);
						while (s2 >> keyword)
						{
							if (searchKey[keyword].empty())
							{
								searchKey[keyword].push_back(i);
							}
							else if (searchKey[keyword].back() != i)
							{
								searchKey[keyword].push_back(i);
							}
						}
					}
					//cout << '\n';
				}
				isKey = true;
			}
			searched = true;
			vector<string> input;
			string key = cmd.substr(2);
			//cout << key << '\n';
			stringstream q(key);
			string word;
			while (q >> word)
			{
				//cout << word << '\n';
				parse(word);
				//cout << word << '\n';
				stringstream s2(word);
				string keyword;
				while (s2 >> keyword)
				{
					//cout << keyword << '\n';
					if (searchKey.find(keyword) == searchKey.end())
					{
						vector<size_t> hmm;
						startCK = hmm.begin();
						endCK = hmm.end();
						didCK = true;
						cout << "Keyword search: 0 entries found" << '\n';
						return;
					}
					input.push_back(keyword);
				}
			}
			if (input.size() == 1)
			{
				cout << "Keyword search: " << searchKey[input.front()].size() << " entries found" << '\n';
				startCK = searchKey[input.front()].begin();
				endCK = searchKey[input.front()].end();
				didCK = true;
				return;
			}
			vector<size_t> temp(searchKey[input[0]].begin(), searchKey[input[0]].end());
			
			//set_intersection(searchKey[input[0]].begin(), searchKey[input[0]].end(), searchKey[input[1]].begin(), searchKey[input[1]].end(), back_inserter(result));
			for (size_t i = 1; i < input.size(); i++)
			{
				resultKey.clear();
				set_intersection(temp.begin(), temp.end(), searchKey[input[i]].begin(), searchKey[input[i]].end(), back_inserter(resultKey));
				temp = resultKey;
			}
			startCK = resultKey.begin();
			endCK = resultKey.end();
			didCK = true;
			cout << "Keyword search: " << resultKey.size() << " entries found" << '\n';
		}
		else if (cmd[0] == 'r') // O(n)
		{
			if (searched)
			{
				if (!didCK)
				{
					int count = 0;
					auto start = startRecent;
					while (start != endRecent)
					{
						excerptList.push_back(indexList[start->entry]);
						start++;
						count++;
					}
					cout << count << " log entries appended" << '\n';
				}
				else
				{
					int count = 0;
					auto start = startCK;
					while (start != endCK)
					{
						excerptList.push_back(*start);
						start++;
						count++;
					}
					cout << count << " log entries appended" << '\n';
				}
			}
		}
		else if (cmd[0] == 'd') // O(n)
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			int size = static_cast<int>(excerptList.size());
			if (in >= 0 || in < size)
			{
				excerptList.erase(excerptList.begin() + in);
				cout << "Deleted excerpt list entry " << in << '\n';
			}
			else
			{
				cerr << "Invalid input. Try again." << endl;
				return;
			}
		}
		else if (cmd[0] == 'b') // O(n)
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			int size = static_cast<int>(excerptList.size());
			if (in >= 0 || in < size)
			{
				excerptList.push_front(excerptList[in]);
				excerptList.erase(excerptList.begin() + in + 1);
				cout << "Moved excerpt list entry " << in << '\n';
			}
			else
			{
				cerr << "Invalid input. Try again." << endl;
				return;
			}
		}
		else if (cmd[0] == 'e') // O(n)
		{
			string str = cmd.substr(2);
			int in = stoi(str);
			int size = static_cast<int>(excerptList.size());
			if (in >= 0 || in < size)
			{
				excerptList.push_back(excerptList[in]);
				excerptList.erase(excerptList.begin() + in);
				cout << "Moved excerpt list entry " << in << '\n';
			}
			else
			{
				cerr << "Invalid input. Try again." << endl;
				return;
			}
		}
		else if (cmd[0] == 's') // O(nlogn)
		{
			if (!excerptList.empty())
			{
				Compare comp(masterList);
				cout << "excerpt list sorted" << '\n';
				cout << "previous ordering:" << '\n';
				cout << 0 << '|' << masterList[excerptList[0]].entry << '|' << masterList[excerptList[0]].date << '|' << masterList[excerptList[0]].cat
					<< '|' << masterList[excerptList[0]].msg << '\n';
				cout << "..." << '\n';
				cout << excerptList.size() - 1 << '|' << masterList[excerptList[excerptList.size() - 1]].entry << '|'
					<< masterList[excerptList[excerptList.size() - 1]].date << '|' << masterList[excerptList[excerptList.size() - 1]].cat
					<< '|' << masterList[excerptList[excerptList.size() - 1]].msg << '\n';
				sort(excerptList.begin(), excerptList.end(), comp);
				cout << "new ordering:" << '\n';
				cout << 0 << '|' << masterList[excerptList[0]].entry << '|' << masterList[excerptList[0]].date << '|' << masterList[excerptList[0]].cat
					<< '|' << masterList[excerptList[0]].msg << '\n';
				cout << "..." << '\n';
				cout << excerptList.size() - 1 << '|' << masterList[excerptList[excerptList.size() - 1]].entry << '|'
					<< masterList[excerptList[excerptList.size() - 1]].date << '|' << masterList[excerptList[excerptList.size() - 1]].cat
					<< '|' << masterList[excerptList[excerptList.size() - 1]].msg << '\n';
			}
			else
			{
				cout << "excerpt list sorted" << '\n';
				cout << "(previously empty)" << '\n';
			}
			
		}
		else if (cmd[0] == 'l')  // O(1)
		{
			cout << "excerpt list cleared" << '\n';
			if (excerptList.empty())
			{
				cout << "(previously empty)" << '\n';
				return;
			}
			cout << "previous contents:" << '\n';
			cout << 0 << '|' << masterList[excerptList[0]].entry << '|' << masterList[excerptList[0]].date << '|' << masterList[excerptList[0]].cat
				<< '|' << masterList[excerptList[0]].msg << '\n';
			cout << "..." << '\n';
			cout << excerptList.size() - 1 << '|' << masterList[excerptList[excerptList.size() - 1]].entry << '|'
				<< masterList[excerptList[excerptList.size() - 1]].date << '|' << masterList[excerptList[excerptList.size() - 1]].cat
				<< '|' << masterList[excerptList[excerptList.size() - 1]].msg << '\n';
			excerptList.clear();
		}
		else if (cmd[0] == 't') // O(logn)
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
			startRecent = lower_bound(masterList.begin(), masterList.end(), temp, dateLess);
			endRecent = upper_bound(masterList.begin(), masterList.end(), temp1, dateLess);
			if (endRecent->numDate == num1)
			{
				endRecent++;
			}
			//cout << startRecent->date << '\n';
			//cout << endRecent->date << '\n';
			didCK = false;
			searched = true;
			cout << "Timestamps search: " << endRecent - startRecent << " entries found" << '\n';
		}
		else if (cmd[0] == 'm') // O(logn)
		{
			string str = cmd.substr(2, 2) + cmd.substr(5, 2) + cmd.substr(8, 2) + cmd.substr(11, 2) + cmd.substr(14, 2);
			long long num = stoll(str);
			Log temp;
			temp.numDate = num;
			startRecent = lower_bound(masterList.begin(), masterList.end(), temp, dateLess);
			if (startRecent->date != str)
			{
				//return;
			}
			endRecent = upper_bound(masterList.begin(), masterList.end(), temp, dateLess);
			didCK = false;
			searched = true;
			cout << "Timestamp search: " << endRecent - startRecent << " entries found" << '\n';
		}
		else if (cmd[0] == 'g') // o(n)
		{
		if (searched)
		{
			if (!didCK)
			{
				auto start = startRecent;
				auto end = endRecent;
				while (start != end)
				{
					cout << start->entry << '|'
						<< start->date << '|' << start->cat
						<< '|' << start->msg << '\n';
					start++;
				}
			}
			else
			{
				auto start = startCK;
				auto end = endCK;
				while (start != end)
				{
					cout << masterList[*start].entry << '|'
						<< masterList[*start].date << '|' << masterList[*start].cat
						<< '|' << masterList[*start].msg << '\n';
					start++;
				}
			}
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
			cout << i << '|' << masterList[excerptList[i]].entry << '|' << masterList[excerptList[i]].date << '|' << masterList[excerptList[i]].cat
				<< '|' << masterList[excerptList[i]].msg << '\n';
		}
	}

};
