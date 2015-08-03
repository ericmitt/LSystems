#pragma once
#include <string>
using namespace std;

class RuleItem
{
public:
	wstring left;
	wstring right;
public:
	RuleItem(wstring,wstring);
	wstring ToString();
};

class Rules
{
public:
	Rules();
	void AddRule(wstring ruleline);
	void AddRule(wstring k, wstring val);
	wstring SearchFor(wstring val);
	void Reset();
	void Edit(int i,wstring k, wstring val);
	int GetSize();
	wstring GetRuleNum(int x);
	

private:
	std::vector<RuleItem> m_rules;
	
};

