#include "Header.h"

void main() {
	UIA* uia = new UIA;
	uia->init();
	while (true) {
		if (SUCCEEDED(uia->hr)) {
			uia->windowFromFocus();
		}
		Sleep(1500);
	}
}