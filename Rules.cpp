#include "pch.h"
#include <string>
#include "Rules.h"



RuleItem::RuleItem(wstring l,wstring r)
{
	left = l;
	right = r;
}

wstring RuleItem::ToString()
{
	return (left + L":" + right);	
}

//=============================================================================


Rules::Rules()
{
	
}

void Rules::AddRule(wstring k, wstring val)
{
	m_rules.push_back(RuleItem(k,val));
}

void Rules::AddRule(wstring ruleline)
{
	wstring key,val;
	int cpt = ruleline.size()-1;
	val =ruleline.substr(2,cpt);
	key =ruleline.substr(0,1);

	AddRule(key,val);
	}

void Rules::Edit(int i,wstring k, wstring val)
{
	m_rules[i].left = k;
	m_rules[i].right = val;
}
wstring Rules::SearchFor(wstring val)
{
	
	wstring ret = val;
	
	for (int i=0; i<  m_rules.size();i++)
	{
		if (m_rules[i].left == val)
			ret = m_rules[i].right;
	}

	return ret;
}

void Rules::Reset()
{
	m_rules.clear();
}

int Rules::GetSize()
{
	return m_rules.size();
}
wstring Rules::GetRuleNum(int x)
{
	return (m_rules[x].left + L":" + m_rules[x].right);	
}



