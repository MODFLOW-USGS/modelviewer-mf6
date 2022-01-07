// VectorOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "VectorOptionsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorOptionsPage property page

IMPLEMENT_DYNCREATE(CVectorOptionsPage, CPropertyPage)

CVectorOptionsPage::CVectorOptionsPage() : CPropertyPage(CVectorOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CVectorOptionsPage)
	//}}AFX_DATA_INIT
	m_ScaleFactor = 0.0;
	m_ShowGlyph = FALSE;
	m_ColorOption = 0;
	m_LogTransform = FALSE;
	m_ExchangeData = FALSE;
}

CVectorOptionsPage::~CVectorOptionsPage()
{
}

void CVectorOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVectorOptionsPage)
	//}}AFX_DATA_MAP
	if (m_ExchangeData)
	{
		DDX_Radio(pDX, IDC_BLACK, m_ColorOption);
		DDX_Check(pDX, IDC_SHOW_GLYPH, m_ShowGlyph);
		DDX_Text(pDX, IDC_SCALE_FACTOR, m_ScaleFactor);
		DDX_Check(pDX, IDC_LogTransform, m_LogTransform);
		DDX_Text(pDX, IDC_LINE_WIDTH, m_LineWidth);
	}
}


BEGIN_MESSAGE_MAP(CVectorOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CVectorOptionsPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_GLYPH_SPIN, OnDeltaposGlyphSpin)
	ON_BN_CLICKED(IDC_SHOW_GLYPH, OnShowGlyph)
	ON_BN_CLICKED(IDC_BLACK, OnBlack)
	ON_BN_CLICKED(IDC_GRAY, OnGray)
	ON_BN_CLICKED(IDC_WHITE, OnWhite)
	ON_BN_CLICKED(IDC_LogTransform, OnLogTransform)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorOptionsPage message handlers

BOOL CVectorOptionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVectorOptionsPage::Reinitialize()
{
	m_ColorOption = 0;
	m_ShowGlyph = FALSE;
	CustomUpdateData(FALSE);
	GetDlgItem(IDC_SCALE_FACTOR)->SetWindowText("");
	Activate(FALSE);
}

void CVectorOptionsPage::Activate(BOOL b)
{
	GetDlgItem(IDC_SCALE_FACTOR)->EnableWindow(b);
	GetDlgItem(IDC_SHOW_GLYPH)->EnableWindow(b);
	BOOL bb = ((CButton *) GetDlgItem(IDC_SHOW_GLYPH))->GetCheck();
	GetDlgItem(IDC_GLYPH_SPIN)->EnableWindow(b && bb);
	GetDlgItem(IDC_BLACK)->EnableWindow(b);
	GetDlgItem(IDC_GRAY)->EnableWindow(b);
	GetDlgItem(IDC_WHITE)->EnableWindow(b);
	GetDlgItem(IDC_LogTransform)->EnableWindow(b);
	GetDlgItem(IDC_LINE_WIDTH)->EnableWindow(b);
}


BOOL CVectorOptionsPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CVectorOptionsPage::OnDeltaposGlyphSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (pNMUpDown->iDelta < 0)
	{
		m_pDoc->EnlargeVectorGlyph();
	}
	else
	{
		m_pDoc->ShrinkVectorGlyph();
	}
	*pResult = 0;
}

void CVectorOptionsPage::OnShowGlyph() 
{
	if (((CButton *) GetDlgItem(IDC_SHOW_GLYPH))->GetCheck())
	{
		m_pDoc->ActivateVectorGlyph(TRUE);	
		GetDlgItem(IDC_GLYPH_SPIN)->EnableWindow(TRUE);
	}
	else
	{
		m_pDoc->ActivateVectorGlyph(FALSE);
		GetDlgItem(IDC_GLYPH_SPIN)->EnableWindow(FALSE);
	}
}

void CVectorOptionsPage::OnBlack() 
{
	m_pDoc->SetVectorColor(0, 0, 0);
}

void CVectorOptionsPage::OnGray() 
{
	m_pDoc->SetVectorColor(0.5, 0.5, 0.5);
}

void CVectorOptionsPage::OnWhite() 
{
	m_pDoc->SetVectorColor(1, 1, 1);
}

void CVectorOptionsPage::OnDefault()
{
	m_LogTransform = FALSE;
	m_pDoc->SetLogTransformVector(m_LogTransform);
	m_pDoc->SetVectorSizeToOptimal();
	m_ScaleFactor = m_pDoc->GetVectorScaleFactor();
	m_LineWidth = 1;
	m_ColorOption = 0;
	m_ShowGlyph = FALSE;
	CustomUpdateData(FALSE);
	m_pDoc->SetVectorColor(0, 0, 0);
	m_pDoc->ActivateVectorGlyph(0);
	GetDlgItem(IDC_GLYPH_SPIN)->EnableWindow(FALSE);
}

void CVectorOptionsPage::Apply()
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->SetVectorScaleFactor(m_ScaleFactor);
		m_pDoc->SetVectorLineWidth(m_LineWidth);
	}
}

void CVectorOptionsPage::OnLogTransform() 
{
	int b = ((CButton *) GetDlgItem(IDC_LogTransform))->GetCheck();
	m_LogTransform = b;
	m_pDoc->SetLogTransformVector(b);
	m_ScaleFactor = m_pDoc->GetVectorScaleFactor();
	CustomUpdateData(FALSE);

}
