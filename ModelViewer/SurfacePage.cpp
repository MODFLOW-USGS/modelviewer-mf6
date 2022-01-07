// SurfacePage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "SurfacePage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSurfacePage property page

IMPLEMENT_DYNCREATE(CSurfacePage, CPropertyPage)

CSurfacePage::CSurfacePage() : CPropertyPage(CSurfacePage::IDD)
{
	//{{AFX_DATA_INIT(CSurfacePage)
	m_Diffuse = 0;
	m_Ambient = 0;
	m_Specular = 0;
	m_SpecularPower = 0;
	//}}AFX_DATA_INIT
}

CSurfacePage::~CSurfacePage()
{
}

void CSurfacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSurfacePage)
	DDX_Control(pDX, IDC_SPECULAR_POWER_SLIDER, m_SpecularPowerSlider);
	DDX_Control(pDX, IDC_SPECULAR_SLIDER, m_SpecularSlider);
	DDX_Control(pDX, IDC_AMBIENT_SLIDER, m_AmbientSlider);
	DDX_Control(pDX, IDC_DIFFUSE_SLIDER, m_DiffuseSlider);
	DDX_Slider(pDX, IDC_DIFFUSE_SLIDER, m_Diffuse);
	DDX_Slider(pDX, IDC_AMBIENT_SLIDER, m_Ambient);
	DDX_Slider(pDX, IDC_SPECULAR_SLIDER, m_Specular);
	DDX_Slider(pDX, IDC_SPECULAR_POWER_SLIDER, m_SpecularPower);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSurfacePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSurfacePage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_DIFFUSE_SLIDER, OnReleasedcaptureDiffuseSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_AMBIENT_SLIDER, OnReleasedcaptureAmbientSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SPECULAR_SLIDER, OnReleasedcaptureSpecularSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SPECULAR_POWER_SLIDER, OnReleasedcaptureSpecularPowerSlider)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSurfacePage message handlers

BOOL CSurfacePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_DiffuseSlider.SetRange(0, 100, TRUE);
	m_AmbientSlider.SetRange(0, 100, TRUE);
	m_SpecularSlider.SetRange(0, 100, TRUE);
	m_SpecularPowerSlider.SetRange(0, 100, TRUE);
	// The diffused parameter is currently hard wired to 1 and 
	// cannot be adjusted
	m_DiffuseSlider.EnableWindow(FALSE);
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSurfacePage::Reinitialize()
{
	m_Diffuse = 100;
	m_Ambient = 0;
	m_Specular = 0;
	m_SpecularPower = 1;
	UpdateData(FALSE);
	UpdateLabels();
	Activate(FALSE);
}

void CSurfacePage::Activate(BOOL b)
{
	//m_DiffuseSlider.EnableWindow(b);
	m_AmbientSlider.EnableWindow(b);
	m_SpecularSlider.EnableWindow(b);
	m_SpecularPowerSlider.EnableWindow(b);
}

void CSurfacePage::OnDefault()
{
	m_Diffuse = 100;
	m_Ambient = 0;
	m_Specular = 0;
	m_SpecularPower = 1;
	UpdateData(FALSE);
	UpdateLabels();
	m_pDoc->SetDiffuseLighting(m_Diffuse*0.01);
	m_pDoc->SetAmbientLighting(m_Ambient*0.01);
	m_pDoc->SetSpecularLighting(m_Specular*0.01);
	m_pDoc->SetSpecularPower(m_SpecularPower*1.0);
}

void CSurfacePage::OnReleasedcaptureDiffuseSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetDiffuseLighting(m_Diffuse*0.01);
	*pResult = 0;
}

void CSurfacePage::OnReleasedcaptureAmbientSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetAmbientLighting(m_Ambient*0.01);
	*pResult = 0;
}

void CSurfacePage::OnReleasedcaptureSpecularSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetSpecularLighting(m_Specular*0.01);
	*pResult = 0;
}

void CSurfacePage::OnReleasedcaptureSpecularPowerSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	m_pDoc->SetSpecularPower(m_SpecularPower*1.0);
	*pResult = 0;
}

void CSurfacePage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	char label[6];
	UpdateData(TRUE);
	if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_DIFFUSE_SLIDER))
	{
		sprintf(label,"%4.2f", m_Diffuse*.01);
		GetDlgItem(IDC_DIFFUSE)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_AMBIENT_SLIDER))
	{
		sprintf(label,"%4.2f", m_Ambient*.01);
		GetDlgItem(IDC_AMBIENT)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_SPECULAR_SLIDER))
	{
		sprintf(label,"%4.2f", m_Specular*.01);
		GetDlgItem(IDC_SPECULAR)->SetWindowText(label);
	}
	else if (pScrollBar == (CScrollBar *) GetDlgItem(IDC_SPECULAR_POWER_SLIDER))
	{
		sprintf(label,"%d", m_SpecularPower);
		GetDlgItem(IDC_SPECULAR_POWER)->SetWindowText(label);
	}
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSurfacePage::UpdateLabels()
{
	char label[6];
	sprintf(label,"%4.2f", m_Diffuse*.01);
	GetDlgItem(IDC_DIFFUSE)->SetWindowText(label);
	sprintf(label,"%4.2f", m_Ambient*.01);
	GetDlgItem(IDC_AMBIENT)->SetWindowText(label);
	sprintf(label,"%4.2f", m_Specular*.01);
	GetDlgItem(IDC_SPECULAR)->SetWindowText(label);
	sprintf(label,"%d", m_SpecularPower);
	GetDlgItem(IDC_SPECULAR_POWER)->SetWindowText(label);
}