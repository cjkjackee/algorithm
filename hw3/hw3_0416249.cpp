#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//variable
vector<vector<int> > table;

class day
{
	public:
		day(){};
		~day(){};
		int total();
		void show();
		day operator+(int);
		day operator= (vector<int>);
		day operator= (day);
	private:
		vector<int> c;
		int ans;
};

int day::total()
{
	return this->ans;
}

void day::show()
{
	for (int i=0;i<this->c.size();++i)
		cout << table[this->c[i]][i] << " ";
	cout << endl;
}

day day::operator+(int n)
{
	day ret = *this;
	int ans;

	for (unsigned int x=0;x<this->c.size();++x)
	{
		ans = 0;
		for (unsigned int i=0;i<this->c.size();++i)
		{
			if (i == x)
			{
				if (c[i]+n >= table.size())
					continue;
				ans += table[c[i]+n][i];
			}
			else 
				ans += table[c[i]][i];
		}

		if (ans >= ret.ans)
		{
			ret = *this;
			ret.c[x] = this->c[x]+n;
			ret.ans = ans;
		}
	}
	return ret;
}

day day::operator=(vector<int> num)
{
	this->c = num;
	this->ans = 0;
	for (unsigned int i=0;i<this->c.size();++i)
		this->ans += table[this->c[i]][i];
	
	return *this;
}

day day::operator=(day n2)
{
	this->c = n2.c;
	this->ans = n2.ans;
	return *this;
}

//variable
map<int,day> work;

//function
day cal(int);

int main ()
{
	day init, max;
	fstream fin, fout;
	stringstream ss;
	string s;
	vector<int> tmp;
	int n;
	bool trig = false;

	fin.open("input.txt",fstream::in);
	while(getline(fin,s))
	{
		tmp.clear();

		if (s[s.length()-1] == 13)
			s.pop_back();
		
		if (s.empty())
			continue;

		ss.clear();
		ss << s;

		while(ss >> n)
		{
			tmp.push_back(n);
			if (tmp.size() > 1)
			{
				trig = true;
				table.clear();
				work.clear();
			}
		}

		if (trig)
		{
			table.push_back(tmp);

			while(getline(fin,s))
			{
				if (s[s.length()-1] == 13)
					s.pop_back();
				
				if (s.empty())
					break;

				tmp.clear();
				ss.clear();
				ss << s;

				while(ss >> n)
					tmp.push_back(n);
				table.push_back(tmp);
			}
			
			tmp.clear();
			for (unsigned int i=0;i<table[0].size();++i)
				tmp.push_back(0);

			work[table[0].size()] = tmp;

			trig = false;
		}
		else
		{
			max = cal(tmp[0]);
			cout << max.total() << endl;
		}
	}
	return 0;
}

day cal(int n)
{
	map<int,day>::iterator it;

	it = work.find(n);
	if (it!=work.end())
		return work[n];
	else
	{
		work[n] = cal(n-1)+1;
		return work[n];
	}
}
