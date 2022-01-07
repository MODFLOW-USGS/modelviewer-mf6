// PathlinesDisplayPage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "PathlinesDisplayPage.h"
#include "PathlinesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDisplayPage property page

IMPLEMENT_DYNCREATE(CPathlinesDisplayPage, CPropertyPage)

CPathlinesDisplayPage::CPathlinesDisplayPage() : CPropertyPage(CPathlinesDisplayPage::IDD)
{
	//{{AFX_DATA_INIT(CPathlinesDisplayPage)
	//}}AFX_DATA_INIT
	m_PathlineRepresentation = 0;
	m_TubeDiameter = 1.0;
	m_ExchangeData = FALSE;
}

CPathlinesDisplayPage::~CPathlinesDisplayPage()
{
}

void CPathlinesDisplayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathlinesDisplayPage)
	//}}AFX_DATA_MAP
	if (m_ExchangeData)
	{
		DDX_Radio(pDX, IDC_TUBE, m_PathlineRepresentation);
		DDX_Text(pDX, IDC_TUBE_DIAMETER, m_TubeDiameter);
	}
}


BEGIN_MESSAGE_MAP(CPathlinesDisplayPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPathlinesDisplayPage)
	ON_BN_CLICKED(IDC_LINE, OnLine)
	ON_BN_CLICKED(IDC_TUBE, OnTube)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDisplayPage message handlers

BOOL CPathlinesDisplayPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathlinesDisplayPage::Reinitialize()
{
	/*
	m_Representation = 0;
	((CButton *) GetDlgItem(IDC_LINE))->SetCheck(TRUE);
	((CButton *) GetDlgItem(IDC_TUBE))->SetCheck(FALSE);
	GetDlgItem(IDC_TUBE_DIAMETER)->SetWindowText("");
	*/
	Activate(FALSE);
}

void CPathlinesDisplayPage::Activate(BOOL b)
{
	GetDlgItem(IDC_LINE)->EnableWindow(b);
	GetDlgItem(IDC_TUBE)->EnableWindow(b);
	GetDlgItem(IDC_TUBE_DIAMETER)->EnableWindow(b &&
					((CButton *) GetDlgItem(IDC_TUBE))->GetCheck());
	m_IsActive = b;
}

BOOL CPathlinesDisplayPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CPathlinesDisplayPage::Apply() 
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->SetPathlineTubeDiameter(m_TubeDiameter);
	}
}

void CPathlinesDisplayPage::OnLine() 
{
	m_pDoc->SetPathlineRepresentationToLine();	
	GetDlgItem(IDC_TUBE_DIAMETER)->EnableWindow(FALSE);
	m_Parent->m_ApplyButton.EnableWindow(FALSE);
}

void CPathlinesDisplayPage::OnTube() 
{
	m_pDoc->SetPathlineRepresentationToTube();
	GetDlgItem(IDC_TUBE_DIAMETER)->EnableWindow(TRUE);
	m_Parent->m_ApplyButton.EnableWindow(TRUE);
}

BOOL CPathlinesDisplayPage::OnSetActive() 
{
	BOOL b = ((CButton *) GetDlgItem(IDC_TUBE))->GetCheck();
	m_Parent->m_ApplyButton.EnableWindow(b && m_IsActive);
	m_Parent->m_DefaultButton.EnableWindow(FALSE);
	return CPropertyPage::OnSetActive();
}
