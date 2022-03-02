// OverlayDlg.cpp : implementation file
//

#include "modelviewer.h"
#include "OverlayDlg.h"
#include "MvDoc.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using std::ifstream;

/////////////////////////////////////////////////////////////////////////////
// COverlayDlg dialog

COverlayDlg::COverlayDlg(CWnd* pParent, CMvDoc* pDoc)
    : CDialog(COverlayDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(COverlayDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    ASSERT(pParent != NULL);

    m_pParent                = pParent;
    m_pDoc                   = pDoc;
    m_nID                    = COverlayDlg::IDD;

    m_PropertySheet          = new CPropertySheet;
    m_ControlsPage           = new COverlayControlsPage;
    m_ControlsPage->m_Parent = this;
    m_ControlsPage->m_pDoc   = m_pDoc;
    m_FilePage               = new COverlayFilePage;
    m_BoundsPage             = new COverlayBoundsPage;
    m_PropertySheet->AddPage(m_ControlsPage);
    m_PropertySheet->AddPage(m_FilePage);
    m_PropertySheet->AddPage(m_BoundsPage);
}

COverlayDlg::~COverlayDlg()
{
    delete m_PropertySheet;
    delete m_ControlsPage;
    delete m_FilePage;
    delete m_BoundsPage;
}

void COverlayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(COverlayDlg)
    DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
    DDX_Control(pDX, IDC_REMOVE, m_RemoveButton);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COverlayDlg, CDialog)
    //{{AFX_MSG_MAP(COverlayDlg)
    ON_WM_HELPINFO()
    ON_BN_CLICKED(IDC_APPLY, OnApply)
    ON_BN_CLICKED(IDC_DONE, OnDone)
    ON_BN_CLICKED(IDC_REMOVE, OnRemove)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayDlg message handlers

BOOL COverlayDlg::Create()
{
    if (!CDialog::Create(m_nID, m_pParent))
    {
        return FALSE;
    }

    m_PropertySheet->Create(this, WS_CHILD | WS_VISIBLE, 0);

    CRect rcSheet;
    GetDlgItem(IDC_PROP_SHEET)->GetWindowRect(&rcSheet);
    ScreenToClient(&rcSheet);

    CTabCtrl* tabctrl = m_PropertySheet->GetTabControl();
    tabctrl->MoveWindow(0, 0, rcSheet.Width(), rcSheet.Height());
    m_PropertySheet->SetWindowPos(NULL, rcSheet.left, rcSheet.top,
                                  rcSheet.Width(), rcSheet.Height(),
                                  SWP_NOZORDER | SWP_NOACTIVATE);

    m_PropertySheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
    m_PropertySheet->ModifyStyle(0, WS_TABSTOP);
    // Force the controls on the property pages to be created
    m_PropertySheet->SetActivePage(2);
    m_PropertySheet->SetActivePage(1);
    m_PropertySheet->SetActivePage(0);
    // Both pages are initially inactive
    m_ControlsPage->Activate(FALSE);
    m_FilePage->Activate(FALSE);

    return TRUE;
}

BOOL COverlayDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    WINDOWPLACEMENT wndpl;
    GetWindowPlacement(&wndpl);
    int screenWidth               = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight              = ::GetSystemMetrics(SM_CYSCREEN);
    int dlgWidth                  = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
    int dlgHeight                 = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
    wndpl.rcNormalPosition.right  = screenWidth - 200;
    wndpl.rcNormalPosition.left   = wndpl.rcNormalPosition.right - dlgWidth;
    wndpl.rcNormalPosition.top    = 150;
    wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
    wndpl.showCmd                 = SW_HIDE;
    SetWindowPlacement(&wndpl);

    m_ApplyButton.EnableWindow(FALSE);
    m_RemoveButton.EnableWindow(FALSE);
    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COverlayDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // Inactivates help when F1 is pressed
    return 0;
}

void COverlayDlg::PostNcDestroy()
{
    delete this;
}

void COverlayDlg::Reinitialize()
{
    m_ControlsPage->Reinitialize();
    m_FilePage->Reinitialize();
    m_ApplyButton.EnableWindow(FALSE);
    m_RemoveButton.EnableWindow(FALSE);
}

void COverlayDlg::Activate(BOOL b)
{
    m_ControlsPage->Activate(b);
    m_FilePage->Activate(b);
    m_ApplyButton.EnableWindow(b);
}

void COverlayDlg::OnApply()
{
    m_FilePage->UpdateData(TRUE);
    if (m_FilePage->m_Filename.GetLength() == 0)
    {
        m_PropertySheet->SetActivePage(1);
        AfxMessageBox("Please specify the overlay file.", MB_ICONEXCLAMATION);
        return;
    }
    // Test open the overlay file
    ifstream in(m_FilePage->m_Filename, std::ifstream::in);
    if (!in.is_open())
    {
        m_PropertySheet->SetActivePage(1);
        AfxMessageBox("Unable to open the overlay file");
        return;
    }
    in.close();

    m_ControlsPage->UpdateData(TRUE);

    char filename[MAX_PATH];
    strcpy(filename, (LPCTSTR)(m_FilePage->m_Filename));
    int type = m_FilePage->m_TypeComboBox.GetCurSel() + 1;

    m_pDoc->ApplyOverlayControl(filename, type, m_ControlsPage->m_XOrig, m_ControlsPage->m_YOrig,
                                m_ControlsPage->m_Scale, m_ControlsPage->m_Angle, m_ControlsPage->m_Drape,
                                m_ControlsPage->m_Trim, m_ControlsPage->m_Crop, m_ControlsPage->m_Elev, m_ControlsPage->m_DrapeGap);
}

void COverlayDlg::OnDone()
{
    ShowWindow(SW_HIDE);
}

void COverlayDlg::OnRemove()
{
    if (AfxMessageBox("Do you want to remove the overlay?", MB_YESNO) == IDNO)
    {
        return;
    }
    m_pDoc->RemoveOverlay();
}
