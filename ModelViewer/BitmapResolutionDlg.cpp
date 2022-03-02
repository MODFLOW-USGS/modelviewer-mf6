// BitmapResolutionDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "BitmapResolutionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapResolutionDlg dialog

CBitmapResolutionDlg::CBitmapResolutionDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBitmapResolutionDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CBitmapResolutionDlg)
    m_SideOption       = -1;
    m_ResolutionOption = -1;
    //}}AFX_DATA_INIT
}

void CBitmapResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBitmapResolutionDlg)
    DDX_Radio(pDX, IDC_BMP_WIDTH_OPTION, m_SideOption);
    DDX_Radio(pDX, IDC_RESOLUTION_SCREEN, m_ResolutionOption);
    //}}AFX_DATA_MAP

    if (m_ResolutionOption != 0)
    {
        if (m_SideOption == 0)
        {
            DDX_Text(pDX, IDC_BMP_WIDTH, m_WidthInInches);
            DDV_MinMaxDouble(pDX, m_WidthInInches, 0.0, 16.0);
        }
        else
        {
            DDX_Text(pDX, IDC_BMP_HEIGHT, m_HeightInInches);
            DDV_MinMaxDouble(pDX, m_HeightInInches, 0.0, 16.0);
        }
    }
}

BEGIN_MESSAGE_MAP(CBitmapResolutionDlg, CDialog)
    //{{AFX_MSG_MAP(CBitmapResolutionDlg)
    ON_BN_CLICKED(IDC_BMP_HEIGHT_OPTION, OnBmpHeightOption)
    ON_BN_CLICKED(IDC_BMP_WIDTH_OPTION, OnBmpWidthOption)
    ON_BN_CLICKED(IDC_RESOLUTION_150, OnResolution150)
    ON_BN_CLICKED(IDC_RESOLUTION_300, OnResolution300)
    ON_BN_CLICKED(IDC_RESOLUTION_SCREEN, OnResolutionScreen)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapResolutionDlg message handlers

BOOL CBitmapResolutionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (m_ResolutionOption == 0)
    {
        GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
        GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
    }
    else
    {
        GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
        GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
        if (m_SideOption == 0)
        {
            GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
            if (m_WidthInInches == 0)
            {
                GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
            }
            GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
            GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
        }
        else
        {
            GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
            if (m_HeightInInches == 0)
            {
                GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
            }
            GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
            GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
        }
    }
    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CBitmapResolutionDlg::OnBmpHeightOption()
{
    GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
    GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
}

void CBitmapResolutionDlg::OnBmpWidthOption()
{
    GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
}

void CBitmapResolutionDlg::OnResolution150()
{
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
    if (((CButton*)GetDlgItem(IDC_BMP_WIDTH_OPTION))->GetCheck())
    {
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
    }
}

void CBitmapResolutionDlg::OnResolution300()
{
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
    if (((CButton*)GetDlgItem(IDC_BMP_WIDTH_OPTION))->GetCheck())
    {
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
    }
}

void CBitmapResolutionDlg::OnResolutionScreen()
{
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
    GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
}

BOOL CBitmapResolutionDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // Inactivates help when F1 is pressed
    // return CDialog::OnHelpInfo(pHelpInfo);
    return 0;
}
