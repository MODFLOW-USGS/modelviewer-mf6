// LightsPage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "LightsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightsPage property page

IMPLEMENT_DYNCREATE(CLightsPage, CPropertyPage)

CLightsPage::CLightsPage() : CPropertyPage(CLightsPage::IDD)
{
	//{{AFX_DATA_INIT(CLightsPage)
	m_AuxiliaryLightIntensity = 0;
	m_HeadlightIntensity = 0;
	m_AuxiliaryLightOn = FALSE;
	m_HeadlightOn = FALSE;
	m_DirX = 0;
	m_DirY = 0;
	m_DirZ = 0;
	//}}AFX_DATA_INIT
}

CLightsPage::~CLightsPage()
{
}

void CLightsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightsPage)
	DDX_Control(pDX, IDC_HEADLIGHT, m_HeadlightCheckBox);
	DDX_Control(pDX, IDC_AUXILIARY_LIGHT, m_AuxiliaryLightCheckBox);
	DDX_Control(pDX, IDC_Z_SLIDER, m_ZSlider);
	DDX_Control(pDX, IDC_Y_SLIDER, m_YSlider);
	DDX_Control(pDX, IDC_X_SLIDER, m_XSlider);
	DDX_Control(pDX, IDC_HEADLIGHT_INTENSITY_SLIDER, m_HeadlightIntensitySlider);
	DDX_Control(pDX, IDC_AUXILIARY_LIGHT_INTENSITY_SLIDER, m_AuxiliaryLightIntensitySlider);
	DDX_Slider(pDX, IDC_AUXILIARY_LIGHT_INTENSITY_SLIDER, m_AuxiliaryLightIntensity);
	DDX_Slider(pDX, IDC_HEADLIGHT_INTENSITY_SLIDER, m_HeadlightIntensity);
	DDX_Check(pDX, IDC_AUXILIARY_LIGHT, m_AuxiliaryLightOn);
	DDX_Check(pDX, IDC_HEADLIGHT, m_HeadlightOn);
	DDX_Slider(pDX, IDC_X_SLIDER, m_DirX);
	DDX_Slider(pDX, IDC_Y_SLIDER, m_DirY);
	DDX_Slider(pDX, IDC_Z_SLIDER, m_DirZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLightsPage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_HEADLIGHT_INTENSITY_SLIDER, OnReleasedcaptureHeadlightIntensitySlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AUXILIARY_LIGHT_INTENSITY_SLIDER, OnReleasedcaptureAuxiliaryLightIntensitySlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_X_SLIDER, OnReleasedcaptureXSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Y_SLIDER, OnReleasedcaptureYSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Z_SLIDER, OnReleasedcaptureZSlider)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_HEADLIGHT, OnHeadlight)
	ON_BN_CLICKED(IDC_AUXILIARY_LIGHT, OnAuxiliaryLight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightsPage message handlers

BOOL CLightsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_HeadlightIntensitySlider.SetRange(0, 100, TRUE);
	m_AuxiliaryLightIntensitySlider.SetRange(0, 100, TRUE);
	m_XSlider.SetRange(0, 20, TRUE);
	m_YSlider.SetRange(0, 20, TRUE);
	m_ZSlider.SetRange(0, 20, TRUE);
	Reinitialize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightsPage::Reinitialize()
{
	m_AuxiliaryLightIntensity = 0;
	m_AuxiliaryLightOn = FALSE;
	m_HeadlightIntensity = 100;
	m_HeadlightOn = TRUE;
	m_DirX = 10;
	m_DirY = 10;
	m_DirZ = 20;
	UpdateData(FALSE);
	UpdateLabels();
	Activate(FALSE);
}

void CLightsPage::Activate(BOOL b)
{
	m_HeadlightCheckBox.EnableWindow(b);
	m_AuxiliaryLightCheckBox.EnableWindow(b);
	m_HeadlightIntensitySlider.EnableWindow(b && m_HeadlightCheckBox.GetCheck());
	BOOL bb = m_AuxiliaryLightCheckBox.GetCheck();
	m_AuxiliaryLightIntensitySlider.EnableWindow(b && bb);
	m_XSlider.EnableWindow(b && bb);
	m_YSlider.EnableWindow(b && bb);
	m_ZSlider.EnableWindow(b && bb);
}

void CLightsPage::OnDefault()
{
	m_HeadlightOn = TRUE;
	m_AuxiliaryLightOn = FALSE;
	m_HeadlightIntensity = 100;
	m_AuxiliaryLightIntensity = 0;
	m_DirX = 10;
	m_DirY = 10;
	m_DirZ = 20;
	UpdateData(FALSE);
	UpdateLabels();
	m_AuxiliaryLightIntensitySlider.EnableWindow(FALSE);
	m_XSlider.EnableWindow(FALSE);
	m_YSlider.EnableWindow(FALSE);
	m_ZSlider.EnableWindow(FALSE);
	m_pDoc->SwitchOnHeadlight(TRUE);
	m_pDoc->SwitchOnAuxiliaryLight(FALSE);
	m_pDoc->SetHeadlightIntensity(m_HeadlightIntensity*0.01);
	m_pDoc->SetAuxiliaryLightIntensity(m_AuxiliaryLightIntensity*0.01);
	m_pDoc->SetAuxiliaryLightPosition(m_DirX*0.1, m_DirY*0.1, m_DirZ*0.1);
}

void CLightsPage::OnReleasedcaptureHeadlightIntensitySlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetHeadlightIntensity(m_HeadlightIntensity*0.01);
	*pResult = 0;
}

void CLightsPage::OnReleasedcaptureAuxiliaryLightIntensitySlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetAuxiliaryLightIntensity(m_AuxiliaryLightIntensity*0.01);
	*pResult = 0;
}

void CLightsPage::OnReleasedcaptureXSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetAuxiliaryLightPosition((m_DirX-10)*0.1, (m_DirY-10)*0.1, (m_DirZ-10)*0.1);
	*pResult = 0;
}

void CLightsPage::OnReleasedcaptureYSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetAuxiliaryLightPosition((m_DirX-10)*0.1, (m_DirY-10)*0.1, (m_DirZ-10)*0.1);
	*pResult = 0;
}

void CLightsPage::OnReleasedcaptureZSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetAuxiliaryLightPosition((m_DirX-10)*0.1, (m_DirY-10)*0.1, (m_DirZ-10)*0.1);
	*pResult = 0;
}

void CLightsPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char label[25];
	UpdateData(TRUE);
	if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_HEADLIGHT_INTENSITY_SLIDER))
	{
		sprintf(label,"%4.2f", m_HeadlightIntensity*.01);
		GetDlgItem(IDC_HEADLIGHT_INTENSITY)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_AUXILIARY_LIGHT_INTENSITY_SLIDER))
	{
		sprintf(label,"%4.2f", m_AuxiliaryLightIntensity*.01);
		GetDlgItem(IDC_AUXILIARY_LIGHT_INTENSITY)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_X_SLIDER)
			|| pScrollBar == (CScrollBar *) GetDlgItem(IDC_Y_SLIDER)
			|| pScrollBar == (CScrollBar *) GetDlgItem(IDC_Z_SLIDER))
	{
		sprintf(label, "(%4.1f, %4.1f, %4.1f)", (m_DirX-10)*0.1, (m_DirY-10)*0.1, (m_DirZ-10)*0.1);
		GetDlgItem(IDC_AUXILIARY_LIGHT_POSITION)->SetWindowText(label);
	}
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLightsPage::OnHeadlight() 
{
	BOOL b = m_HeadlightCheckBox.GetCheck();
	m_pDoc->SwitchOnHeadlight(b);
	m_HeadlightIntensitySlider.EnableWindow(b);
}

void CLightsPage::OnAuxiliaryLight() 
{
	BOOL b = m_AuxiliaryLightCheckBox.GetCheck();
	m_AuxiliaryLightIntensitySlider.EnableWindow(b);
	m_XSlider.EnableWindow(b);
	m_YSlider.EnableWindow(b);
	m_ZSlider.EnableWindow(b);
	m_pDoc->SwitchOnAuxiliaryLight(b);
}

void CLightsPage::UpdateLabels()
{
	char label[25];
	sprintf(label,"%4.2f", m_HeadlightIntensity*.01);
	GetDlgItem(IDC_HEADLIGHT_INTENSITY)->SetWindowText(label);
	sprintf(label,"%4.2f", m_AuxiliaryLightIntensity*.01);
	GetDlgItem(IDC_AUXILIARY_LIGHT_INTENSITY)->SetWindowText(label);
	sprintf(label, "(%4.1f, %4.1f, %4.1f)", (m_DirX-10)*0.1, (m_DirY-10)*0.1, (m_DirZ-10)*0.1);
	GetDlgItem(IDC_AUXILIARY_LIGHT_POSITION)->SetWindowText(label);
}
