#pragma once








class GenomeGenerator
{
public:
	GenomeGenerator(){};
	wstring Generate(wstring s, int iterations);
	Rules* GetRules();
	void ResetRules();
	void AddRule(wstring key, wstring val);

private:
	wstring Substitute(wstring s);
	wstring SearchRuleFor(wstring car);
	wstring s1;
	Rules m_rules;

public:
	~GenomeGenerator(void){};
};

