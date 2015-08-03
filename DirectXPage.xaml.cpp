//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include "GenomeGenerator.h"
#include <stdlib.h>
#include <string>
#include <locale>
#include <codecvt>


using namespace GraphGenome;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Popups;
using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;





		

DirectXPage::DirectXPage() :
	m_renderNeeded(true)
{
	InitializeComponent();

	m_renderer = ref new GenomeRenderer();

	
	//init mini du generateur 
	m_generator.AddRule(L"A",L"AA");
	m_generator.AddRule(L"B",L"BA[BA][CA]");
	m_generator.AddRule(L"C",L"CA[CA][BA]");
	m_generator.AddRule(L"[",L"[");
	m_generator.AddRule(L"]",L"]");
	
	txt_genome2->Text = ref new String( m_generator.Generate(L"A[BA][CA]A[CA][BA]",4).data() );
 

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		SwapChainPanel,
		DisplayProperties::LogicalDpi
		);

	
	SetInitialPos( Window::Current->Bounds.Width/2,Window::Current->Bounds.Height -40);


	Window::Current->CoreWindow->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	DisplayProperties::OrientationChanged +=
        ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnOrientationChanged);

	DisplayProperties::DisplayContentsInvalidated +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnDisplayContentsInvalidated);
	
	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &DirectXPage::OnRendering));

	m_timer = ref new BasicTimer();
}

void DirectXPage::SetInitialPos(float x, float y)
{
	m_renderer->SetInitialPos(x,y);
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ args)
{
	auto currentPoint = args->GetCurrentPoint(nullptr);

	if (currentPoint->Properties->IsRightButtonPressed)
		return;

	SetInitialPos(currentPoint->Position.X,currentPoint->Position.Y);


	GetDataFromUI();
	m_renderNeeded = true;
}

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();
	m_renderNeeded = true;
}

void DirectXPage::OnLogicalDpiChanged(Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
	m_renderNeeded = true;
}

void DirectXPage::OnOrientationChanged(Object^ sender)
{
	m_renderer->UpdateForWindowSizeChange();
	m_renderNeeded = true;
}

void DirectXPage::OnDisplayContentsInvalidated(Object^ sender)
{
	m_renderer->ValidateDevice();
	m_renderNeeded = true;
}

void DirectXPage::OnRendering(Object^ sender, Object^ args)
{
	if (m_renderNeeded)
	{
		m_timer->Update();
		m_renderer->Update(m_timer->Total, m_timer->Delta);
		m_renderer->Render();
		m_renderer->Present();
		m_renderNeeded = false;
	}
}


void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	m_renderer->SaveInternalState(state);
}

void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	m_renderer->LoadInternalState(state);
}




void DirectXPage::ReadRules(String^filename)
{
	m_generator.ResetRules();
	
	create_task(KnownFolders::DocumentsLibrary->CreateFileAsync(filename, CreationCollisionOption::OpenIfExists)).then([this](StorageFile^ file)
		{
	
			create_task(FileIO::ReadLinesAsync(file)).then([this](Windows::Foundation::Collections::IVector<Platform::String^>^ lines)
			{
			
				for (unsigned int i =0; i < lines->Size; i++)
				{
					m_generator.GetRules()->AddRule(lines->GetAt(i)->Data());
				}
			
			});
			
		});
}
void DirectXPage::SaveRules(String^filename)
	{
		create_task(KnownFolders::DocumentsLibrary->CreateFileAsync(filename, CreationCollisionOption::ReplaceExisting)).then([this](StorageFile^ file)
		{
			Vector<String^> lstRules;
			for (int i=0; i< m_generator.GetRules()->GetSize();i++)
			{
				String ^s =  ref new String(m_generator.GetRules()->GetRuleNum(i).data());
				lstRules.Append(s);
			}
			Windows::Foundation::Collections::IIterable<String^>^  itr;
			itr = lstRules.GetView();
			FileIO::AppendLinesAsync(file,itr);
		});
}
void DirectXPage::LoadRulesFile()
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
    openPicker->FileTypeFilter->Append(".txt");
    
    auto v = create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
    {
        if (file)
        {
            txtRulesFileName->Text = file->Name;
			ReadRules(file->Name);
			GetDataFromUI();
			txt_genome2->Text= ref new String( m_generator.Generate(txt_Genome->Text->Data(),3).data() );
			m_renderNeeded = true;	
		}
        else
        {
            txtRulesFileName->Text =  "File operation cancelled.";
        }
    });	
}


void DirectXPage::GetDataFromUI()
{
	String^ s = txt_nbiterations->Text;
	wstring ws(s->Data()); 
	int nbiteration = _wtoi(ws.c_str());

	s = txt_lg->Text;
	ws = s->Data();
	float longueur =(float) _wtof(ws.c_str());

	String^ s3 = txt_rdnMax->Text;
	ws =  s3->Data(); 
	int rdnmax = _wtoi(ws.c_str());

	String^ s4 = txt_angleRef->Text;
	ws =  s4->Data(); 
	float angleref = (float) _wtof(ws.c_str());

	
	this->m_renderer->SetLg(longueur);
	this->m_renderer->SetGenome(txt_genome2->Text);
	this->m_renderer->SetVarianceMax(rdnmax);
	this->m_renderer->SetAngleRef(angleref);
	this->m_renderer->SetRandomMode(chkRandomMode->IsChecked->Value );

	if (chk_rendergenome->IsChecked->Value == false)
	{	
		txt_genome2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}else
	{
		txt_genome2->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
		
}

void DirectXPage::OnTestPressed(Object^ sender, RoutedEventArgs^ args)
{
	String^ s = txt_nbiterations->Text;
	wstring ws(s->Data()); 
	int n = _wtoi(ws.c_str());
	 s = txt_Genome->Text;
	 ws = m_generator.Generate(s->Data(),n);
	txt_genome2->Text = ref new String(ws.data());
	GetDataFromUI();

	m_renderNeeded = true;
}
void DirectXPage::OnPreviousGeneration(Object^ sender, RoutedEventArgs^ args)
{
	wstring s = txt_nbiterations->Text->Data();
	int x = _wtoi(s.c_str());
	x--;
	txt_nbiterations->Text = x.ToString();	
	txt_genome2->Text = ref new String(m_generator.Generate(txt_Genome->Text->Data(),x).data() );
	GetDataFromUI();
	m_renderNeeded = true;
}
void DirectXPage::OnNextGeneration(Object^ sender, RoutedEventArgs^ args)
{
	wstring s = txt_nbiterations->Text->Data();
	int x = _wtoi(s.c_str());
	x++;
	txt_nbiterations->Text = x.ToString();
	if (x >=8)
	{
		MessageDialog^ msg = ref new MessageDialog("Ca fait beaucoup d'itérations...");
		msg->ShowAsync();
	}
	txt_genome2->Text = ref new String(m_generator.Generate(txt_Genome->Text->Data(),x).data() );
	GetDataFromUI();
	
	m_renderNeeded = true;
}
void DirectXPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	LoadRulesFile();	
	
}
