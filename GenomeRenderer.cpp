#include "pch.h"
#include "Rules.h"
#include "GenomeRenderer.h"
#include "GenomeGenerator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <codecvt>
#include <string.h>


using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

#define PI 3.14159


int GetRdnValue(int max)
{
	bool flagneg =false;
	if (max <0)
	{
		max*=-1;
		flagneg=true;
	}

	int x = (rand() % max + 1);

	if (flagneg==true)
		return x*-1;
	else
	return x ;
}


void GenomeRenderer::SetGenome(Platform::String^ genom)
{
	m_genome = genom->Data();
}

void GenomeRenderer::SetInitialPos(float x, float y)
{
	m_initPosition.X = x;
	m_initPosition.Y = y;
}

void GenomeRenderer::SetVarianceMax(int val)
{
	maxRdn = val;
}
void GenomeRenderer::SetAngleRef(float val)
{
	m_angleRef = val;
}

GenomeRenderer::GenomeRenderer() :
	m_renderNeeded(true),
	m_initPosition(100,100),
	maxRdn(60)
	{
	

		srand ((unsigned int) time(NULL) );
		m_noeudCrt =  new CNoeud(400,400);
		m_noeudCrt->dirCrt-=90;
		m_nivBranche=1;
		m_stack =  new stack<CNoeud*>();
		lgBase= .8f;
	
		m_angleRef = 30;
	
	}

void GenomeRenderer::SetLg(float lgp)
{
	lgBase = lgp;
}

void GenomeRenderer::SetRandomMode(bool val)
{
	m_random_mode = val;
}

void GenomeRenderer::Render()
{

	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(ColorF(D2D1::ColorF::Wheat));

	m_noeudCrt->posX = m_initPosition.X;
	m_noeudCrt->posY = m_initPosition.Y;
	m_noeudCrt->dirCrt = -90;
	m_nivBranche=1;
	
	float posX ,posY, lg ;
	int x=0;
	D2D1_POINT_2F p ;
				
	
	for (long i=0; i< (long)m_genome.length(); i++)
	{
		switch (m_genome[i])
		{
		case 'A':
			{
				lg= (float) pow(1.2,m_nivBranche) * lgBase;
				float delta = (float)GetRdnValue(maxRdn);
				
				
				//si dans les premiers = un peu plus droit
				if (i <= 2)
					delta = delta/3;
				if (m_random_mode == false)
					delta=0;

				//epaisseur
				float ep = 10.f-m_nivBranche*2;
				//si n+1 est ] alors on reduit
				if (m_genome[i+1]==']')
					ep=1;
				
				posX = m_noeudCrt->GetX() + ((float)cos((m_noeudCrt->GetDir()+delta)* (2.0*PI)/360) * lg);
				posY = m_noeudCrt->GetY() + ((float)sin((m_noeudCrt->GetDir()+delta)* (2.0*PI)/360) * lg);
				p.x = posX;
				p.y = posY;
				m_d2dContext->DrawLine(m_noeudCrt->GetPoint(),p,m_blackBrush.Get(),ep);
				m_noeudCrt->posX = posX;
				m_noeudCrt->posY = posY;
				break;
			}

		case 'B':
			{
				if (m_random_mode == true)
					x = GetRdnValue(1);

				if  (x ==0)
					m_noeudCrt->dirCrt+=m_angleRef;
				else
					m_noeudCrt->dirCrt+=GetRdnValue(maxRdn)*1.f;	
				
				break;
				
				//Feuilles
				/*
				lg= (float)pow(1.2,m_nivBranche) * lgBase;
				D2D1_ELLIPSE ellips;
				ellips.point.x = m_noeudCrt->posX;
				ellips.point.y = m_noeudCrt->posY;
				ellips.radiusX = lg/10;
				ellips.radiusY = lg*2/10;
				m_d2dContext->FillEllipse(&ellips,m_blueBrush.Get());
				ellips.radiusX = lg*2;
				ellips.radiusY = lg;
				m_d2dContext->FillEllipse(&ellips,m_greenBrush.Get());
				break;
				*/
				
			}
		case 'C':
			{
				if (m_random_mode == true)
					x = GetRdnValue(1);
				
				if  (x ==0)
					m_noeudCrt->dirCrt-=m_angleRef;
				else
					m_noeudCrt->dirCrt-=GetRdnValue(maxRdn)*1.f;	
				break;
			}

		case 'D':
			{
				if (m_random_mode == true)
					x = GetRdnValue(1);
				
				if  (x ==0)
					m_noeudCrt->dirCrt+=m_angleRef/3;
				else
					m_noeudCrt->dirCrt+=GetRdnValue(10)*1.f;	
				break;
			}
		case 'E':
			{
				if (m_random_mode == true)
					x = GetRdnValue(1);
				
				if  (x ==0)
					m_noeudCrt->dirCrt-=m_angleRef/3;
				else
					m_noeudCrt->dirCrt-=GetRdnValue(10)*1.f;	
				break;
			}

		case '[':
			m_nivBranche++;
			m_stack->push(new CNoeud(m_noeudCrt));
			break;

		case ']':
			m_nivBranche--;
			m_noeudCrt = m_stack->top();
			m_stack->pop();
			break;

		default:
			break;
		}
	
	}

	// Ignore D2DERR_RECREATE_TARGET. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	m_renderNeeded = false;
}

void GenomeRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void GenomeRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Black),
			&m_blackBrush
			)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Blue),
			&m_blueBrush
			)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Red),
			&m_redBrush
			)
		);

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(
			ColorF(ColorF::Green),
			&m_greenBrush
			)
		);

	
}

void GenomeRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	// Add code to create window size dependent objects here.
}

void GenomeRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeTotal; // Unused parameter.
	(void) timeDelta; // Unused parameter.

	// Add code to update time dependent objects here.
}

void GenomeRenderer::SaveInternalState(IPropertySet^ state)
{
	
}

void GenomeRenderer::LoadInternalState(IPropertySet^ state)
{
	
}
