#include "pch.h"
#include "Rules.h"
#include "GenomeGenerator.h"
#include <vector>




Rules* GenomeGenerator::GetRules()
{
	return &m_rules;
}

void GenomeGenerator::AddRule(wstring key, wstring val)
{
	m_rules.AddRule(key,val);
}

 void GenomeGenerator::ResetRules()
{
	 m_rules.Reset();
}








wstring GenomeGenerator::SearchRuleFor(wstring car)
{
	return m_rules.SearchFor(car);
}

wstring GenomeGenerator::Substitute(wstring s)
{
	wstring tmp;
	wstring result;
	int max = s.length();
	
	for (int i=0; i< max;i++)
	{
		wstring recherche;
		recherche += s[i];
		recherche += L"\0";
		tmp = SearchRuleFor (recherche);
		result+=tmp;
	}

	return result;
}



wstring GenomeGenerator::Generate(wstring s, int iterations)
{
	wstring s2;
	s1 = s;

	for (int i=0; i <iterations;i++)
	{
		s2 = Substitute(s1);
		//cout <<i << " : " << s2.length() << endl;
		s1 =s2;
	}

	return s1;
}
