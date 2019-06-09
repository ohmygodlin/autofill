// HTMLDocHelper.cpp: implementation of the HTMLDocHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "HTMLDocHelper.h"
#include<stdio.h>
#include<comutil.h>
#include<string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HTMLDocHelper::HTMLDocHelper()
{

}

HTMLDocHelper::~HTMLDocHelper()
{

}

HTMLDocHelper::HTMLDocHelper(CComQIPtr<IHTMLDocument2> spDoc)
{
	if(!spDoc) {
		printf("NULL CComQIPtr pointer for IHTMLDocument2!\n");
		exit(1);
	}
	_spDoc = spDoc;
}

CComQIPtr<IHTMLElement> HTMLDocHelper::FindElementByID(LPCTSTR targetElement,
													   int& targetIndex,
													   int startIndex,
													   int endIndex)
{
	targetIndex = -1;
	CComQIPtr<IHTMLElement> spEle;
	if(!_spDoc) 
		return spEle;
	CComQIPtr<IHTMLElementCollection> spEleCol;
	//Get Element Collection
	HRESULT hr = _spDoc ->get_all(&spEleCol);
	if(FAILED(hr)) {
		printf("Can get element collection!\n");
		return spEle;
	}
	if(!spEleCol) {
		printf("No Element Collection!\n");
		return spEle;
	}
	long nLength = 0;
	spEleCol ->get_length(&nLength);
	printf("Total Elements: %d\n",nLength);
	nLength = (endIndex < 0) ? nLength : endIndex;
	for(int i = startIndex; i < nLength; i ++)
	{
		CComPtr<IDispatch> spDisp;
		//Get IDispatch for each Element
		hr = spEleCol ->item(CComVariant(i) , CComVariant(i) , &spDisp);
		printf("Number %d Element: ", i);
		if(FAILED(hr)) {
			printf("Fail to get IDispatch\n");
			continue;
		}
		//Get IHTMLElement for each Element
		spEle = spDisp;
		if(!spEle) {
			printf("Fail to get IHTMLElement\n");
			continue;
		}
		//Ensure to Free this BSTR
		BSTR bstrID;
		//Get ID for each element
		spEle ->get_id(&bstrID);
		if(FAILED(hr)) {
			printf("Can not get ID for this element\n");
			continue;
		}
		LPTSTR strID = _com_util::ConvertBSTRToString(bstrID);  
		::SysFreeString(bstrID);
		printf("strID: %s\n",strID);
		if(strID != NULL && strcmp(strID,targetElement) == 0) {
			delete[] strID;
			targetIndex = i;
			return spEle;
		}
		//Ensure to Free this LPTSTR.
		delete[] strID;
	}
	return spEle;
}