#pragma once

#include "DirectXBase.h"
#include <stdio.h>
#include <stack>
#include "Noeud.h"
#include <vccorlib.h>


using namespace std;



ref class GenomeRenderer sealed : public DirectXBase
{
public:
	GenomeRenderer();

	// DirectXBase methods.
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);


	// Methods to save and load state in response to suspend.
	void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
	void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	void SetInitialPos(float x, float y);
	void SetGenome(Platform::String^ genom);
	void SetLg(float lgp);
	void SetRandomMode(bool val);
	void SetVarianceMax(int val);
	void SetAngleRef(float val);

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blueBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_redBrush;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_greenBrush;

	Windows::Foundation::Point m_initPosition;
	bool m_renderNeeded;

	CNoeud* m_noeudCrt;
private:
	stack<CNoeud*>*  m_stack;
	int m_nivBranche;
	wstring m_genome;
	float lgBase ;
	bool m_random_mode ;
	int maxRdn;
	float m_angleRef;
};

