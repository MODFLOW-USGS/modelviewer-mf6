// ScalePage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "ScalePage.h"
#include "MvDoc.h"
#include "GeometryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScalePage property page

IMPLEMENT_DYNCREATE(CScalePage, CPropertyPage)

CScalePage::CScalePage() : CPropertyPage(CScalePage::IDD)
{
	//{{AFX_DATA_INIT(CScalePage)
	m_XScale = 0.0;
	m_YScale = 0.0;
	m_ZScale = 0.0;
	//}}AFX_DATA_INIT
}

CScalePage::~CScalePage()
{
}

void CScalePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScalePage)
	DDX_Text(pDX, IDC_XSCALE, m_XScale);
	DDX_Text(pDX, IDC_YSCALE, m_YScale);
	DDX_Text(pDX, IDC_ZSCALE, m_ZScale);
	//}}AFX_DATA_MAP

	pDX->PrepareEditCtrl(IDC_XSCALE);
	if (pDX->m_bSaveAndValidate)
	{
		if (m_XScale <= 0)
		{
			AfxMessageBox("Please enter a positive number.", MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
	pDX->PrepareEditCtrl(IDC_YSCALE);
	if (pDX->m_bSaveAndValidate)
	{
		if (m_YScale <= 0)
		{
			AfxMessageBox("Please enter a positive number.", MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
	pDX->PrepareEditCtrl(IDC_ZSCALE);
	if (pDX->m_bSaveAndValidate)
	{
		if (m_ZScale <= 0)
		{
			AfxMessageBox("Please enter a positive number.", MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(CScalePage, CPropertyPage)
	//{{AFX_MSG_MAP(CScalePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScalePage message handlers

BOOL CScalePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScalePage::Reinitialize()
{
	m_XScale = 1.0;
	m_YScale = 1.0;
	m_ZScale = 1.0;
	UpdateData(FALSE);
	Activate(FALSE);
}

void CScalePage::Activate(BOOL b)
{
	GetDlgItem(IDC_XSCALE)->EnableWindow(b);
	GetDlgItem(IDC_YSCALE)->EnableWindow(b);
	GetDlgItem(IDC_ZSCALE)->EnableWindow(b);
	m_IsActive = b;
	if (m_Parent->m_PropertySheet->GetActiveIndex() == 0)
	{
		m_Parent->m_ApplyButton.EnableWindow(b);
	}
}

void CScalePage::Apply() 
{
	if (UpdateData(TRUE))
	{
		m_pDoc->SetScale(m_XScale, m_YScale, m_ZScale);
	}
}

BOOL CScalePage::OnSetActive() 
{
	m_Parent->m_ApplyButton.EnableWindow(m_IsActive);
	return CPropertyPage::OnSetActive();
}
