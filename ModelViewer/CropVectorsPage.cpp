// CropVectorsPage.cpp : implementation file
//

#include "modelviewer.h"
#include "CropVectorsPage.h"
#include "VectorDlg.h"

#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCropVectorsPage property page

IMPLEMENT_DYNCREATE(CCropVectorsPage, CPropertyPage)

CCropVectorsPage::CCropVectorsPage() : CPropertyPage(CCropVectorsPage::IDD)
{
	//{{AFX_DATA_INIT(CCropVectorsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_XMin = 0.0;
	m_XMax = 1.0;
	m_XDelta = 0.1;
	m_YMin = 0.0;
	m_YMax = 1.0;
	m_YDelta = 0.1;
	m_ZMin = 0.0;
	m_ZMax = 1.0;
	m_ZDelta = 0.0;
	m_ZDelta = 0.1;
	m_CropAngle = 0;
	m_ExchangeData = FALSE;

}

CCropVectorsPage::~CCropVectorsPage()
{
}

void CCropVectorsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCropVectorsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	if (m_ExchangeData)
	{
		DDX_Text(pDX, IDC_XDELTA, m_XDelta);
		DDV_MinMaxDouble(pDX, m_XDelta, 0.f, 1.f);
		DDX_Text(pDX, IDC_XMAX, m_XMax);
		DDV_MinMaxDouble(pDX, m_XMax, 0.f, 1.f);
		DDX_Text(pDX, IDC_XMIN, m_XMin);
		DDV_MinMaxDouble(pDX, m_XMin, 0.f, 1.f);
		DDX_Text(pDX, IDC_YDELTA, m_YDelta);
		DDV_MinMaxDouble(pDX, m_YDelta, 0.f, 1.f);
		DDX_Text(pDX, IDC_YMAX, m_YMax);
		DDV_MinMaxDouble(pDX, m_YMax, 0.f, 1.f);
		DDX_Text(pDX, IDC_YMIN, m_YMin);
		DDV_MinMaxDouble(pDX, m_YMin, 0.f, 1.f);
		DDX_Text(pDX, IDC_ZDELTA, m_ZDelta);
		DDV_MinMaxDouble(pDX, m_ZDelta, 0.f, 1.f);
		DDX_Text(pDX, IDC_ZMAX, m_ZMax);
		DDV_MinMaxDouble(pDX, m_ZMax, 0.f, 1.f);
		DDX_Text(pDX, IDC_ZMIN, m_ZMin);
		DDV_MinMaxDouble(pDX, m_ZMin, 0.f, 1.f);
		DDX_Text(pDX, IDC_CROP_ANGLE, m_CropAngle);
		DDV_MinMaxInt(pDX, m_CropAngle, -45, 45);

		pDX->PrepareEditCtrl(IDC_XMAX);
		if (pDX->m_bSaveAndValidate)
		{
			if (m_XMax < m_XMin)
			{
				AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
		pDX->PrepareEditCtrl(IDC_YMAX);
		if (pDX->m_bSaveAndValidate)
		{
			if (m_YMax < m_YMin)
			{
				AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
		pDX->PrepareEditCtrl(IDC_ZMAX);
		if (pDX->m_bSaveAndValidate)
		{
			if (m_ZMax < m_ZMin)
			{
				AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				pDX->Fail();
			}
		}
	}

}


BEGIN_MESSAGE_MAP(CCropVectorsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCropVectorsPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XMIN_SPIN, OnDeltaposXminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XMAX_SPIN, OnDeltaposXmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YMIN_SPIN, OnDeltaposYminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YMAX_SPIN, OnDeltaposYmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZMIN_SPIN, OnDeltaposZminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZMAX_SPIN, OnDeltaposZmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CROP_ANGLE_SPIN, OnDeltaposCropAngleSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropVectorsPage message handlers

void CCropVectorsPage::OnDeltaposXminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_XMin = __min(m_XMax, m_XMin + m_XDelta);
		}
		else
		{
			m_XMin = __max(0, m_XMin - m_XDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

BOOL CCropVectorsPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;

}

void CCropVectorsPage::Apply()
{
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->CropVectors(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
	}
}

void CCropVectorsPage::OnDeltaposXmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_XMax = __min(1, m_XMax + m_XDelta);
		}
		else
		{
			m_XMax = __max(m_XMin, m_XMax - m_XDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}	
	
	*pResult = 0;
}

void CCropVectorsPage::OnDeltaposYminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_YMin = __min(m_YMax, m_YMin + m_YDelta);
		}
		else
		{
			m_YMin = __max(0, m_YMin - m_YDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	
	*pResult = 0;
}

void CCropVectorsPage::OnDeltaposYmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_YMax = __min(1, m_YMax + m_YDelta);
		}
		else
		{
			m_YMax = __max(m_YMin, m_YMax - m_YDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	
	*pResult = 0;
}

void CCropVectorsPage::OnDeltaposZminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_ZMin = __min(m_ZMax, m_ZMin + m_ZDelta);
		}
		else
		{
			m_ZMin = __max(0, m_ZMin - m_ZDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	
	*pResult = 0;
}

void CCropVectorsPage::OnDeltaposZmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_ZMax = __min(1, m_ZMax + m_ZDelta);
		}
		else
		{
			m_ZMax = __max(m_ZMin, m_ZMax - m_ZDelta);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	
	*pResult = 0;
}

BOOL CCropVectorsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCropVectorsPage::Reinitialize()
{
	AssignDefaultValues();
/*	GetDlgItem(IDC_XMIN)->SetWindowText("");
	GetDlgItem(IDC_XMAX)->SetWindowText("");
	GetDlgItem(IDC_YMIN)->SetWindowText("");
	GetDlgItem(IDC_YMAX)->SetWindowText("");
	GetDlgItem(IDC_ZMIN)->SetWindowText("");
	GetDlgItem(IDC_ZMAX)->SetWindowText("");
	GetDlgItem(IDC_XDELTA)->SetWindowText("");
	GetDlgItem(IDC_YDELTA)->SetWindowText("");
	GetDlgItem(IDC_ZDELTA)->SetWindowText("");*/
	Activate(FALSE);
}

void CCropVectorsPage::Activate(BOOL b)
{
	GetDlgItem(IDC_XMIN)->EnableWindow(b);
	GetDlgItem(IDC_XMAX)->EnableWindow(b);
	GetDlgItem(IDC_YMIN)->EnableWindow(b);
	GetDlgItem(IDC_YMAX)->EnableWindow(b);
	GetDlgItem(IDC_ZMIN)->EnableWindow(b);
	GetDlgItem(IDC_ZMAX)->EnableWindow(b);
	GetDlgItem(IDC_XDELTA)->EnableWindow(b);
	GetDlgItem(IDC_YDELTA)->EnableWindow(b);
	GetDlgItem(IDC_ZDELTA)->EnableWindow(b);
	GetDlgItem(IDC_XMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_XMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_YMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_YMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_ZMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_ZMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_CROP_ANGLE)->EnableWindow(b);
	GetDlgItem(IDC_CROP_ANGLE_SPIN)->EnableWindow(b);
	m_IsActive = b;
}
void CCropVectorsPage::AssignDefaultValues()
{
	m_XMin = 0.0;
	m_XMax = 1.0;
	m_XDelta = 0.1;
	m_YMin = 0.0;
	m_YMax = 1.0;
	m_YDelta = 0.1;
	m_ZMin = 0.0;
	m_ZMax = 1.0;
	m_ZDelta = 0.0;
	m_ZDelta = 0.1;
	m_CropAngle = 0.0;
	CustomUpdateData(FALSE);
}


void CCropVectorsPage::OnDefault()
{
	AssignDefaultValues();
	m_pDoc->CropVectors(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
}


void CCropVectorsPage::OnDeltaposCropAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_CropAngle = __min(45, m_CropAngle + 1);
		}
		else
		{
			m_CropAngle = __max(-45, m_CropAngle - 1);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	
	*pResult = 0;
}
