#include <iostream>
#include<UIAutomation.h>
#include<UIAutomationClient.h>
#include<UIAutomationCore.h>
#include<queue>
#include<vector>
#include <random>
#include<atlbase.h>
using namespace std;

class UIA {
public:
	IUIAutomation* aut;
	IUIAutomationElement* windowElement;
	HRESULT hr;
	queue<IUIAutomationElement*> q;
	vector<IUIAutomationElement*> elements;

	HRESULT InitializeUIAutomation(IUIAutomation** ppAutomation)
	{
		return CoCreateInstance(CLSID_CUIAutomation, NULL,
			CLSCTX_INPROC_SERVER, IID_IUIAutomation,
			reinterpret_cast<void**>(ppAutomation));
	}

	void init() {
		if (SUCCEEDED(CoInitialize(NULL))) {
			hr = InitializeUIAutomation(&(aut));
		}
	};

	VARIANT truevar() {
		VARIANT trueVar;
		trueVar.vt = VT_BOOL;
		trueVar.boolVal = VARIANT_TRUE;
		return trueVar;
	}

	CONTROLTYPEID controlType(IUIAutomationElement* element) {
		CONTROLTYPEID var;
		element->get_CurrentControlType(&var);
		return var;
	}

	BSTR name(IUIAutomationElement* element) {
		BSTR var;
		element->get_CurrentName(&var);
		return var;
	}

	void windowFromFocus() {
		aut->GetFocusedElement(&windowElement);
		IUIAutomationElement* root = NULL;
		IUIAutomationTreeWalker* tree = NULL;

		aut->GetRootElement(&root);
		aut->get_ControlViewWalker(&tree);

		BSTR name; windowElement->get_CurrentName(&name);
		BSTR rname; root->get_CurrentName(&rname);

		while(true) {
			// break if control is window/pane type.
			if (controlType(windowElement) == UIA_WindowControlTypeId)break;
			if (controlType(windowElement) == UIA_PaneControlTypeId)break;
			// break if reached root windowElement
			BSTR wname; windowElement->get_CurrentName(&wname);
			if (rname == wname)break;

			// Else propogate to parent.
			tree->GetParentElement(windowElement, &windowElement);

		}
		BSTR wname; windowElement->get_CurrentName(&wname);
		wcout << wname << endl;
		cout << controlType(windowElement) << endl;
	}

	void editControls_BFS() {
		IUIAutomationCondition* condition;
		aut->CreateTrueCondition(&condition);

		q.push(windowElement);
		while (q.size() > 0) {
			IUIAutomationElement * current = q.front();
			q.pop();
			if (controlType(current) == UIA_EditControlTypeId) {
				elements.push_back(current);
			}
			else {
				IUIAutomationElementArray* children;
				current->FindAll(TreeScope_Children, condition, &children);
				for (int i = 0; ; i++) {
					IUIAutomationElement* child = NULL;
					children->GetElement(i, &child);
					if (child)q.push(child);
					else break;
				}
				current->Release();
			}
		}
	}

};

string random_string(std::size_t length)
{
	const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

	std::string random_string;

	for (std::size_t i = 0; i < length; ++i)
	{
		random_string += CHARACTERS[distribution(generator)];
	}

	return random_string;
}

void main() {
	while (true) {
		cout << "Set focus within 3 sec." << endl;
		Sleep(3000);
		UIA* uia = new UIA;
		uia->init();
		uia->windowFromFocus();
		uia->editControls_BFS();
		string res = random_string(10);
		CComBSTR temp(res.c_str());
		BSTR val = temp.Detach();
		for (int i = 0; i < uia->elements.size(); i++) {
			wcout << i + 1 << " " << uia->name(uia->elements[i]) << endl;
			IUIAutomationValuePattern* pat;
			IUIAutomationLegacyIAccessiblePattern* leg;
			uia->elements[i]->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&pat));
			//uia->elements[i]->GetCurrentPatternAs(UIA_LegacyIAccessiblePatternId, IID_PPV_ARGS(&leg));
			if (pat) {
				pat->SetValue(val);
				pat->Release();
			}
		}
		uia->elements.clear();
		cout << "random value added : " << res << endl;
		cout << endl;
		delete(uia);
	}
}