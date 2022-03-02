// AxesPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "AxesPage.h"
#include "MvDoc.h"
#include "GeometryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesPage property page

IMPLEMENT_DYNCREATE(CAxesPage, CPropertyPage)

CAxesPage::CAxesPage()
    : CPropertyPage(CAxesPage::IDD)
{
    //{{AFX_DATA_INIT(CAxesPage)
    //}}AFX_DATA_INIT
    m_Representation = 0;
    m_AxesSize       = 0.0;
    m_TubeDiameter   = 0.0;
    m_XPos           = 0.0;
    m_YPos           = 0.0;
    m_ZPos           = 0.0;
    m_ExchangeData   = FALSE;
}

CAxesPage::~CAxesPage()
{
}

void CAxesPage::DoDataExchange(CDataExchange *pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAxesPage)
    //}}AFX_DATA_MAP

    if (m_ExchangeData)
    {
        DDX_Radio(pDX, IDC_TUBE, m_Representation);
        DDX_Text(pDX, IDC_AXES_SIZE, m_AxesSize);
        DDX_Text(pDX, IDC_AXES_TUBE_DIAMETER, m_TubeDiameter);
        DDX_Text(pDX, IDC_X_ORIGIN, m_XPos);
        DDX_Text(pDX, IDC_Y_ORIGIN, m_YPos);
        DDX_Text(pDX, IDC_Z_ORIGIN, m_ZPos);
    }
}

BEGIN_MESSAGE_MAP(CAxesPage, CPropertyPage)
    //{{AFX_MSG_MAP(CAxesPage)
    ON_BN_CLICKED(IDC_LINE, OnLine)
    ON_BN_CLICKED(IDC_TUBE, OnTube)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesPage message handlers

BOOL CAxesPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CAxesPage::Reinitialize()
{
    m_Representation = 0;
    ((CButton *)GetDlgItem(IDC_TUBE))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_LINE))->SetCheck(FALSE);
    GetDlgItem(IDC_AXES_SIZE)->SetWindowText("");
    GetDlgItem(IDC_AXES_TUBE_DIAMETER)->SetWindowText("");
    GetDlgItem(IDC_X_ORIGIN)->SetWindowText("");
    GetDlgItem(IDC_Y_ORIGIN)->SetWindowText("");
    GetDlgItem(IDC_Z_ORIGIN)->SetWindowText("");
    Activate(FALSE);
}

void CAxesPage::Activate(BOOL b)
{
    GetDlgItem(IDC_LINE)->EnableWindow(b);
    GetDlgItem(IDC_TUBE)->EnableWindow(b);
    GetDlgItem(IDC_AXES_SIZE)->EnableWindow(b);
    GetDlgItem(IDC_AXES_TUBE_DIAMETER)->EnableWindow(b && ((CButton *)GetDlgItem(IDC_TUBE))->GetCheck());
    GetDlgItem(IDC_X_ORIGIN)->EnableWindow(b);
    GetDlgItem(IDC_Y_ORIGIN)->EnableWindow(b);
    GetDlgItem(IDC_Z_ORIGIN)->EnableWindow(b);
    m_IsActive = b;
    if (m_Parent->m_PropertySheet->GetActiveIndex() == 1)
    {
        m_Parent->m_ApplyButton.EnableWindow(b);
    }
}

BOOL CAxesPage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CAxesPage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        m_pDoc->SetAxesProperties(m_XPos, m_YPos, m_ZPos, m_AxesSize, m_TubeDiameter);
    }
}

void CAxesPage::OnLine()
{
    m_pDoc->SetAxesRepresentationToLine();
    GetDlgItem(IDC_AXES_TUBE_DIAMETER)->EnableWindow(FALSE);
}

void CAxesPage::OnTube()
{
    m_pDoc->SetAxesRepresentationToTube();
    GetDlgItem(IDC_AXES_TUBE_DIAMETER)->EnableWindow(TRUE);
}

BOOL CAxesPage::OnSetActive()
{
    m_Parent->m_ApplyButton.EnableWindow(m_IsActive);
    return CPropertyPage::OnSetActive();
}
