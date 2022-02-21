// OverlayBoundsPage.cpp : implementation file
//

#include "modelviewer.h"
#include "OverlayBoundsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayBoundsPage property page

IMPLEMENT_DYNCREATE(COverlayBoundsPage, CPropertyPage)

COverlayBoundsPage::COverlayBoundsPage() : CPropertyPage(COverlayBoundsPage::IDD)
{
	//{{AFX_DATA_INIT(COverlayBoundsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COverlayBoundsPage::~COverlayBoundsPage()
{
}

void COverlayBoundsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COverlayBoundsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COverlayBoundsPage, CPropertyPage)
	//{{AFX_MSG_MAP(COverlayBoundsPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayBoundsPage message handlers


void COverlayBoundsPage::Reinitialize()
{
	GetDlgItem(IDC_OVERLAY_XMIN)->SetWindowText("");
	GetDlgItem(IDC_OVERLAY_YMIN)->SetWindowText("");
	GetDlgItem(IDC_OVERLAY_XMAX)->SetWindowText("");
	GetDlgItem(IDC_OVERLAY_YMAX)->SetWindowText("");
}


void COverlayBoundsPage::SetBounds(double xmin, double xmax, double ymin, double ymax)
{
	char buff[40];
	sprintf(buff, "%g", xmin);
	GetDlgItem(IDC_OVERLAY_XMIN)->SetWindowText(buff);
	sprintf(buff, "%g", xmax);
	GetDlgItem(IDC_OVERLAY_XMAX)->SetWindowText(buff);
	sprintf(buff, "%g", ymin);
	GetDlgItem(IDC_OVERLAY_YMIN)->SetWindowText(buff);
	sprintf(buff, "%g", ymax);
	GetDlgItem(IDC_OVERLAY_YMAX)->SetWindowText(buff);
}

