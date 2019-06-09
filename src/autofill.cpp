#include<windows.h>
#include<stdio.h>
#include<mshtml.h>
#include<exdisp.h>
#include<atlbase.h>
#include "HTMLDocHelper.h"
#include "KeyboardSimulator.h"
//#include<oleacc.h>
#include<comutil.h>
#include<string.h>
#include<time.h>
#pragma comment(lib, "comsupp.lib")

#define EleID_MAX 100

inline void ensureLoad(CComQIPtr<IWebBrowser2> spBrowser)
{
	VARIANT_BOOL isBusy;
	do{
		HRESULT hr = spBrowser->get_Busy(&isBusy);
		if(FAILED(hr)) {
			printf("Is Busy error\n");
			return;
		}
	}while(isBusy == VARIANT_TRUE);
}

void _setfocus(CComQIPtr<IHTMLDocument2> spDoc) 
{
	HTMLDocHelper *dhElement = new HTMLDocHelper(spDoc);
	int targetIndex;
	CComQIPtr<IHTMLElement2> spEle = dhElement ->FindElementByID("tbCaptcha",
													targetIndex
													);
		if(!spEle) {
			printf("No This ID: tbCaptcha!\n");
		}
		if(HRESULT hr = FAILED(spEle -> focus())) {
			printf("No focus,Error code: %d\n",hr);
		}
	delete dhElement;

}

/*Check whether Element in Document is disable.
	if still disable, do nothing, return false;
	else click the element, return true;
*/
bool checkElementEnabled(CComQIPtr<IWebBrowser2> spBrowser)
{
	ensureLoad(spBrowser);
	HWND hwndCMBRush;
	HRESULT hr = spBrowser->get_HWND((long*)(&hwndCMBRush));
	if(FAILED(hr)) {
		printf("Can not get HWND\n");
		return false;
	}
	if(::IsIconic(hwndCMBRush)) {
		::ShowWindow(hwndCMBRush, SW_SHOWMAXIMIZED);
	}
	else
		::SetForegroundWindow(hwndCMBRush);
//	if(::IsIconic(hwndCMBRush)) {
//	}
	//	::BringWindowToTop(hwndCMBRush);
//	::SetForegroundWindow( (HWND)((DWORD)hwndCMBRush | 0x01));
//	::SetForegroundWindow(hwndCMBRush);
//	::SetFocus(hwndCMBRush);
	CComPtr<IDispatch> spDisp;
	//Get IDispatch for each Document
	hr = spBrowser -> get_Document(&spDisp);
	if(FAILED(hr)) {
		printf("Can not get IDispatch for each Document\n");
		return false;
	}
	//Get IHTMLDocument2 for each Document
	CComQIPtr<IHTMLDocument2> spDoc = spDisp;
	if(!spDoc) {
		printf("Can not get IHTMLDocument2 for each Document\n");
		return false;
	}
	KeyboardSimulator *kbSim = KeyboardSimulator::getInstance();
	FILE* chFile = fopen("chars.txt", "r");
	if(chFile == NULL) {
		printf("Can not Open this character file!\n");
		return false;
	}
	TCHAR targetElement[EleID_MAX];
	int startIndex = 0;
	while(fscanf(chFile, "%s%*c",targetElement) != EOF) {
		HTMLDocHelper *dhElement = new HTMLDocHelper(spDoc);
		int targetIndex;
	//	TCHAR targetElement[] = "tbPasswordCtl"; //"clock addcartbtn btnQualify tbCardNoCtl tbPasswordCtl";
		CComQIPtr<IHTMLElement2> spEle = dhElement ->FindElementByID(targetElement,
													targetIndex,
													startIndex);
		if(!spEle) {
			printf("No This ID: %s!\n", targetElement);
			continue;
		}
		startIndex = targetIndex;
		if(hr = FAILED(spEle -> focus())) {
			printf("No focus,Error code: %d\n",hr);
		}
		
		if(kbSim ->getCharsFromFile(chFile))
			kbSim ->setChars();
		delete dhElement;
	}
	delete kbSim;
	fclose( chFile );
	HTMLDocHelper *dhElement = new HTMLDocHelper(spDoc);
	int targetIndex;
	CComQIPtr<IHTMLElement2> spEle = dhElement ->FindElementByID("tbCaptcha",
													targetIndex
													);
		if(!spEle) {
			printf("No This ID: tbCaptcha!\n");
		}else if(hr = FAILED(spEle -> focus())) {
			printf("No focus,Error code: %d\n",hr);
		}
	delete dhElement;

//	_setfocus(spDoc);

	return false;

	//Handler the target Element!!!
/*	CComQIPtr<IHTMLInputButtonElement> spBtn = spEle;
	if(!spBtn) {
		printf("Is not a Input Button!\n");
		return true;
	}
	BSTR bstrValue;
		//Get Location URL.
	hr = spBtn->get_value(&bstrValue);
	LPTSTR strValue = _com_util::ConvertBSTRToString(bstrValue);
		::SysFreeString(bstrValue);
		printf("%s\n", strValue);
	VARIANT_BOOL isDisabled;
	//Set refresh interval
	const double interval = 5000;
	clock_t startTime = clock();
	do
	{
		spBtn->get_disabled(&isDisabled);
		clock_t curTime = clock();
		if(curTime - startTime >= interval) {
			delete dhElement;
			return false;
		}
	}while(isDisabled == VARIANT_TRUE);
	spEle->click();
	delete dhElement;
	return true;
*/
//	exit(1);
	//::SetActiveWindow(tmp);
/*	if(!spBtn) {
		printf("IMPOSSIBLE, Can change to Input Button!!!!!\n");
	}
	
	printf("%d\n",t);
	*/	
/*	for(int i = 0; i <= 10; i++)
	{
		
		BSTR bstrValue;
		//Get Location URL.
		hr = spBtn->get_value(&bstrValue);
		if(FAILED(hr)) {
			printf("Can not get Value for for spBtn\n");
			return;
		}
		LPTSTR strValue = _com_util::ConvertBSTRToString(bstrValue);
		::SysFreeString(bstrValue);
		printf("%s\n", strValue);
		Sleep(1000);
		
	}
	system("pause");
*/
	/*TBC FindElement*/
}

/* Travesal Function called back by EnumChildWindows
	if find the "Internet Explorer_Server", return false to terminate travesal.
	else return true to continue searching.
*/
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[100];
	TCHAR tstr[] = _T("Internet Explorer_Server");
	if ( _tcscmp( buf, tstr ) == 0 )
	{
		*(HWND*)lParam = hwnd;
		return FALSE;
	}
	else
		return TRUE;
};

void processCMBRushBrs(CComQIPtr<IWebBrowser2> spBrowser)
{
	// Time extends interval, should fresh the page.
	while(!checkElementEnabled(spBrowser)) {
		spBrowser ->Refresh();
	}
/*	TCHAR targetURL[] = "https://ccclub.cmbchina.com//ccclub/Purchase/Pay.aspx";
	CComVariant nullVar;

	// Navigate to a new URL.
	HRESULT hr = spBrowser ->Navigate(_com_util::ConvertStringToBSTR(targetURL), 
		&nullVar,
		&nullVar,
		&nullVar,
		&nullVar
		);
	if(hr == DISP_E_BADVARTYPE) {
		printf("Bad variable type!\n");
		return ;
	}
	if(FAILED(hr)) {
		printf("Can not navigate to %s!, Error code is %lld\n",targetURL, hr);
		return ;
	}
	ensureLoad(spBrowser);

	// Set the browser in front and get focus.
	HWND hwndCMBRush;
	hr = spBrowser->get_HWND((long*)(&hwndCMBRush));
	if(FAILED(hr)) {
		printf("Can not get HWND\n");
		return ;
	}
	::SetForegroundWindow(hwndCMBRush);
	HWND hwndChild = NULL;
	::EnumChildWindows( hwndCMBRush, EnumChildProc, (LPARAM)&hwndChild );
	::SetFocus(hwndChild);
	KeyboardSimulator *kbSim = KeyboardSimulator::getInstance();
	kbSim ->getCharsFromFile("chars.txt");
	kbSim ->setChars();
	delete kbSim;
*/
//	::Sleep(1000);
}

void setF5(CComQIPtr<IWebBrowser2> spBrowser) {
	HWND hwndCMBRush;
	HRESULT hr = spBrowser->get_HWND((long*)(&hwndCMBRush));
	if(FAILED(hr)) {
		printf("Can not get HWND\n");
		return;
	}
	if(::IsIconic(hwndCMBRush)) {
		::ShowWindow(hwndCMBRush, SW_SHOWMAXIMIZED);
	}
	else
		::SetForegroundWindow(hwndCMBRush);

	KeyboardSimulator *kbSim = KeyboardSimulator::getInstance();
	kbSim ->setChars(VK_F5);
	delete kbSim;
}

void process(TCHAR* targetURL)
{
	CComPtr<IShellWindows> spShellWin;
	//Can only get the IE window, including windows explores.
	HRESULT hr = spShellWin.CoCreateInstance(CLSID_ShellWindows);
	if(FAILED(hr)) {
		printf("Can not get ShellWin Handlers. Error code: %d!\n", hr);
		exit(1);
	}
	long nCount = 0;
	spShellWin -> get_Count(&nCount);
	printf("Total windows: %d\n",nCount);
//	system("pause");
	for (long i = 0; i < nCount; i ++)
	{
		CComPtr<IDispatch> spDisp;
		//Get IDispatch for each window
		hr = spShellWin ->Item(CComVariant(i), &spDisp);
		if(FAILED(hr)) {
			printf("Can not get Single ShellWin Handler %d!\n", i);
			continue;
		}
		//Get IWebBrowser2 for each window
		CComQIPtr<IWebBrowser2> spBrowser = spDisp;
		if ( !spBrowser ){
			printf("The %d window is not IWebBrowser2\n",i);
			continue;
		}
		//Ensure to Free this BSTR.
		BSTR bstrTitle;
		//Get Location URL.
		hr = spBrowser ->get_LocationURL(&bstrTitle);
		if(FAILED(hr)) {
			printf("Can not get LocationURL for for spBrowser\n");
			return;
		}
		//Ensure to Free this LPTSTR.
		LPTSTR strTitle = _com_util::ConvertBSTRToString(bstrTitle);
		::SysFreeString(bstrTitle);
		printf("spBrowser Name: %s\n",strTitle);
//		system("pause");
		if(strcmp(strTitle,targetURL) == 0) {
//			processCMBRushBrs(spBrowser);	
//			spBrowser ->Refresh();
//			setF5(spBrowser);
			checkElementEnabled(spBrowser);
			return ;
		}
		delete[] strTitle;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Initialize COM, to ensure Windows loading DLLs.
	::CoInitialize(NULL);
	TCHAR targetURL[] = "https://ccclub.cmbchina.com//ccclub/Purchase/Pay.aspx";
	//"http://ccclub.cmbchina.com/ccclub/productinfo.aspx?gid=2009101654382";
	//"file:///Y:/qiangdui/CMBRush/t.html"
	//"https://ccclub.cmbchina.com//ccclub/Purchase/Pay.aspx"
	process(targetURL);
	::CoUninitialize();

	return 0;
}