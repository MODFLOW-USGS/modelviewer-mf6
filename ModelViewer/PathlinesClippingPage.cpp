// PathlinesClippingPage.cpp : implementation file
//

#include "modelviewer.h"
#include "PathlinesClippingPage.h"
#include "PathlinesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathlinesClippingPage property page

IMPLEMENT_DYNCREATE(CPathlinesClippingPage, CPropertyPage)

CPathlinesClippingPage::CPathlinesClippingPage() : CPropertyPage(CPathlinesClippingPage::IDD)
{
	//{{AFX_DATA_INIT(CPathlinesClippingPage)
	//}}AFX_DATA_INIT
	m_ExchangeData = FALSE;
	m_MaximumTime = 0;
	m_MinimumTime = 0;
	m_ClippingMode = 0;
}

CPathlinesClippingPage::~CPathlinesClippingPage()
{
}

void CPathlinesClippingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathlinesClippingPage)
	//}}AFX_DATA_MAP
	if (m_ExchangeData)
	{
		DDX_Text(pDX, IDC_MAXIMUM_TIME, m_MaximumTime);
		DDX_Text(pDX, IDC_MINIMUM_TIME, m_MinimumTime);
		DDX_Radio(pDX, IDC_NO_PATHLINE_CLIPPING, m_ClippingMode);
	}
}


BEGIN_MESSAGE_MAP(CPathlinesClippingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPathlinesClippingPage)
	ON_BN_CLICKED(IDC_NO_PATHLINE_CLIPPING, OnNoPathlineClipping)
	ON_BN_CLICKED(IDC_CLIP_BY_SIMTIME, OnClipBySimtime)
	ON_BN_CLICKED(IDC_CLIP_BY_TIME_RANGE, OnClipByTimeRange)
	ON_BN_CLICKED(IDC_CLIP_BY_MAXIMUM_TIME, OnClipByMaximumTime)
	ON_BN_CLICKED(IDC_CLIP_BY_MINIMUM_TIME, OnClipByMinimumTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathlinesClippingPage message handlers

BOOL CPathlinesClippingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathlinesClippingPage::Reinitialize()
{
	m_ClippingMode = 0;
	CustomUpdateData(FALSE);
	GetDlgItem(IDC_MINIMUM_TIME)->SetWindowText("");
	GetDlgItem(IDC_MAXIMUM_TIME)->SetWindowText("");
	Activate(FALSE);
}

void CPathlinesClippingPage::Activate(BOOL b)
{
	GetDlgItem(IDC_NO_PATHLINE_CLIPPING)->EnableWindow(b);
	GetDlgItem(IDC_CLIP_BY_SIMTIME)->EnableWindow(b);
	GetDlgItem(IDC_CLIP_BY_TIME_RANGE)->EnableWindow(b);
	GetDlgItem(IDC_CLIP_BY_MINIMUM_TIME)->EnableWindow(b);
	GetDlgItem(IDC_CLIP_BY_MAXIMUM_TIME)->EnableWindow(b);
	BOOL bb = 
		((CButton *) GetDlgItem(IDC_CLIP_BY_TIME_RANGE))->GetCheck()
		|| ((CButton *) GetDlgItem(IDC_CLIP_BY_MINIMUM_TIME))->GetCheck()
		|| ((CButton *) GetDlgItem(IDC_CLIP_BY_MAXIMUM_TIME))->GetCheck();
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(b && bb);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(b && bb);
	m_IsActive = b;
}

BOOL CPathlinesClippingPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CPathlinesClippingPage::Apply() 
{
	if (((CButton *) GetDlgItem(IDC_CLIP_BY_TIME_RANGE))->GetCheck()
		|| ((CButton *) GetDlgItem(IDC_CLIP_BY_MINIMUM_TIME))->GetCheck()
		|| ((CButton *) GetDlgItem(IDC_CLIP_BY_MAXIMUM_TIME))->GetCheck())
	{
		if (CustomUpdateData(TRUE))
		{
			m_pDoc->SetPathlineTimeClippingRange(m_MinimumTime, m_MaximumTime);
		}
	}
}

void CPathlinesClippingPage::OnNoPathlineClipping() 
{
	m_ClippingMode = 0;
	m_pDoc->SetPathlineTimeClippingMode(m_ClippingMode);
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(FALSE);
	m_Parent->m_ApplyButton.EnableWindow(FALSE);
	m_Parent->m_DefaultButton.EnableWindow(FALSE);
}

void CPathlinesClippingPage::OnClipBySimtime() 
{
	m_ClippingMode = 1;
	m_pDoc->SetPathlineTimeClippingMode(m_ClippingMode);
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(FALSE);
	m_Parent->m_ApplyButton.EnableWindow(FALSE);
	m_Parent->m_DefaultButton.EnableWindow(FALSE);
}

void CPathlinesClippingPage::OnClipByTimeRange() 
{
	m_ClippingMode = 2;
	m_pDoc->SetPathlineTimeClippingMode(m_ClippingMode);
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(TRUE);
	m_Parent->m_ApplyButton.EnableWindow(TRUE);
	m_Parent->m_DefaultButton.EnableWindow(TRUE);
}

void CPathlinesClippingPage::OnDefault() 
{
	double range[2];
	m_pDoc->GetPathlineTimeRange(range);
	if (range[0] == range[1])
	{
		m_MinimumTime = range[0]-1;
		m_MaximumTime = range[1]+1;
	}
	else
	{
		m_MinimumTime = range[0];
		m_MaximumTime = range[1];
	}
	CustomUpdateData(FALSE);
	Apply();
}

BOOL CPathlinesClippingPage::OnSetActive() 
{
	BOOL b = ((CButton *) GetDlgItem(IDC_CLIP_BY_TIME_RANGE))->GetCheck()
		 || ((CButton *) GetDlgItem(IDC_CLIP_BY_MINIMUM_TIME))->GetCheck()
		 || ((CButton *) GetDlgItem(IDC_CLIP_BY_MAXIMUM_TIME))->GetCheck();
	m_Parent->m_ApplyButton.EnableWindow(b && m_IsActive);
	m_Parent->m_DefaultButton.EnableWindow(b && m_IsActive);
	return CPropertyPage::OnSetActive();
}

void CPathlinesClippingPage::OnClipByMaximumTime() 
{
	m_ClippingMode = 4;
	m_pDoc->SetPathlineTimeClippingMode(m_ClippingMode);
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(TRUE);
	m_Parent->m_ApplyButton.EnableWindow(TRUE);
	m_Parent->m_DefaultButton.EnableWindow(TRUE);
	
}

void CPathlinesClippingPage::OnClipByMinimumTime() 
{
	m_ClippingMode = 3;
	m_pDoc->SetPathlineTimeClippingMode(m_ClippingMode);
	GetDlgItem(IDC_MINIMUM_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_MAXIMUM_TIME)->EnableWindow(TRUE);
	m_Parent->m_ApplyButton.EnableWindow(TRUE);
	m_Parent->m_DefaultButton.EnableWindow(TRUE);
	
}
