// CropDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "CropDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCropDlg dialog

CCropDlg::CCropDlg(CWnd* pParent, CMvDoc* pDoc)
    : CDialog(CCropDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCropDlg)
    //}}AFX_DATA_INIT

    ASSERT(pParent != NULL);

    m_pParent                = pParent;
    m_pDoc                   = pDoc;
    m_nID                    = CCropDlg::IDD;

    m_PropertySheet          = new CPropertySheet;
    m_ControlsPage           = new CCropControlsPage;
    m_ControlsPage->m_pDoc   = pDoc;
    m_ControlsPage->m_Parent = this;
    m_OptionsPage            = new CCropOptionsPage;
    m_OptionsPage->m_pDoc    = pDoc;
    m_OptionsPage->m_Parent  = this;
    m_PropertySheet->AddPage(m_ControlsPage);
    m_PropertySheet->AddPage(m_OptionsPage);
}

CCropDlg::~CCropDlg()
{
    delete m_PropertySheet;
    delete m_ControlsPage;
    delete m_OptionsPage;
}

void CCropDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCropDlg)
    DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
    DDX_Control(pDX, IDC_DEFAULT, m_DefaultButton);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCropDlg, CDialog)
    //{{AFX_MSG_MAP(CCropDlg)
    ON_BN_CLICKED(IDC_DONE, OnDone)
    ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
    ON_BN_CLICKED(IDC_APPLY, OnApply)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropDlg message handlers

BOOL CCropDlg::Create()
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
    m_PropertySheet->SetActivePage(1);
    m_PropertySheet->SetActivePage(0);

    return TRUE;
}

void CCropDlg::PostNcDestroy()
{
    delete this;
}

BOOL CCropDlg::OnInitDialog()
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
    wndpl.rcNormalPosition.top    = 10;
    wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
    wndpl.showCmd                 = SW_HIDE;
    SetWindowPlacement(&wndpl);

    m_DefaultButton.EnableWindow(FALSE);
    m_ApplyButton.EnableWindow(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CCropDlg::Reinitialize()
{
    m_ControlsPage->Reinitialize();
    m_OptionsPage->Reinitialize();
    m_DefaultButton.EnableWindow(FALSE);
    m_ApplyButton.EnableWindow(FALSE);
}

void CCropDlg::Activate(BOOL b)
{
    m_ControlsPage->Activate(b);
    m_OptionsPage->Activate(b);
    m_DefaultButton.EnableWindow(b);
}

void CCropDlg::OnDefault()
{
    if (AfxMessageBox("Do you want to use default settings?", MB_YESNO) == IDNO)
    {
        return;
    }

    switch (m_PropertySheet->GetActiveIndex())
    {
    case 0:
        m_ControlsPage->OnDefault();
        break;
    case 1:
        m_OptionsPage->OnDefault();
        break;
    }
}

void CCropDlg::OnDone()
{
    ShowWindow(SW_HIDE);
}

void CCropDlg::OnApply()
{
    switch (m_PropertySheet->GetActiveIndex())
    {
    case 0:
        m_ControlsPage->Apply();
        break;
    }
}
