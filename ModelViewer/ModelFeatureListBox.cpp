// ModelFeatureListBox.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "ModelFeatureListBox.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelFeatureListBox

CModelFeatureListBox::CModelFeatureListBox()
{
}

CModelFeatureListBox::~CModelFeatureListBox()
{
}


BEGIN_MESSAGE_MAP(CModelFeatureListBox, CListBox)
	//{{AFX_MSG_MAP(CModelFeatureListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelFeatureListBox message handlers
#define ITEM_HEIGHT   20

void CModelFeatureListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	RECT *itemRect = &lpDrawItemStruct->rcItem;
	CRect textRect(itemRect->left+3, itemRect->top+2, 
				   itemRect->right-2, itemRect->bottom-2);
	CRect innerFrameRect(itemRect->left+1, itemRect->top+1, 
				   itemRect->right-1, itemRect->bottom-1);
	double rgba[4];
	CString label;

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		GetText(lpDrawItemStruct->itemID, label);
		m_pDoc->GetModelFeatureColor(label.GetBuffer(40), rgba);
		int r = (int) (rgba[0]*255);
		int g = (int) (rgba[1]*255);
		int b = (int) (rgba[2]*255);
		COLORREF cr = RGB(r, g, b);
		// Paint the color item in the color requested
		COLORREF bk = pDC->GetBkColor();
		CBrush br(cr);
		pDC->FillRect(itemRect, &br);
		pDC->SetBkColor(cr);
		pDC->DrawText((LPCTSTR) label, textRect, DT_VCENTER | DT_SINGLELINE);
		pDC->SetBkColor(bk);
	}

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		GetText(lpDrawItemStruct->itemID, label);
		double rgba[4];
		m_pDoc->GetModelFeatureColor(label.GetBuffer(40), rgba);
		int r = (int) (rgba[0]*255);
		int g = (int) (rgba[1]*255);
		int b = (int) (rgba[2]*255);
		COLORREF cr = RGB(r, g, b);
		// item has been selected - hilite frame
		COLORREF crHilite = RGB(255-GetRValue(cr),
						255-GetGValue(cr), 255-GetBValue(cr));
		CBrush br(crHilite);
		pDC->FrameRect(itemRect, &br);
		pDC->FrameRect(innerFrameRect, &br);
	}

	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		GetText(lpDrawItemStruct->itemID, label);
		m_pDoc->GetModelFeatureColor(label.GetBuffer(40), rgba);
		int r = (int) (rgba[0]*255);
		int g = (int) (rgba[1]*255);
		int b = (int) (rgba[2]*255);
		COLORREF cr = RGB(r, g, b);
		CBrush br(cr);
		pDC->FrameRect(itemRect, &br);
		pDC->FrameRect(innerFrameRect, &br);
	}
}

void CModelFeatureListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = ITEM_HEIGHT;
}
