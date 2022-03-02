// DisplaySizeDlg.cpp : implementation file
//

#include "modelviewer.h"
#include "DisplaySizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplaySizeDlg dialog

CDisplaySizeDlg::CDisplaySizeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDisplaySizeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDisplaySizeDlg)
    m_DisplayHeight = 0;
    m_DisplayWidth  = 0;
    //}}AFX_DATA_INIT
}

void CDisplaySizeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDisplaySizeDlg)
    DDX_Text(pDX, IDC_DISPLAY_HEIGHT, m_DisplayHeight);
    DDX_Text(pDX, IDC_DISPLAY_WIDTH, m_DisplayWidth);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDisplaySizeDlg, CDialog)
    //{{AFX_MSG_MAP(CDisplaySizeDlg)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplaySizeDlg message handlers

BOOL CDisplaySizeDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // Inactivates help when F1 is pressed
    // return CDialog::OnHelpInfo(pHelpInfo);
    return 0;
}
