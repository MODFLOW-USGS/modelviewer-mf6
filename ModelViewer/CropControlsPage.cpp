// CropControlsPage.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "CropControlsPage.h"
#include "MvDoc.h"
#include "CropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCropControlsPage property page

IMPLEMENT_DYNCREATE(CCropControlsPage, CPropertyPage)

CCropControlsPage::CCropControlsPage() : CPropertyPage(CCropControlsPage::IDD)
{
	//{{AFX_DATA_INIT(CCropControlsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_XMin = 0.0;
	m_XMax = 1;
	m_XDelta = 0.1;
	m_XSync = FALSE;
	m_YMin = 0.0;
	m_YMax = 1;
	m_YDelta = 0.1;
	m_YSync = FALSE;
	m_ZMin = 0.0;
	m_ZMax = 1;
	m_ZDelta = 0.0;
	m_ZSync = FALSE;
	m_CropAngle = 0;
	m_ZDelta = 0.1;
	m_ExchangeData = FALSE;
}

CCropControlsPage::~CCropControlsPage()
{
}

void CCropControlsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCropControlsPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	if (m_ExchangeData)
	{
		DDX_Text(pDX, IDC_XDELTA, m_XDelta);
		DDV_MinMaxDouble(pDX, m_XDelta, 0.0, 1.0);
		DDX_Text(pDX, IDC_XMAX, m_XMax);
		DDV_MinMaxDouble(pDX, m_XMax, 0.0, 1.0);
		DDX_Text(pDX, IDC_XMIN, m_XMin);
		DDV_MinMaxDouble(pDX, m_XMin, 0.0, 1.0);
		DDX_Check(pDX, IDC_XSYNC, m_XSync);
		DDX_Text(pDX, IDC_YDELTA, m_YDelta);
		DDV_MinMaxDouble(pDX, m_YDelta, 0.0, 1.0);
		DDX_Text(pDX, IDC_YMAX, m_YMax);
		DDV_MinMaxDouble(pDX, m_YMax, 0.0, 1.0);
		DDX_Text(pDX, IDC_YMIN, m_YMin);
		DDV_MinMaxDouble(pDX, m_YMin, 0.0, 1.0);
		DDX_Check(pDX, IDC_YSYNC, m_YSync);
		DDX_Text(pDX, IDC_ZDELTA, m_ZDelta);
		DDV_MinMaxDouble(pDX, m_ZDelta, 0.0, 1.0);
		DDX_Text(pDX, IDC_ZMAX, m_ZMax);
		DDV_MinMaxDouble(pDX, m_ZMax, 0.0, 1.0);
		DDX_Text(pDX, IDC_ZMIN, m_ZMin);
		DDV_MinMaxDouble(pDX, m_ZMin, 0.0, 1.0);
		DDX_Check(pDX, IDC_ZSYNC, m_ZSync);
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


BEGIN_MESSAGE_MAP(CCropControlsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCropControlsPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XMIN_SPIN, OnDeltaposXminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_XMAX_SPIN, OnDeltaposXmaxSpin)
	ON_BN_CLICKED(IDC_XSYNC, OnXsync)
	ON_BN_CLICKED(IDC_YSYNC, OnYsync)
	ON_BN_CLICKED(IDC_ZSYNC, OnZsync)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CROP_ANGLE_SPIN, OnDeltaposCropAngleSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YMIN_SPIN, OnDeltaposYminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_YMAX_SPIN, OnDeltaposYmaxSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZMIN_SPIN, OnDeltaposZminSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZMAX_SPIN, OnDeltaposZmaxSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCropControlsPage message handlers

BOOL CCropControlsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCropControlsPage::Reinitialize()
{
	GetDlgItem(IDC_XMIN)->SetWindowText("");
	GetDlgItem(IDC_XMAX)->SetWindowText("");
	GetDlgItem(IDC_YMIN)->SetWindowText("");
	GetDlgItem(IDC_YMAX)->SetWindowText("");
	GetDlgItem(IDC_ZMIN)->SetWindowText("");
	GetDlgItem(IDC_ZMAX)->SetWindowText("");
	GetDlgItem(IDC_XDELTA)->SetWindowText("");
	GetDlgItem(IDC_YDELTA)->SetWindowText("");
	GetDlgItem(IDC_ZDELTA)->SetWindowText("");
	GetDlgItem(IDC_CROP_ANGLE)->SetWindowText("");
	Activate(FALSE);
}

void CCropControlsPage::Activate(BOOL b)
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
	GetDlgItem(IDC_XSYNC)->EnableWindow(b);
	GetDlgItem(IDC_YSYNC)->EnableWindow(b);
	GetDlgItem(IDC_ZSYNC)->EnableWindow(b);
	GetDlgItem(IDC_XMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_XMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_YMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_YMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_ZMIN_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_ZMAX_SPIN)->EnableWindow(b);
	GetDlgItem(IDC_CROP_ANGLE)->EnableWindow(b);
	GetDlgItem(IDC_CROP_ANGLE_SPIN)->EnableWindow(b);
	m_IsActive = b;
	if (m_Parent->m_PropertySheet->GetActiveIndex() == 0)
	{
		m_Parent->m_ApplyButton.EnableWindow(b);
	}
}


BOOL CCropControlsPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CCropControlsPage::OnDefault() 
{
	m_XMax = 1;
	m_XMin = 0;
	m_YMax = 1;
	m_YMin = 0;
	m_ZMax = 1;
	m_ZMin = 0;
	m_CropAngle = 0;
	m_XSync = FALSE;
	m_YSync = FALSE;
	m_ZSync = FALSE;
	CustomUpdateData(FALSE);
	m_pDoc->Crop(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
}

void CCropControlsPage::OnDeltaposXmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_XMax = __min(1, m_XMax + m_XDelta);
			if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
			{
				m_XMin = m_XMax;
			}
		}
		else
		{
			if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
			{
				m_XMin = __max(0, m_XMin - m_XDelta);
				m_XMax = m_XMin;
			}
			else
			{
				m_XMax = __max(m_XMin, m_XMax - m_XDelta);
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}	
	*pResult = 0;
}

void CCropControlsPage::OnDeltaposXminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
			{
				m_XMax = __min(1, m_XMax + m_XDelta);
				m_XMin = m_XMax;
			}
			else
			{
				m_XMin = __min(m_XMax, m_XMin + m_XDelta);
			}
		}
		else
		{
			m_XMin = __max(0, m_XMin - m_XDelta);
			if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
			{
				m_XMax = m_XMin;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CCropControlsPage::OnDeltaposYmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_YMax = __min(1, m_YMax + m_YDelta);
			if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
			{
				m_YMin = m_YMax;
			}
		}
		else
		{
			if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
			{
				m_YMin = __max(0, m_YMin - m_YDelta);
				m_YMax = m_YMin;
			}
			else
			{
				m_YMax = __max(m_YMin, m_YMax - m_YDelta);
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CCropControlsPage::OnDeltaposYminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
			{
				m_YMax = __min(1, m_YMax + m_YDelta);
				m_YMin = m_YMax;
			}
			else
			{
				m_YMin = __min(m_YMax, m_YMin + m_YDelta);
			}
		}
		else
		{
			m_YMin = __max(0, m_YMin - m_YDelta);
			if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
			{
				m_YMax = m_YMin;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CCropControlsPage::OnDeltaposZmaxSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_ZMax = __min(1, m_ZMax + m_ZDelta);
			if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
			{
				m_ZMin = m_ZMax;
			}
		}
		else
		{
			if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
			{
				m_ZMin = __max(0, m_ZMin - m_ZDelta);
				m_ZMax = m_ZMin;
			}
			else
			{
				m_ZMax = __max(m_ZMin, m_ZMax - m_ZDelta);
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CCropControlsPage::OnDeltaposZminSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
			{
				m_ZMax = __min(1, m_ZMax + m_ZDelta);
				m_ZMin = m_ZMax;
			}
			else
			{
				m_ZMin = __min(m_ZMax, m_ZMin + m_ZDelta);
			}
		}
		else
		{
			m_ZMin = __max(0, m_ZMin - m_ZDelta);
			if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
			{
				m_ZMax = m_ZMin;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CCropControlsPage::OnXsync() 
{
	if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
	{
		char buff[20];
		GetDlgItem(IDC_XMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_XMAX)->SetWindowText(buff);
	}
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->Crop(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
	}
}

void CCropControlsPage::OnYsync() 
{
	if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
	{
		char buff[20];
		GetDlgItem(IDC_YMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_YMAX)->SetWindowText(buff);
	}
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->Crop(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
	}
}

void CCropControlsPage::OnZsync() 
{
	if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
	{
		char buff[20];
		GetDlgItem(IDC_ZMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_ZMAX)->SetWindowText(buff);
	}
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->Crop(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
	}
}

void CCropControlsPage::OnDeltaposCropAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCropControlsPage::Apply() 
{
	char buff[20];
	if (((CButton *) GetDlgItem(IDC_XSYNC))->GetCheck() == 1)
	{
		GetDlgItem(IDC_XMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_XMAX)->SetWindowText(buff);
	}
	if (((CButton *) GetDlgItem(IDC_YSYNC))->GetCheck() == 1)
	{
		GetDlgItem(IDC_YMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_YMAX)->SetWindowText(buff);
	}
	if (((CButton *) GetDlgItem(IDC_ZSYNC))->GetCheck() == 1)
	{
		GetDlgItem(IDC_ZMIN)->GetWindowText(buff, 19);
		GetDlgItem(IDC_ZMAX)->SetWindowText(buff);
	}
	if (CustomUpdateData(TRUE))
	{
		m_pDoc->Crop(m_XMin, m_XMax, m_YMin, m_YMax, m_ZMin, m_ZMax, m_CropAngle);
	}
}

BOOL CCropControlsPage::OnSetActive() 
{
	m_Parent->m_ApplyButton.EnableWindow(m_IsActive);
	return CPropertyPage::OnSetActive();
}
