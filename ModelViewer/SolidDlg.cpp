// SolidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "SolidDlg.h"
#include "MvDoc.h"
#include "mvDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolidDlg dialog


CSolidDlg::CSolidDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CSolidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSolidDlg)
	m_SolidThresholdMin = 0.0;
	m_SolidThresholdMax = 0.0;
	m_ApplyThreshold = FALSE;
	m_SolidDisplayMode = -1;
	m_NumberOfColorBands = 0;
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CSolidDlg::IDD;
	m_PrimaryScalarMode = 0;
}


void CSolidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSolidDlg)
	DDX_Text(pDX, IDC_SOLID_THRESHOLD_MIN, m_SolidThresholdMin);
	DDX_Text(pDX, IDC_SOLID_THRESHOLD_MAX, m_SolidThresholdMax);
	DDX_Check(pDX, IDC_APPLY_THRESHOLD, m_ApplyThreshold);
	DDX_Radio(pDX, IDC_SOLID_DISPLAY_SMOOTH, m_SolidDisplayMode);
	DDX_Text(pDX, IDC_COLOR_BANDS, m_NumberOfColorBands);
	DDV_MinMaxInt(pDX, m_NumberOfColorBands, 2, 50);
	//}}AFX_DATA_MAP

	pDX->PrepareEditCtrl(IDC_SOLID_THRESHOLD_MAX);
	if (pDX->m_bSaveAndValidate)
	{
		if (m_SolidThresholdMax <= m_SolidThresholdMin)
		{
			AfxMessageBox("Please insure that \"Max Value\" is greater than \"Min Value\".", MB_ICONEXCLAMATION);
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(CSolidDlg, CDialog)
	//{{AFX_MSG_MAP(CSolidDlg)
	ON_BN_CLICKED(IDC_SOLID_DISPLAY_SMOOTH, OnSolidDisplaySmooth)
	ON_BN_CLICKED(IDC_SOLID_DISPLAY_BLOCKY, OnSolidDisplayBlocky)
	ON_BN_CLICKED(IDC_APPLY_THRESHOLD, OnApplyThreshold)
	ON_BN_CLICKED(IDC_FULL_SOLID, OnFullSolid)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_SOLID_DISPLAY_BANDED, OnSolidDisplayBanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSolidDlg message handlers

BOOL CSolidDlg::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CSolidDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CSolidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.right = screenWidth - 10;
	wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - dlgWidth;
	wndpl.rcNormalPosition.top = screenHeight/4 - 10;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

	Reinitialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSolidDlg::Reinitialize()
{
	m_ApplyThreshold = FALSE;
	m_SolidDisplayMode = 0;
	UpdateData(FALSE);
	GetDlgItem(IDC_SOLID_THRESHOLD_MAX)->SetWindowText("");
	GetDlgItem(IDC_SOLID_THRESHOLD_MIN)->SetWindowText("");
	GetDlgItem(IDC_COLOR_BANDS)->SetWindowText("");
	Activate(FALSE);
}

void CSolidDlg::Activate(BOOL b)
{
	GetDlgItem(IDC_SOLID_DISPLAY_SMOOTH)->EnableWindow(b && m_pDoc->GetGridType() != MV_UNSTRUCTURED_GRID);
	GetDlgItem(IDC_SOLID_DISPLAY_BLOCKY)->EnableWindow(b && m_PrimaryScalarMode == 0 && m_pDoc->GetGridType() != MV_UNSTRUCTURED_GRID);
	GetDlgItem(IDC_SOLID_DISPLAY_BANDED)->EnableWindow(b && m_pDoc->GetGridType() != MV_UNSTRUCTURED_GRID);
	GetDlgItem(IDC_APPLY_THRESHOLD)->EnableWindow(b);
	GetDlgItem(IDC_COLOR_BANDS)->EnableWindow(b && 
		((CButton *) GetDlgItem(IDC_SOLID_DISPLAY_BANDED))->GetCheck());
	GetDlgItem(IDC_APPLY)->EnableWindow(b);

	BOOL bb = ((CButton *) GetDlgItem(IDC_APPLY_THRESHOLD))->GetCheck();
	GetDlgItem(IDC_SOLID_THRESHOLD_MIN)->EnableWindow(b && bb);
	GetDlgItem(IDC_SOLID_THRESHOLD_MAX)->EnableWindow(b && bb);
	GetDlgItem(IDC_FULL_SOLID)->EnableWindow(b && bb);
}

void CSolidDlg::OnSolidDisplaySmooth() 
{
	m_SolidDisplayMode = 0;
	GetDlgItem(IDC_COLOR_BANDS)->EnableWindow(FALSE);
	m_pDoc->SetSolidDisplayToSmooth();
}

void CSolidDlg::OnSolidDisplayBlocky() 
{
	m_SolidDisplayMode = 1;
	GetDlgItem(IDC_COLOR_BANDS)->EnableWindow(FALSE);
	m_pDoc->SetSolidDisplayToBlocky();
}

void CSolidDlg::OnSolidDisplayBanded() 
{
	m_SolidDisplayMode = 3;
	GetDlgItem(IDC_COLOR_BANDS)->EnableWindow(TRUE);
	m_pDoc->SetSolidDisplayToBanded();
}

void CSolidDlg::OnApplyThreshold() 
{
	m_ApplyThreshold = ((CButton *) GetDlgItem(IDC_APPLY_THRESHOLD))->GetCheck();
	GetDlgItem(IDC_SOLID_THRESHOLD_MIN)->EnableWindow(m_ApplyThreshold);
	GetDlgItem(IDC_SOLID_THRESHOLD_MAX)->EnableWindow(m_ApplyThreshold);
	GetDlgItem(IDC_FULL_SOLID)->EnableWindow(m_ApplyThreshold);
	OnApply();
}

void CSolidDlg::OnFullSolid() 
{
	double range[2];
	m_pDoc->GetScalarDataRange(range);
	if (range[0] == range[1])
	{
		m_SolidThresholdMin = range[0]-1;
		m_SolidThresholdMax = range[1]+1;
	}
	else
	{
		m_SolidThresholdMin = range[0];
		m_SolidThresholdMax = range[1];
	} 
	UpdateData(FALSE);
	m_pDoc->ApplySolidControl(m_ApplyThreshold, m_SolidThresholdMin, m_SolidThresholdMax, m_NumberOfColorBands);
}

void CSolidDlg::OnApply() 
{
	if (UpdateData(TRUE))
	{
		m_pDoc->ApplySolidControl(m_ApplyThreshold, m_SolidThresholdMin, m_SolidThresholdMax, m_NumberOfColorBands);
	}
}

void CSolidDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}

BOOL CSolidDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Inactivates help when F1 is pressed	
	//return CDialog::OnHelpInfo(pHelpInfo);
	return 0;
}
