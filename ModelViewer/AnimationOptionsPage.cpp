// AnimationOptionsPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "AnimationOptionsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationOptionsPage property page

IMPLEMENT_DYNCREATE(CAnimationOptionsPage, CPropertyPage)

CAnimationOptionsPage::CAnimationOptionsPage()
    : CPropertyPage(CAnimationOptionsPage::IDD)
{
    //{{AFX_DATA_INIT(CAnimationOptionsPage)
    m_Delay   = 0.0;
    m_Elevate = 0.0;
    m_Rotate  = 0.0;
    //}}AFX_DATA_INIT
}

CAnimationOptionsPage::~CAnimationOptionsPage()
{
}

void CAnimationOptionsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAnimationOptionsPage)
    DDX_Text(pDX, IDC_DELAY, m_Delay);
    DDX_Text(pDX, IDC_ELEVATE_VIEWPOINT, m_Elevate);
    DDX_Text(pDX, IDC_ROTATE_VIEWPOINT, m_Rotate);
    //}}AFX_DATA_MAP

    pDX->PrepareEditCtrl(IDC_DELAY);
    if (pDX->m_bSaveAndValidate)
    {
        if (m_Delay < 0)
        {
            AfxMessageBox("Please insure that \"Delay\" is non-negative.", MB_ICONEXCLAMATION);
            pDX->Fail();
        }
    }
}

BEGIN_MESSAGE_MAP(CAnimationOptionsPage, CPropertyPage)
    //{{AFX_MSG_MAP(CAnimationOptionsPage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationOptionsPage message handlers

BOOL CAnimationOptionsPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Activate(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimationOptionsPage::Reinitialize()
{
    m_Delay   = 0;
    m_Rotate  = 0;
    m_Elevate = 0;
    UpdateData(FALSE);
    Activate(FALSE);
}

void CAnimationOptionsPage::Activate(BOOL b)
{
    GetDlgItem(IDC_DELAY)->EnableWindow(b);
    GetDlgItem(IDC_ROTATE_VIEWPOINT)->EnableWindow(b);
    GetDlgItem(IDC_ELEVATE_VIEWPOINT)->EnableWindow(b);
}
