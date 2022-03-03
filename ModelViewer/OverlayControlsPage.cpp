// OverlayControlsPage.cpp : implementation file
//

#include "modelviewer.h"
#include "OverlayControlsPage.h"
#include "OverlayDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayControlsPage property page

IMPLEMENT_DYNCREATE(COverlayControlsPage, CPropertyPage)

COverlayControlsPage::COverlayControlsPage()
    : CPropertyPage(COverlayControlsPage::IDD)
{
    //{{AFX_DATA_INIT(COverlayControlsPage)
    m_XOrig    = 0.0;
    m_YOrig    = 0.0;
    m_Scale    = 0.0;
    m_Elev     = 0.0;
    m_DrapeGap = 0.0;
    m_Trim     = FALSE;
    m_Angle    = 0.0;
    m_Drape    = -1;
    m_Crop     = FALSE;
    //}}AFX_DATA_INIT
}

COverlayControlsPage::~COverlayControlsPage()
{
}

void COverlayControlsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COverlayControlsPage)
    DDX_Control(pDX, IDC_OVERLAY_FLAT, m_FlatRadioButton);
    DDX_Control(pDX, IDC_OVERLAY_TRIM, m_TrimCheckBox);
    DDX_Text(pDX, IDC_XORIG, m_XOrig);
    DDX_Text(pDX, IDC_YORIG, m_YOrig);
    DDX_Text(pDX, IDC_OVERLAY_SCALE, m_Scale);
    DDX_Text(pDX, IDC_OVERLAY_ELEV, m_Elev);
    DDX_Text(pDX, IDC_DRAPE_GAP, m_DrapeGap);
    DDX_Check(pDX, IDC_OVERLAY_TRIM, m_Trim);
    DDX_Text(pDX, IDC_ANGLE, m_Angle);
    DDX_Radio(pDX, IDC_OVERLAY_FLAT, m_Drape);
    DDX_Check(pDX, IDC_OVERLAY_CROP, m_Crop);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COverlayControlsPage, CPropertyPage)
    //{{AFX_MSG_MAP(COverlayControlsPage)
    ON_BN_CLICKED(IDC_OVERLAY_DRAPE, OnDrape)
    ON_BN_CLICKED(IDC_OVERLAY_FLAT, OnFlat)
    ON_BN_CLICKED(IDC_OVERLAY_TRIM, OnOverlayTrim)
    ON_BN_CLICKED(IDC_OVERLAY_CROP, OnOverlayCrop)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayControlsPage message handlers

void COverlayControlsPage::Reinitialize()
{
    m_Drape    = FALSE;
    m_Angle    = 0.0;
    m_XOrig    = 0.0;
    m_YOrig    = 0.0;
    m_Elev     = 0.0;
    m_Scale    = 1.0;
    m_DrapeGap = 0;
    UpdateData(FALSE);
    Activate(FALSE);
}

void COverlayControlsPage::Activate(BOOL b)
{
    BOOL flat = m_FlatRadioButton.GetCheck();
    GetDlgItem(IDC_OVERLAY_FLAT)->EnableWindow(b);
    GetDlgItem(IDC_OVERLAY_DRAPE)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_OVERLAY_TRIM)->EnableWindow(b && flat);
    GetDlgItem(IDC_OVERLAY_CROP)->EnableWindow(b);
    GetDlgItem(IDC_ANGLE)->EnableWindow(b);
    GetDlgItem(IDC_XORIG)->EnableWindow(b);
    GetDlgItem(IDC_YORIG)->EnableWindow(b);
    GetDlgItem(IDC_OVERLAY_ELEV)->EnableWindow(b && flat);
    GetDlgItem(IDC_OVERLAY_SCALE)->EnableWindow(b);
    GetDlgItem(IDC_DRAPE_GAP)->EnableWindow(b && !flat);
}

void COverlayControlsPage::OnDrape()
{
    GetDlgItem(IDC_OVERLAY_TRIM)->EnableWindow(FALSE);
    GetDlgItem(IDC_OVERLAY_ELEV)->EnableWindow(FALSE);
    GetDlgItem(IDC_DRAPE_GAP)->EnableWindow(TRUE);
    m_TrimCheckBox.SetCheck(TRUE);
    if (m_pDoc->HasOverlay())
    {
        m_Parent->Apply();
    }
}

void COverlayControlsPage::OnFlat()
{
    GetDlgItem(IDC_OVERLAY_TRIM)->EnableWindow(TRUE);
    GetDlgItem(IDC_OVERLAY_ELEV)->EnableWindow(TRUE);
    GetDlgItem(IDC_DRAPE_GAP)->EnableWindow(FALSE);
    if (m_pDoc->HasOverlay())
    {
        m_Parent->Apply();
    }
}

void COverlayControlsPage::OnOverlayTrim()
{
    if (m_pDoc->HasOverlay())
    {
        m_Parent->Apply();
    }
}

void COverlayControlsPage::OnOverlayCrop()
{
    if (m_pDoc->HasOverlay())
    {
        m_Parent->Apply();
    }
}
