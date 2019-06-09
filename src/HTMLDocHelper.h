// HTMLDocHelper.h: interface for the HTMLDocHelper class.
// This class is helped to traverse HTMLDocument.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTMLDOCHELPER_H__FD9680B3_EB62_4DF7_AB1B_30F7BA185C8D__INCLUDED_)
#define AFX_HTMLDOCHELPER_H__FD9680B3_EB62_4DF7_AB1B_30F7BA185C8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<mshtml.h>
#include<exdisp.h>
#include<atlbase.h>

enum ElementType {
	InputText, InputButton, Anchor
};

class HTMLDocHelper  
{
public:
	HTMLDocHelper(CComQIPtr<IHTMLDocument2> spDoc);
	virtual ~HTMLDocHelper();

	void Handler();

	/* Find HTML Element by Element ID.
	** targetElement: target element id string.
	** targetIndex: return index of the target .
	** startIndex: startIndex to search, if ignored, use 0.
	** endIndex: end index to search, if negative, use the element count instead
	** if found, return the IHTMLElement auto pointer,
	** else return NULL.
	*/
	CComQIPtr<IHTMLElement> FindElementByID(LPCTSTR targetElement,
											int& targetIndex,
											int startIndex = 0,
											int endIndex = -1);

protected:
	HTMLDocHelper();
	CComQIPtr<IHTMLDocument2> _spDoc;

};

#endif // !defined(AFX_HTMLDOCHELPER_H__FD9680B3_EB62_4DF7_AB1B_30F7BA185C8D__INCLUDED_)
