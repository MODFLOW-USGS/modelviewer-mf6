// ColorBarDataSource.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "ColorBarDataSource.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MV_DATASET 0
#define MV_PATHLINE 1

/////////////////////////////////////////////////////////////////////////////
// CColorBarDataSource property page
class CMvDoc;


IMPLEMENT_DYNCREATE(CColorBarDataSource, CPropertyPage)

CColorBarDataSource::CColorBarDataSource() : CPropertyPage(CColorBarDataSource::IDD)
{
	m_ExchangeData = FALSE;
	//{{AFX_DATA_INIT(CColorBarDataSource)
	m_DataSourceIndex = 0;
	//}}AFX_DATA_INIT
}

CColorBarDataSource::~CColorBarDataSource()
{
}

void CColorBarDataSource::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBarDataSource)
	DDX_CBIndex(pDX, IDC_COMBO_DATA_SOURCE, m_DataSourceIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBarDataSource, CPropertyPage)
	//{{AFX_MSG_MAP(CColorBarDataSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColorBarDataSource::Reinitialize()
{
	OnDefault();
	Activate(FALSE);
}

void CColorBarDataSource::Activate(BOOL b)
{
	GetDlgItem(IDC_COMBO_DATA_SOURCE)->EnableWindow(b  && m_pDoc->HasPathlineData());
}

void CColorBarDataSource::OnDefault() 
{
	m_DataSourceIndex = MV_DATASET;
	if (CustomUpdateData(FALSE))
	{
		m_pDoc->SetColorBarSource(m_DataSourceIndex);
		double ValueBlue = m_pDoc->GetColorBarValueBlue();
		double ValueRed = m_pDoc->GetColorBarValueRed();
		m_pDoc->SetColorBarEndPoints(ValueBlue, ValueRed);
		m_pDoc->UpdateColorBarDlg();
		m_pDoc->UpdatePathlinesDlg();
	}
}

void CColorBarDataSource::Apply() 
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->SetColorBarSource(m_DataSourceIndex);
		double ValueBlue = m_pDoc->GetColorBarValueBlue();
		double ValueRed = m_pDoc->GetColorBarValueRed();
		m_pDoc->SetColorBarEndPoints(ValueBlue, ValueRed);
		m_pDoc->UpdateColorBarDlg();
		m_pDoc->UpdatePathlinesDlg();
	}

}

BOOL CColorBarDataSource::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CColorBarDataSource message handlers

BOOL CColorBarDataSource::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
