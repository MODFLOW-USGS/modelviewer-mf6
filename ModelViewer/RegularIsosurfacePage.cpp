// RegularIsosurfacePage.cpp : implementation file
//

#include "ModelViewer.h"
#include "RegularIsosurfacePage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegularIsosurfacePage property page

IMPLEMENT_DYNCREATE(CRegularIsosurfacePage, CPropertyPage)

CRegularIsosurfacePage::CRegularIsosurfacePage()
    : CPropertyPage(CRegularIsosurfacePage::IDD)
{
    //{{AFX_DATA_INIT(CRegularIsosurfacePage)
    //}}AFX_DATA_INIT
    m_ExchangeData = FALSE;
}

CRegularIsosurfacePage::~CRegularIsosurfacePage()
{
}

void CRegularIsosurfacePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRegularIsosurfacePage)
    //}}AFX_DATA_MAP

    // Data validation is performed only when m_DoDataValidation
    // is set to 1. This allows switching tabs without data validation.
    if (m_ExchangeData)
    {
        DDX_Text(pDX, IDC_ISOSURFACE_MAX, m_ValueMax);
        DDX_Text(pDX, IDC_ISOSURFACE_MIN, m_ValueMin);
        DDX_Text(pDX, IDC_NUMBER_OF_ISOSURFACES, m_IsosurfaceCount);
    }
}

BEGIN_MESSAGE_MAP(CRegularIsosurfacePage, CPropertyPage)
    //{{AFX_MSG_MAP(CRegularIsosurfacePage)
    ON_BN_CLICKED(IDC_FULL_RANGE, OnFullRange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegularIsosurfacePage message handlers

BOOL CRegularIsosurfacePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    GetDlgItem(IDC_ISOSURFACE_MIN)->SetWindowText("");
    GetDlgItem(IDC_ISOSURFACE_MAX)->SetWindowText("");
    GetDlgItem(IDC_NUMBER_OF_ISOSURFACES)->SetWindowText("");
    Activate(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegularIsosurfacePage::Reinitialize()
{
    GetDlgItem(IDC_ISOSURFACE_MIN)->SetWindowText("");
    GetDlgItem(IDC_ISOSURFACE_MAX)->SetWindowText("");
    GetDlgItem(IDC_NUMBER_OF_ISOSURFACES)->SetWindowText("");
    Activate(FALSE);
}

void CRegularIsosurfacePage::Activate(BOOL b)
{
    GetDlgItem(IDC_ISOSURFACE_MIN)->EnableWindow(b);
    GetDlgItem(IDC_ISOSURFACE_MAX)->EnableWindow(b);
    GetDlgItem(IDC_NUMBER_OF_ISOSURFACES)->EnableWindow(b);
    GetDlgItem(IDC_FULL_RANGE)->EnableWindow(b);
    m_IsActive = b;
}

BOOL CRegularIsosurfacePage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CRegularIsosurfacePage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        m_pDoc->SetRegularIsosurfaces(m_IsosurfaceCount, m_ValueMin, m_ValueMax);
    }
}

void CRegularIsosurfacePage::OnFullRange()
{
    if (AfxMessageBox("Do you want to set the max and min values to the full range of the data?", MB_YESNO) == IDNO)
    {
        return;
    }
    double range[2];
    m_pDoc->GetScalarDataRange(range);
    m_ValueMin = range[0];
    m_ValueMax = range[1];
    CustomUpdateData(FALSE);
    Apply();
}

BOOL CRegularIsosurfacePage::OnSetActive()
{
    if (m_IsActive)
    {
        Apply();
    }
    return CPropertyPage::OnSetActive();
}
