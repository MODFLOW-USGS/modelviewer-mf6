// PathlineDataPage.cpp : implementation file
//

#include "modelviewer.h"
#include "PathlineDataPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathlineDataPage property page

IMPLEMENT_DYNCREATE(CPathlineDataPage, CPropertyPage)

CPathlineDataPage::CPathlineDataPage() : CPropertyPage(CPathlineDataPage::IDD)
{
	//{{AFX_DATA_INIT(CPathlineDataPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPathlineDataPage::~CPathlineDataPage()
{
}

void CPathlineDataPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathlineDataPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathlineDataPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPathlineDataPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathlineDataPage message handlers

void CPathlineDataPage::Reinitialize()
{
	GetDlgItem(IDC_MIN)->SetWindowText("");
	GetDlgItem(IDC_MAX)->SetWindowText("");
}

void CPathlineDataPage::SetRange(double *range)
{
	if (range != 0)
	{
		char buff[20];
		sprintf(buff, "%g", range[0]);
		GetDlgItem(IDC_MIN)->SetWindowText(buff);
		sprintf(buff, "%g", range[1]);
		GetDlgItem(IDC_MAX)->SetWindowText(buff);
	}
	else
	{
		GetDlgItem(IDC_MIN)->SetWindowText("");
		GetDlgItem(IDC_MAX)->SetWindowText("");
	}
}
