// VectorDataPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "VectorDataPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorDataPage property page

IMPLEMENT_DYNCREATE(CVectorDataPage, CPropertyPage)

CVectorDataPage::CVectorDataPage() : CPropertyPage(CVectorDataPage::IDD)
{
	//{{AFX_DATA_INIT(CVectorDataPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CVectorDataPage::~CVectorDataPage()
{
}

void CVectorDataPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVectorDataPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVectorDataPage, CPropertyPage)
	//{{AFX_MSG_MAP(CVectorDataPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorDataPage message handlers

void CVectorDataPage::Reinitialize()
{
	GetDlgItem(IDC_MIN)->SetWindowText("");
	GetDlgItem(IDC_MAX)->SetWindowText("");
}

void CVectorDataPage::SetRange(double *range)
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
