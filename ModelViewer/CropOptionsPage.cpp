// CropOptionsPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "CropOptionsPage.h"
#include "MvDoc.h"
#include "CropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCropOptionsPage property page

IMPLEMENT_DYNCREATE(CCropOptionsPage, CPropertyPage)

CCropOptionsPage::CCropOptionsPage()
    : CPropertyPage(CCropOptionsPage::IDD)
{
    //{{AFX_DATA_INIT(CCropOptionsPage)
    m_Red                   = 0;
    m_Green                 = 0;
    m_Blue                  = 0;
    m_Opacity               = 0;
    m_ShowCroppedAwayPieces = FALSE;
    //}}AFX_DATA_INIT
}

CCropOptionsPage::~CCropOptionsPage()
{
}

void CCropOptionsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCropOptionsPage)
    DDX_Control(pDX, IDC_OPACITY_SLIDER, m_OpacitySlider);
    DDX_Control(pDX, IDC_BLUE_SLIDER, m_BlueSlider);
    DDX_Control(pDX, IDC_GREEN_SLIDER, m_GreenSlider);
    DDX_Control(pDX, IDC_RED_SLIDER, m_RedSlider);
    DDX_Slider(pDX, IDC_RED_SLIDER, m_Red);
    DDX_Slider(pDX, IDC_GREEN_SLIDER, m_Green);
    DDX_Slider(pDX, IDC_BLUE_SLIDER, m_Blue);
    DDX_Slider(pDX, IDC_OPACITY_SLIDER, m_Opacity);
    DDX_Check(pDX, IDC_SHOW_CROPPED_AWAY, m_ShowCroppedAwayPieces);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCropOptionsPage, CPropertyPage)
    //{{AFX_MSG_MAP(CCropOptionsPage)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_RED_SLIDER, OnReleasedcaptureRedSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_GREEN_SLIDER, OnReleasedcaptureGreenSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BLUE_SLIDER, OnReleasedcaptureBlueSlider)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_OPACITY_SLIDER, OnReleasedcaptureOpacitySlider)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_SHOW_CROPPED_AWAY, OnShowCroppedAway)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropOptionsPage message handlers

BOOL CCropOptionsPage::OnInitDialog()
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

void CCropOptionsPage::Reinitialize()
{
    m_ShowCroppedAwayPieces = FALSE;
    m_Red                   = 100;
    m_Green                 = 80;
    m_Blue                  = 60;
    m_Opacity               = 20;
    UpdateData(FALSE);
    UpdateLabels();
    Activate(FALSE);
}

void CCropOptionsPage::Activate(BOOL b)
{
    GetDlgItem(IDC_SHOW_CROPPED_AWAY)->EnableWindow(b);
    BOOL bb = ((CButton*)GetDlgItem(IDC_SHOW_CROPPED_AWAY))->GetCheck();
    m_RedSlider.EnableWindow(b && bb);
    m_GreenSlider.EnableWindow(b && bb);
    m_BlueSlider.EnableWindow(b && bb);
    m_OpacitySlider.EnableWindow(b && bb);
}

void CCropOptionsPage::OnDefault()
{
    m_ShowCroppedAwayPieces = FALSE;
    m_Red                   = 100;
    m_Green                 = 80;
    m_Blue                  = 60;
    m_Opacity               = 20;
    UpdateData(FALSE);
    UpdateLabels();
    m_pDoc->HideCroppedAwayPieces();
    m_pDoc->SetCroppedAwayPiecesColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    m_pDoc->SetCroppedAwayPiecesOpacity(m_Opacity * 0.01);
}

void CCropOptionsPage::OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetCroppedAwayPiecesColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CCropOptionsPage::OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetCroppedAwayPiecesColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CCropOptionsPage::OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetCroppedAwayPiecesColor(m_Red * 0.01, m_Green * 0.01, m_Blue * 0.01);
    *pResult = 0;
}

void CCropOptionsPage::OnReleasedcaptureOpacitySlider(NMHDR* pNMHDR, LRESULT* pResult)
{
    UpdateData(TRUE);
    m_pDoc->SetCroppedAwayPiecesOpacity(m_Opacity * 0.01);
    *pResult = 0;
}

void CCropOptionsPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

void CCropOptionsPage::UpdateLabels()
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

void CCropOptionsPage::OnShowCroppedAway()
{
    BOOL b = ((CButton*)GetDlgItem(IDC_SHOW_CROPPED_AWAY))->GetCheck();
    if (b)
    {
        m_pDoc->ShowCroppedAwayPieces();
    }
    else
    {
        m_pDoc->HideCroppedAwayPieces();
    }
    m_RedSlider.EnableWindow(b);
    m_GreenSlider.EnableWindow(b);
    m_BlueSlider.EnableWindow(b);
    m_OpacitySlider.EnableWindow(b);
}

BOOL CCropOptionsPage::OnSetActive()
{
    m_Parent->m_ApplyButton.EnableWindow(FALSE);
    return CPropertyPage::OnSetActive();
}
