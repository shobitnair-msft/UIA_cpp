#pragma once
#include<UIAutomation.h>
#include<UIAutomationClient.h>
class UIA {
public:
	IUIAutomation* aut;
	IUIAutomationElement* element;
	HRESULT hr;
	void init();
	void fromCursor();
	void rootWindow();
	void windowFromFocus();
	CONTROLTYPEID controlType(IUIAutomationElement*);
};