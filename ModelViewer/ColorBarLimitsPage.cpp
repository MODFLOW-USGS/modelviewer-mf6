// ColorBarLimitsPage.cpp : implementation file
//

#include "modelviewer.h"
#include "ColorBarLimitsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarLimitsPage property page

IMPLEMENT_DYNCREATE(CColorBarLimitsPage, CPropertyPage)

CColorBarLimitsPage::CColorBarLimitsPage() : CPropertyPage(CColorBarLimitsPage::IDD)
{
	//{{AFX_DATA_INIT(CColorBarLimitsPage)
	//}}AFX_DATA_INIT
	m_ExchangeData = FALSE;
	m_ValueBlue = 0.0;
	m_ValueRed = 0.0;
}

CColorBarLimitsPage::~CColorBarLimitsPage()
{
}

void CColorBarLimitsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBarLimitsPage)
	DDX_Control(pDX, IDC_LOG_SCALE, m_LogScaleCheckBox);
	//}}AFX_DATA_MAP

	if (m_ExchangeData)
	{
		DDX_Text(pDX, IDC_VALUE_BLUE, m_ValueBlue);
		DDX_Text(pDX, IDC_VALUE_RED, m_ValueRed);

		pDX->PrepareEditCtrl(IDC_VALUE_RED);
		if (pDX->m_bSaveAndValidate)
		{
			if (m_LogScaleCheckBox.GetCheck() && m_ValueRed <= 0)
			{
				AfxMessageBox("Please insure that \"Value at red limit\" is positive for logarithmic scaling.", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
			if (m_ValueRed == m_ValueBlue)
			{
				AfxMessageBox("Please insure that \"Value at red limit\" is different than \"Value at blue limit\".", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}

		pDX->PrepareEditCtrl(IDC_VALUE_BLUE);
		if (pDX->m_bSaveAndValidate)
		{
			if (m_LogScaleCheckBox.GetCheck() && m_ValueBlue <= 0)
			{
				AfxMessageBox("Please insure that \"Value at blue limit\" is positive for logarithmic scaling.", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CColorBarLimitsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColorBarLimitsPage)
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_LOG_SCALE, OnLogScale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarLimitsPage message handlers

BOOL CColorBarLimitsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBarLimitsPage::Reinitialize()
{
	GetDlgItem(IDC_VALUE_RED)->SetWindowText("");
	GetDlgItem(IDC_VALUE_BLUE)->SetWindowText("");
	m_LogScaleCheckBox.SetCheck(FALSE);
	Activate(FALSE);
}

void CColorBarLimitsPage::Activate(BOOL b)
{
	GetDlgItem(IDC_VALUE_BLUE)->EnableWindow(b);
	GetDlgItem(IDC_VALUE_RED)->EnableWindow(b);
	GetDlgItem(IDC_REVERSE)->EnableWindow(b);
	m_LogScaleCheckBox.EnableWindow(b);
}

BOOL CColorBarLimitsPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CColorBarLimitsPage::OnDefault() 
{
	double range[2];

	if (m_pDoc->GetColorBarSource() == 0)

	{
		m_pDoc->GetScalarDataRange(range);
	}
	else
	{
		m_pDoc->GetPathlineTimeRange(range);
	}
	if (m_ValueBlue < m_ValueRed)
	{
		if (range[0] == range[1])
		{
			m_ValueBlue = range[0]-1;
			m_ValueRed = range[1]+1;
		}
		else
		{
			m_ValueBlue = range[0];
			m_ValueRed = range[1];
		}
	}
	else
	{
		if (range[0] == range[1])
		{
			m_ValueBlue = range[1]+1;
			m_ValueRed = range[0]-1;
		}
		else
		{
			m_ValueBlue = range[1];
			m_ValueRed = range[0];
		}
	}
	m_LogScaleCheckBox.SetCheck(FALSE);
	CustomUpdateData(FALSE);
	m_pDoc->UseLinearColorBar();
	m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
}

void CColorBarLimitsPage::Apply() 
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
		m_pDoc->UpdatePathlinesDlg();
	}
}

void CColorBarLimitsPage::OnReverse() 
{
	if (CustomUpdateData(TRUE))
	{
		double temp = m_ValueRed;
		m_ValueRed = m_ValueBlue;
		m_ValueBlue = temp;
		CustomUpdateData(FALSE);
		m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
	}
}

void CColorBarLimitsPage::OnLogScale() 
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->SetColorBarEndPoints(m_ValueBlue, m_ValueRed);
		if (m_pDoc->GetColorBarSource() == 0)
		{
			if (m_LogScaleCheckBox.GetCheck())
			{
				m_pDoc->UseLogColorBar();
			}
			else
			{
				m_pDoc->UseLinearColorBar();
			}
		}
		else
		{
			m_pDoc->SetPathlineLogTransform(m_LogScaleCheckBox.GetCheck());
			m_pDoc->UpdatePathlinesDlg();
		}
	}
	else
	{
		m_LogScaleCheckBox.SetCheck(FALSE);
	}
}
