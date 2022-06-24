// GridDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "GridDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

CGridDlg::CGridDlg(CWnd* pParent, CMvDoc* pDoc)
    : CDialog(CGridDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CGridDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    ASSERT(pParent != NULL);

    m_pParent                   = pParent;
    m_pDoc                      = pDoc;
    m_nID                       = CGridDlg::IDD;
    m_PropertySheet             = new CPropertySheet;
    m_GridLinesPage             = new CGridLinesPage;
    m_GridLinesPage->m_pDoc     = pDoc;
    m_GridLinesPage->m_Parent   = this;
    m_GridShellPage             = new CGridShellPage;
    m_GridShellPage->m_pDoc     = pDoc;
    m_GridShellPage->m_Parent   = this;
    m_SubgridPage               = new CSubgridPage;
    m_SubgridPage->m_pDoc       = pDoc;
    m_SubgridPage->m_Parent     = this;
#ifdef ENABLE_CGRIDDISPLAYPAGE
    m_GridDisplayPage           = new CGridDisplayPage;
    m_GridDisplayPage->m_pDoc   = pDoc;
    m_GridDisplayPage->m_Parent = this;
#endif
    m_PropertySheet->AddPage(m_GridLinesPage);
    m_PropertySheet->AddPage(m_GridShellPage);
    m_PropertySheet->AddPage(m_SubgridPage);
#ifdef ENABLE_CGRIDDISPLAYPAGE
    m_PropertySheet->AddPage(m_GridDisplayPage);
#endif
}

CGridDlg::~CGridDlg()
{
    delete m_PropertySheet;
    delete m_GridLinesPage;
    delete m_GridShellPage;
    delete m_SubgridPage;
#ifdef ENABLE_CGRIDDISPLAYPAGE
    delete m_GridDisplayPage;
#endif
}

void CGridDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGridDlg)
    DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGridDlg, CDialog)
    //{{AFX_MSG_MAP(CGridDlg)
    ON_BN_CLICKED(IDC_DONE, OnDone)
    ON_BN_CLICKED(IDC_APPLY, OnApply)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDlg message handlers
BOOL CGridDlg::Create()
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
    m_PropertySheet->SetActivePage(3);
    m_PropertySheet->SetActivePage(2);
    m_PropertySheet->SetActivePage(1);
    m_PropertySheet->SetActivePage(0);

    return TRUE;
}

void CGridDlg::PostNcDestroy()
{
    delete this;
}

BOOL CGridDlg::OnInitDialog()
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
    wndpl.rcNormalPosition.top    = screenHeight - dlgHeight - 30;
    wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
    wndpl.showCmd                 = SW_HIDE;
    SetWindowPlacement(&wndpl);

    m_ApplyButton.EnableWindow(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridDlg::Reinitialize()
{
    m_GridLinesPage->Reinitialize();
    m_GridShellPage->Reinitialize();
    m_SubgridPage->Reinitialize();
#ifdef ENABLE_CGRIDDISPLAYPAGE
    m_GridDisplayPage->Reinitialize();
#endif
}

void CGridDlg::Activate(BOOL Activate)
{
    BOOL b = Activate;
    m_GridLinesPage->Activate(b);
    m_GridShellPage->Activate(b);
    m_SubgridPage->Activate(b);
#ifdef ENABLE_CGRIDDISPLAYPAGE
    m_GridDisplayPage->Activate(b);
#endif
}

void CGridDlg::OnApply()
{
    switch (m_PropertySheet->GetActiveIndex())
    {
    case 0:
        m_GridLinesPage->Apply();
        break;
    case 2:
        m_SubgridPage->Apply();
        break;
    }
}

void CGridDlg::OnDone()
{
    ShowWindow(SW_HIDE);
}
