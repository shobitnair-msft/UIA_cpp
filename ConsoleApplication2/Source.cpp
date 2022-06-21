#include "Header.h"
#include <iostream>
#include<UIAutomation.h>
#include<UIAutomationClient.h>
#include<UIAutomationCore.h>
using namespace std;


string toString(BSTR x) {
	wstring z = x;
	string z2 = string(z.begin(), z.end());
	return z2;
}

HRESULT InitializeUIAutomation(IUIAutomation** ppAutomation)
{
	return CoCreateInstance(CLSID_CUIAutomation, NULL,
		CLSCTX_INPROC_SERVER, IID_IUIAutomation,
		reinterpret_cast<void**>(ppAutomation));
}


void UIA::init() {
	if (SUCCEEDED(CoInitialize(NULL))) {
		hr = InitializeUIAutomation(&(aut));
	}
};

void UIA::fromCursor() {
	POINT pos;
	if (GetCursorPos(&pos)) {
		if (SUCCEEDED(aut->ElementFromPoint(pos, &element))) {
			VARIANT v;
			element->GetCurrentPropertyValue(UIA_NamePropertyId, &v);
			BSTR name; element->get_CurrentName(&name);
			UIA_HWND hwnd; element->get_CurrentNativeWindowHandle(&hwnd);
			cout << toString(name) << endl;
			wprintf(L"name: %s\n", name);
			wprintf(L"hwnd: %p\n", hwnd);
		}
	}
	element->Release();
}

void UIA::rootWindow() {
	if (SUCCEEDED(aut->GetRootElement(&element))) {
		BSTR name; element->get_CurrentName(&name);
		UIA_HWND hwnd; element->get_CurrentNativeWindowHandle(&hwnd);
		wprintf(L"name: %s\n", name);
		wprintf(L"hwnd: %p\n", hwnd);
	}
	element->Release();
}
VARIANT truevar() {
	VARIANT trueVar; 
	trueVar.vt = VT_BOOL;
	trueVar.boolVal = VARIANT_TRUE;
	return trueVar;
}

CONTROLTYPEID UIA::controlType(IUIAutomationElement* element) {
	CONTROLTYPEID var;
	element->get_CurrentControlType(&var);
	return var;
}

void UIA::windowFromFocus() {
	aut->GetFocusedElement(&element);
	IUIAutomationElement* root = NULL;
	IUIAutomationTreeWalker* tree = NULL;

	aut->GetRootElement(&root);
	aut->get_ControlViewWalker(&tree);

	BSTR name; element->get_CurrentName(&name);
	BSTR rname; root->get_CurrentName(&rname);

	// Get first parent which has window control type.
	IUIAutomationElement* windowElement = element;

	while (true) {
		// break if control is window type.
		CONTROLTYPEID ctrl; windowElement->get_CurrentControlType(&ctrl);
		if (ctrl == UIA_WindowControlTypeId)break;

		// break if reached root element
		BSTR wname; windowElement->get_CurrentName(&wname);
		if (rname == wname)break;

		// Else propogate to parent.
		tree->GetParentElement(windowElement, &windowElement);

	}
	BSTR wname; windowElement->get_CurrentName(&wname);
	wcout << wname << endl;
	element->Release();
}


