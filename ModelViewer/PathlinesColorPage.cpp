// PathlinesColorPage.cpp : implementation file
//

#include "modelviewer.h"
#include "PathlinesColorPage.h"
#include "PathlinesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathlinesColorPage property page

IMPLEMENT_DYNCREATE(CPathlinesColorPage, CPropertyPage)

CPathlinesColorPage::CPathlinesColorPage()
    : CPropertyPage(CPathlinesColorPage::IDD)
{
    //{{AFX_DATA_INIT(CPathlinesColorPage)
    m_LogTransport = FALSE;
    //}}AFX_DATA_INIT
    m_ValueBlue    = 0.0;
    m_ValueRed     = 0.0;
    m_ExchangeData = FALSE;
}

CPathlinesColorPage::~CPathlinesColorPage()
{
}

void CPathlinesColorPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPathlinesColorPage)
    DDX_Check(pDX, IDC_CHECK_LOG_TRANSPORT, m_LogTransport);
    //}}AFX_DATA_MAP
    if (m_ExchangeData)
    {
        DDX_Text(pDX, IDC_VALUE_BLUE, m_ValueBlue);
        DDX_Text(pDX, IDC_VALUE_RED, m_ValueRed);
    }
}

BEGIN_MESSAGE_MAP(CPathlinesColorPage, CPropertyPage)
    //{{AFX_MSG_MAP(CPathlinesColorPage)
    ON_BN_CLICKED(IDC_CHECK_LOG_TRANSPORT, OnCheckLogTransport)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathlinesColorPage message handlers

BOOL CPathlinesColorPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathlinesColorPage::Reinitialize()
{
    GetDlgItem(IDC_VALUE_BLUE)->SetWindowText("");
    GetDlgItem(IDC_VALUE_RED)->SetWindowText("");
    Activate(FALSE);
}

void CPathlinesColorPage::Activate(BOOL b)
{
    GetDlgItem(IDC_VALUE_BLUE)->EnableWindow(b);
    GetDlgItem(IDC_VALUE_RED)->EnableWindow(b);
    m_IsActive = b;
}

BOOL CPathlinesColorPage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CPathlinesColorPage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        m_pDoc->SetPathlineColorBarEndPoints(m_ValueBlue, m_ValueRed);
        if (m_pDoc->GetColorBarSource() == 1)
        {
            m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
            m_pDoc->UpdateColorBarDlg();
        }
    }
}

void CPathlinesColorPage::OnDefault()
{
    double range[2];
    m_pDoc->GetPathlineTimeRange(range);
    if (m_ValueBlue < m_ValueRed)
    {
        if (range[0] == range[1])
        {
            m_ValueBlue = range[0] - 1;
            m_ValueRed  = range[1] + 1;
        }
        else
        {
            m_ValueBlue = range[0];
            m_ValueRed  = range[1];
        }
    }
    else
    {
        if (range[0] == range[1])
        {
            m_ValueBlue = range[1] + 1;
            m_ValueRed  = range[0] - 1;
        }
        else
        {
            m_ValueBlue = range[1];
            m_ValueRed  = range[0];
        }
    }
    m_LogTransport = 0;
    CustomUpdateData(FALSE);
    m_pDoc->SetPathlineLogTransform(m_LogTransport);
    m_pDoc->SetPathlineColorBarEndPoints(m_ValueBlue, m_ValueRed);
    if (m_pDoc->GetColorBarSource() == 1)
    {
        m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
        m_pDoc->UpdateColorBarDlg();
    }
}

BOOL CPathlinesColorPage::OnSetActive()
{
    m_Parent->m_ApplyButton.EnableWindow(m_IsActive);
    m_Parent->m_DefaultButton.EnableWindow(m_IsActive);
    return CPropertyPage::OnSetActive();
}

void CPathlinesColorPage::OnCheckLogTransport()
{
    UpdateData(TRUE);
    m_pDoc->SetPathlineLogTransform(m_LogTransport);
    if (m_pDoc->GetColorBarSource() == 1)
    {
        m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
        m_pDoc->UpdateColorBarDlg();
    }
}
