// ColorBarTextPage.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "ColorBarTextPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarTextPage property page

IMPLEMENT_DYNCREATE(CColorBarTextPage, CPropertyPage)

CColorBarTextPage::CColorBarTextPage() : CPropertyPage(CColorBarTextPage::IDD)
{
	//{{AFX_DATA_INIT(CColorBarTextPage)
	//}}AFX_DATA_INIT
	m_ExchangeData = FALSE;
	m_FontSize = 0;
	m_NumLabels = 0;
	m_ColorOption = 0;
	m_Precision = 2;
}

CColorBarTextPage::~CColorBarTextPage()
{
}

void CColorBarTextPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBarTextPage)
	//}}AFX_DATA_MAP

	if (m_ExchangeData)
	{
		DDX_Text(pDX, IDC_FONT_SIZE, m_FontSize);
		DDX_Text(pDX, IDC_NUM_LABELS, m_NumLabels);
		DDX_Text(pDX, IDC_PRECISION, m_Precision);
		DDX_Radio(pDX, IDC_BLACK, m_ColorOption);
	}
}


BEGIN_MESSAGE_MAP(CColorBarTextPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColorBarTextPage)
	ON_BN_CLICKED(IDC_BLACK, OnBlack)
	ON_BN_CLICKED(IDC_GRAY, OnGray)
	ON_BN_CLICKED(IDC_WHITE, OnWhite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarTextPage message handlers

BOOL CColorBarTextPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBarTextPage::Reinitialize()
{
	GetDlgItem(IDC_FONT_SIZE)->SetWindowText("");
	GetDlgItem(IDC_NUM_LABELS)->SetWindowText("");
	GetDlgItem(IDC_PRECISION)->SetWindowText("");
	Activate(FALSE);
}


void CColorBarTextPage::Activate(BOOL b)
{
	GetDlgItem(IDC_FONT_SIZE)->EnableWindow(b);
	GetDlgItem(IDC_NUM_LABELS)->EnableWindow(b);
	GetDlgItem(IDC_PRECISION)->EnableWindow(b);
	GetDlgItem(IDC_BLACK)->EnableWindow(b);
	GetDlgItem(IDC_GRAY)->EnableWindow(b);
	GetDlgItem(IDC_WHITE)->EnableWindow(b);
}


BOOL CColorBarTextPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CColorBarTextPage::OnDefault() 
{
	m_ColorOption = 0;
	m_FontSize = 14;
	m_NumLabels = 5;
	m_Precision = 2;
	CustomUpdateData(FALSE);
	m_pDoc->SetColorBarFontSize(m_FontSize, FALSE);
	m_pDoc->SetColorBarNumberOfLabels(m_NumLabels, FALSE);
	m_pDoc->SetColorBarLabelPrecision(m_Precision);
	m_pDoc->SetColorBarTextColor(0, 0, 0);
}

void CColorBarTextPage::Apply() 
{
	if (CustomUpdateData(TRUE))
	{
		if (m_NumLabels < 0)
		{ 
			m_NumLabels = 0;
			CustomUpdateData(FALSE);
		}
		m_pDoc->SetColorBarFontSize(m_FontSize, FALSE);
		m_pDoc->SetColorBarNumberOfLabels(m_NumLabels, FALSE);
		m_pDoc->SetColorBarLabelPrecision(m_Precision);
	}
}

void CColorBarTextPage::OnBlack() 
{
	m_pDoc->SetColorBarTextColor(0, 0, 0);
}

void CColorBarTextPage::OnGray() 
{
	m_pDoc->SetColorBarTextColor(0.5f, 0.5f, 0.5f);
}

void CColorBarTextPage::OnWhite() 
{
	m_pDoc->SetColorBarTextColor(1, 1, 1);
}
