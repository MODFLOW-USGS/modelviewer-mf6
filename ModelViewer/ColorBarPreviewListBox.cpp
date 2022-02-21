// ColorBarPreviewListBox.cpp : implementation file
//

#include "modelviewer.h"
#include "ColorBarPreviewListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarPreviewListBox

CColorBarPreviewListBox::CColorBarPreviewListBox()
{

}

CColorBarPreviewListBox::~CColorBarPreviewListBox()
{
}


BEGIN_MESSAGE_MAP(CColorBarPreviewListBox, CListBox)
	//{{AFX_MSG_MAP(CColorBarPreviewListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarPreviewListBox message handlers

#define ITEM_HEIGHT   20

void CColorBarPreviewListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// Display m_Color in the ListBox.  The ListBox must contain at least one string.

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	RECT *itemRect = &lpDrawItemStruct->rcItem;
	CRect textRect(itemRect->left+3, itemRect->top+2, 
				   itemRect->right-2, itemRect->bottom-2);
	CRect innerFrameRect(itemRect->left+1, itemRect->top+1, 
				   itemRect->right-1, itemRect->bottom-1);

	CBrush br(m_Color);
	pDC->FillRect(itemRect, &br);
}

void CColorBarPreviewListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	lpMeasureItemStruct->itemHeight = ITEM_HEIGHT;
	
}
