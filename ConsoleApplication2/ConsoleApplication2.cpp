#include <iostream>
#include<UIAutomation.h>
#include<UIAutomationClient.h>

using namespace std;

IUIAutomation* _automation;
HRESULT InitializeUIAutomation(IUIAutomation** ppAutomation)
{
	return CoCreateInstance(CLSID_CUIAutomation, NULL,
		CLSCTX_INPROC_SERVER, IID_IUIAutomation,
		reinterpret_cast<void**>(ppAutomation));
}

void showHandleFromCursor() {
	IUIAutomationElement* element = NULL;
	POINT pos;
	if (GetCursorPos(&pos)) {
		if (SUCCEEDED(_automation->ElementFromPoint(pos, &element))) {
			BSTR name; element->get_CurrentName(&name);
			UIA_HWND hwnd; element->get_CurrentNativeWindowHandle(&hwnd);
			
			wprintf(L"name: %s\n", name);
			wprintf(L"hwnd: %p\n", hwnd);
		}
	}
	element->Release();
}

void showHandleFromFocus() {
	IUIAutomationElement* element = NULL;
	if (SUCCEEDED(_automation->GetFocusedElement(&element))) {
		BSTR name; element->get_CurrentName(&name);
		UIA_HWND hwnd; element->get_CurrentNativeWindowHandle(&hwnd);

		wprintf(L"name: %s\n", name);
		wprintf(L"hwnd: %p\n", hwnd);
	}
	element->Release();
}



void main() {
	if (SUCCEEDED(CoInitialize(NULL))) {
		while (true) {
			if (SUCCEEDED(InitializeUIAutomation(&_automation))) {
				showHandleFromCursor();
			}
			Sleep(1000);
		}
	}
}