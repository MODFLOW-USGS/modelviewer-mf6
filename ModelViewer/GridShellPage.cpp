// GridShellPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "GridShellPage.h"
#include "MvDoc.h"
#include "GridDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridShellPage property page

IMPLEMENT_DYNCREATE(CGridShellPage, CPropertyPage)

CGridShellPage::CGridShellPage()
    : CPropertyPage(CGridShellPage::IDD)
{
    //{{AFX_DATA_INIT(CGridShellPage)
    m_Red     = 0;
    m_Green   = 0;
    m_Blue    = 0;
    m_Opacity = 0;
    //}}AFX_DATA_INIT
}

CGridShellPage::~CGridShellPage()
{
}

void CGridShellPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGridShellPage)
    DDX_Control(pDX, IDC_OPACITY_SLIDER, m_OpacitySlider);
    DDX_Control(pDX, IDC_BLUE_SLIDER, m_BlueSlider);
    DDX_Control(pDX, IDC_GREEN_SLIDER, m_GreenSlider);
    DDX_Control(pDX, IDC_RED_SLIDER, m_RedSlider);
    DDX_Slider(pDX, IDC_RED_SLIDER, m_Red);
    DDX_Slider(pDX, IDC_GREEN_SLIDER, m_Green);
    DDX_Slider(pDX, IDC_BLUE_SLIDER, m_Blue);
    DDX_Slider(pDX, IDC_OPACITY_SLIDER, m_Opacity);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGridShellPage, CPropertyPage)
    //{{AFX_MSG_MAP(CGridShellPage)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_RED_SLIDER, OnReleasedcaptureRedSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_GREEN_SLIDER, OnReleasedcaptureGreenSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BLUE_SLIDER, OnReleasedcaptureBlueSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_OPACITY_SLIDER, OnReleasedcaptureOpacitySlider)
    ON_WM_HSCROLL()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridShellPage message handlers

BOOL CGridShellPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_RedSlider.SetRange(0, 100, TRUE);
    m_GreenSlider.SetRange(0, 100, TRUE);
    m_BlueSlider.SetRange(0, 100, TRUE);
    m_OpacitySlider.SetRange(0, 100);
    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridShellPage::Reinitialize()
{
    m_Red     = 100;
    m_Green   = 80;
    m_Blue    = 60;
    m_Opacity = 20;
    UpdateData(FALSE);
    UpdateLabels();
    Activate(FALSE);
}

void CGridShellPage::Activate(BOOL b)
{
    m_RedSlider.EnableWindow(b);
    m_GreenSlider.EnableWindow(b);
    m_BlueSlider.EnableWindow(b);
    m_OpacitySlider.EnableWindow(b);
}

void CGridShellPage::OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetGridShellColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CGridShellPage::OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetGridShellColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CGridShellPage::OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetGridShellColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CGridShellPage::OnReleasedcaptureOpacitySlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetGridShellOpacity(m_Opacity * 0.01);
    *pResult = 0;
}

void CGridShellPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    char label[6];
    UpdateData(TRUE);
    if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_RED_SLIDER))
    {
        sprintf(label, "%4.2f", m_Red / 100.0);
        GetDlgItem(IDC_RED)->SetWindowText(label);
    }
    else if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_GREEN_SLIDER))
    {
        sprintf(label, "%4.2f", m_Green / 100.0);
        GetDlgItem(IDC_GREEN)->SetWindowText(label);
    }
    else if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_BLUE_SLIDER))
    {
        sprintf(label, "%4.2f", m_Blue / 100.0);
        GetDlgItem(IDC_BLUE)->SetWindowText(label);
    }
    else if (pScrollBar == (CScrollBar*)GetDlgItem(IDC_OPACITY_SLIDER))
    {
        sprintf(label, "%4.2f", m_Opacity / 100.0);
        GetDlgItem(IDC_OPACITY)->SetWindowText(label);
    }
    CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGridShellPage::UpdateLabels()
{
    char label[6];
    sprintf(label, "%4.2f", m_Red / 100.0);
    GetDlgItem(IDC_RED)->SetWindowText(label);
    sprintf(label, "%4.2f", m_Green / 100.0);
    GetDlgItem(IDC_GREEN)->SetWindowText(label);
    sprintf(label, "%4.2f", m_Blue / 100.0);
    GetDlgItem(IDC_BLUE)->SetWindowText(label);
    sprintf(label, "%4.2f", m_Opacity / 100.0);
    GetDlgItem(IDC_OPACITY)->SetWindowText(label);
}

BOOL CGridShellPage::OnSetActive()
{
    m_Parent->m_ApplyButton.EnableWindow(FALSE);
    return CPropertyPage::OnSetActive();
}
