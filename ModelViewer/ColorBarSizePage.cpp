// ColorBarSizePage.cpp : implementation file
//

#include "modelviewer.h"
#include "ColorBarSizePage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarSizePage property page

IMPLEMENT_DYNCREATE(CColorBarSizePage, CPropertyPage)

CColorBarSizePage::CColorBarSizePage()
    : CPropertyPage(CColorBarSizePage::IDD)
{
    //{{AFX_DATA_INIT(CColorBarSizePage)
    //}}AFX_DATA_INIT
    m_ExchangeData = FALSE;
    m_Height       = 0;
    m_Offset       = 0;
    m_Width        = 0;
}

CColorBarSizePage::~CColorBarSizePage()
{
}

void CColorBarSizePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorBarSizePage)
    //}}AFX_DATA_MAP

    if (m_ExchangeData)
    {
        DDX_Text(pDX, IDC_HEIGHT, m_Height);
        DDX_Text(pDX, IDC_OFFSET, m_Offset);
        DDX_Text(pDX, IDC_WIDTH, m_Width);
    }
}

BEGIN_MESSAGE_MAP(CColorBarSizePage, CPropertyPage)
    //{{AFX_MSG_MAP(CColorBarSizePage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarSizePage message handlers

BOOL CColorBarSizePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBarSizePage::Reinitialize()
{
    GetDlgItem(IDC_HEIGHT)->SetWindowText("");
    GetDlgItem(IDC_WIDTH)->SetWindowText("");
    GetDlgItem(IDC_OFFSET)->SetWindowText("");
    Activate(FALSE);
}

void CColorBarSizePage::Activate(BOOL b)
{
    GetDlgItem(IDC_HEIGHT)->EnableWindow(b);
    GetDlgItem(IDC_WIDTH)->EnableWindow(b);
    GetDlgItem(IDC_OFFSET)->EnableWindow(b);
}

BOOL CColorBarSizePage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CColorBarSizePage::OnDefault()
{
    m_Width  = 20;
    m_Height = 200;
    m_Offset = 100;
    CustomUpdateData(FALSE);
    m_pDoc->SetColorBarSize(m_Width, m_Height, m_Offset);
}

void CColorBarSizePage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        m_pDoc->SetColorBarSize(m_Width, m_Height, m_Offset);
    }
}
