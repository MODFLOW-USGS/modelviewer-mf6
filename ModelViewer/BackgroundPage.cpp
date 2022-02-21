// BackgroundPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "BackgroundPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackgroundPage property page

IMPLEMENT_DYNCREATE(CBackgroundPage, CPropertyPage)

CBackgroundPage::CBackgroundPage() : CPropertyPage(CBackgroundPage::IDD)
{
	//{{AFX_DATA_INIT(CBackgroundPage)
	m_Red = 0;
	m_Green = 0;
	m_Blue = 0;
	m_Background = -1;
	//}}AFX_DATA_INIT
}

CBackgroundPage::~CBackgroundPage()
{
}

void CBackgroundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackgroundPage)
	DDX_Control(pDX, IDC_BLUE_SLIDER, m_BlueSlider);
	DDX_Control(pDX, IDC_GREEN_SLIDER, m_GreenSlider);
	DDX_Control(pDX, IDC_RED_SLIDER, m_RedSlider);
	DDX_Slider(pDX, IDC_RED_SLIDER, m_Red);
	DDX_Slider(pDX, IDC_GREEN_SLIDER, m_Green);
	DDX_Slider(pDX, IDC_BLUE_SLIDER, m_Blue);
	DDX_Radio(pDX, IDC_WHITE_BACKGROUND, m_Background);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackgroundPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBackgroundPage)
	ON_BN_CLICKED(IDC_WHITE_BACKGROUND, OnWhiteBackground)
	ON_BN_CLICKED(IDC_CUSTOM_BACKGROUND, OnCustomBackground)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_RED_SLIDER, OnReleasedcaptureRedSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_GREEN_SLIDER, OnReleasedcaptureGreenSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_BLUE_SLIDER, OnReleasedcaptureBlueSlider)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundPage message handlers

BOOL CBackgroundPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_RedSlider.SetRange(0, 100, TRUE);
	m_GreenSlider.SetRange(0, 100, TRUE);
	m_BlueSlider.SetRange(0, 100, TRUE);
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBackgroundPage::Reinitialize()
{
	m_Red = 100;
	m_Green = 100;
	m_Blue = 100;
	m_Background = 0;
	UpdateData(FALSE);
	UpdateLabels();
	Activate(FALSE);
}

void CBackgroundPage::Activate(BOOL b)
{
	GetDlgItem(IDC_WHITE_BACKGROUND)->EnableWindow(b);
	GetDlgItem(IDC_CUSTOM_BACKGROUND)->EnableWindow(b);
	m_RedSlider.EnableWindow(m_Background);
	m_GreenSlider.EnableWindow(m_Background);
	m_BlueSlider.EnableWindow(m_Background);
}

void CBackgroundPage::OnDefault()
{
	m_Red = 100;
	m_Green = 100;
	m_Blue = 100;
	m_Background = 0;
	UpdateData(FALSE);
	UpdateLabels();
	m_RedSlider.EnableWindow(FALSE);
	m_GreenSlider.EnableWindow(FALSE);
	m_BlueSlider.EnableWindow(FALSE);
	m_pDoc->SetBackgroundColor(1, 1, 1);
}

void CBackgroundPage::OnWhiteBackground() 
{
	m_RedSlider.EnableWindow(FALSE);
	m_GreenSlider.EnableWindow(FALSE);
	m_BlueSlider.EnableWindow(FALSE);
	m_pDoc->SetBackgroundColor(1, 1, 1);;
}

void CBackgroundPage::OnCustomBackground() 
{
	UpdateData(TRUE);
	m_RedSlider.EnableWindow(TRUE);
	m_GreenSlider.EnableWindow(TRUE);
	m_BlueSlider.EnableWindow(TRUE);
	m_pDoc->SetBackgroundColor(m_Red*0.01, m_Green*0.01, m_Blue*0.01);;
}

void CBackgroundPage::OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetBackgroundColor(m_Red*0.01, m_Green*0.01, m_Blue*0.01);;
	*pResult = 0;
}

void CBackgroundPage::OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetBackgroundColor(m_Red*0.01, m_Green*0.01, m_Blue*0.01);;
	*pResult = 0;
}

void CBackgroundPage::OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetBackgroundColor(m_Red*0.01, m_Green*0.01, m_Blue*0.01);;
	*pResult = 0;
}

void CBackgroundPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char label[6];
	UpdateData(TRUE);
	if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_RED_SLIDER))
	{
		sprintf(label,"%4.2f", m_Red/100.0);
		GetDlgItem(IDC_RED)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_GREEN_SLIDER))
	{
		sprintf(label,"%4.2f", m_Green/100.0);
		GetDlgItem(IDC_GREEN)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_BLUE_SLIDER))
	{
		sprintf(label,"%4.2f", m_Blue/100.0);
		GetDlgItem(IDC_BLUE)->SetWindowText(label);
	}
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBackgroundPage::UpdateLabels()
{
	char label[6];
	sprintf(label,"%4.2f", m_Red/100.0);
	GetDlgItem(IDC_RED)->SetWindowText(label);
	sprintf(label,"%4.2f", m_Green/100.0);
	GetDlgItem(IDC_GREEN)->SetWindowText(label);
	sprintf(label,"%4.2f", m_Blue/100.0);
	GetDlgItem(IDC_BLUE)->SetWindowText(label);
}
