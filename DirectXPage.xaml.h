//
// BlankPage.xaml.h
// Declaration of the BlankPage.xaml class.
//

#pragma once

#include "DirectXPage.g.h"
#include "Rules.h"
#include "GenomeRenderer.h"
#include "GenomeGenerator.h"
#include "BasicTimer.h"
#include <collection.h>


using namespace Platform;
using namespace Platform::Collections;
namespace GraphGenome
{
	/// <summary>
	/// A DirectX page that can be used on its own.  Note that it may not be used within a Frame.
	/// </summary>
    [Windows::Foundation::Metadata::WebHostHidden]
	public ref class DirectXPage sealed
	{
	public:
		DirectXPage();

		void OnPreviousGeneration(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnNextGeneration(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

		

	private:
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnLogicalDpiChanged(Platform::Object^ sender);
		void OnOrientationChanged(Platform::Object^ sender);
		void OnDisplayContentsInvalidated(Platform::Object^ sender);
		void OnRendering(Object^ sender, Object^ args);
		void OnTestPressed(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void SetInitialPos(float x, float y);
		void GetDataFromUI();

		void SaveRules(String^filename);
		void ReadRules(String^filename);
		void LoadRulesFile();

		Windows::Foundation::EventRegistrationToken m_eventToken;
		BasicTimer^ m_timer;
		bool m_renderNeeded;

		GenomeRenderer^ m_renderer;
		GenomeGenerator m_generator;
		
	};
}
