// SubgridPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "SubgridPage.h"
#include "MvDoc.h"
#include "GridDlg.h"
#include "mvDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubgridPage property page

IMPLEMENT_DYNCREATE(CSubgridPage, CPropertyPage)

CSubgridPage::CSubgridPage() : CPropertyPage(CSubgridPage::IDD)
{
	//{{AFX_DATA_INIT(CSubgridPage)
	//}}AFX_DATA_INIT
	m_col_min = 0;
	m_col_max = 0;
	m_row_min = 0;
	m_row_max = 0;
	m_lay_min = 0;
	m_lay_max = 0;
	m_ExchangeData = FALSE;
	m_ActivateSubgrid = FALSE;
}

CSubgridPage::~CSubgridPage()
{
}

void CSubgridPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubgridPage)
	//}}AFX_DATA_MAP
	if (m_ExchangeData)
	{
		DDX_Check(pDX, IDC_ACTIVATE_SUBGRID, m_ActivateSubgrid);

      if (m_pDoc->GetGridType() == MV_STRUCTURED_GRID)
      {
		   DDX_Text(pDX, IDC_ILOW, m_col_min);
		   DDX_Text(pDX, IDC_IHIGH, m_col_max);
		   DDX_Text(pDX, IDC_JLOW, m_row_min);
		   DDX_Text(pDX, IDC_JHIGH, m_row_max);
		   pDX->PrepareEditCtrl(IDC_IHIGH);
		   if (pDX->m_bSaveAndValidate)
		   {
			   if (m_col_max < m_col_min)
			   {
				   AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				   pDX->Fail();
			   }
		   }
		   pDX->PrepareEditCtrl(IDC_JHIGH);
		   if (pDX->m_bSaveAndValidate)
		   {
			   if (m_row_max < m_row_min)
			   {
				   AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				   pDX->Fail();
			   }
		   }
      }
      if (m_pDoc->GetGridType() == MV_STRUCTURED_GRID || m_pDoc->GetGridType() == MV_LAYERED_GRID)
      {
		   DDX_Text(pDX, IDC_KLOW, m_lay_min);
		   DDX_Text(pDX, IDC_KHIGH, m_lay_max);
		   pDX->PrepareEditCtrl(IDC_KHIGH);
		   if (pDX->m_bSaveAndValidate)
		   {
			   if (m_lay_max < m_lay_min)
			   {
				   AfxMessageBox("Please insure that \"Max\" value is greater than or equal to \"Min\" value.", MB_ICONEXCLAMATION);
				   pDX->Fail();
			   }
		   }
      }
	}
}


BEGIN_MESSAGE_MAP(CSubgridPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSubgridPage)
	ON_BN_CLICKED(IDC_ACTIVATE_SUBGRID, OnActivateSubgrid)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ILOW_SPIN, OnDeltaposIlowSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IHIGH_SPIN, OnDeltaposIhighSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_JLOW_SPIN, OnDeltaposJlowSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_JHIGH_SPIN, OnDeltaposJhighSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_KLOW_SPIN, OnDeltaposKlowSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_KHIGH_SPIN, OnDeltaposKhighSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubgridPage message handlers

BOOL CSubgridPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSubgridPage::Reinitialize()
{
	((CButton *) GetDlgItem(IDC_ACTIVATE_SUBGRID))->SetCheck(FALSE);
	GetDlgItem(IDC_ILOW)->SetWindowText("");
	GetDlgItem(IDC_IHIGH)->SetWindowText("");
	GetDlgItem(IDC_JLOW)->SetWindowText("");
	GetDlgItem(IDC_JHIGH)->SetWindowText("");
	GetDlgItem(IDC_KLOW)->SetWindowText("");
	GetDlgItem(IDC_KHIGH)->SetWindowText("");
	Activate(FALSE);
}

void CSubgridPage::Activate(BOOL b)
{
	GetDlgItem(IDC_ACTIVATE_SUBGRID)->EnableWindow(b && m_pDoc->GetGridType() != MV_UNSTRUCTURED_GRID);
	BOOL bb = ((CButton *) GetDlgItem(IDC_ACTIVATE_SUBGRID))->GetCheck();
   BOOL bbb = b && bb && m_pDoc->GetGridType() == MV_STRUCTURED_GRID;
	GetDlgItem(IDC_ILOW)->EnableWindow(bbb);
	GetDlgItem(IDC_IHIGH)->EnableWindow(bbb);
	GetDlgItem(IDC_JLOW)->EnableWindow(bbb);
	GetDlgItem(IDC_JHIGH)->EnableWindow(bbb);
	GetDlgItem(IDC_ILOW_SPIN)->EnableWindow(bbb);
	GetDlgItem(IDC_IHIGH_SPIN)->EnableWindow(bbb);
	GetDlgItem(IDC_JLOW_SPIN)->EnableWindow(bbb);
	GetDlgItem(IDC_JHIGH_SPIN)->EnableWindow(bbb);
   bbb = b && bb && m_pDoc->GetGridType() != MV_UNSTRUCTURED_GRID;
	GetDlgItem(IDC_KLOW)->EnableWindow(bbb);
	GetDlgItem(IDC_KHIGH)->EnableWindow(bbb);
	GetDlgItem(IDC_KLOW_SPIN)->EnableWindow(bbb);
	GetDlgItem(IDC_KHIGH_SPIN)->EnableWindow(bbb);
	m_IsActive = b;
	if (m_Parent->m_PropertySheet->GetActiveIndex() == 2)
	{
		m_Parent->m_ApplyButton.EnableWindow(b && bb);
	}
}

BOOL CSubgridPage::CustomUpdateData(BOOL b)
{
	m_ExchangeData = TRUE;
	BOOL result = UpdateData(b);
	m_ExchangeData = FALSE;
	return result;
}

void CSubgridPage::Apply()
{
	if (CustomUpdateData(TRUE) && m_ActivateSubgrid)
	{
      if (m_pDoc->GetGridType() == MV_STRUCTURED_GRID)
      {
		   if (m_col_min < 1)
		   {
			   m_col_min = 1;
		   }
		   if (m_col_max > m_col_upper_limit)
		   {
			   m_col_max = m_col_upper_limit;
		   }
		   if (m_row_min < 1)
		   {
			   m_row_min = 1;
		   }
		   if (m_row_max > m_row_upper_limit)
		   {
			   m_row_max = m_row_upper_limit;
		   }
      }
      if (m_pDoc->GetGridType() == MV_STRUCTURED_GRID || m_pDoc->GetGridType() == MV_LAYERED_GRID)
      {
		   if (m_lay_min < 1)
		   {
			   m_lay_min = 1;
		   }
		   if (m_lay_max > m_lay_upper_limit)
		   {
			   m_lay_max = m_lay_upper_limit;
		   }
      }
		CustomUpdateData(FALSE);
		m_pDoc->ApplySubgrid(m_col_min, m_col_max, m_row_min, m_row_max, m_lay_min, m_lay_max);
	}
}

void CSubgridPage::OnActivateSubgrid() 
{
	// TODO: Add your control notification handler code here
	BOOL b = ((CButton *) GetDlgItem(IDC_ACTIVATE_SUBGRID))->GetCheck();
   BOOL bb = b && m_pDoc->GetGridType() == MV_STRUCTURED_GRID;
	GetDlgItem(IDC_ILOW)->EnableWindow(bb);
	GetDlgItem(IDC_IHIGH)->EnableWindow(bb);
	GetDlgItem(IDC_JLOW)->EnableWindow(bb);
	GetDlgItem(IDC_JHIGH)->EnableWindow(bb);
	GetDlgItem(IDC_ILOW_SPIN)->EnableWindow(bb);
	GetDlgItem(IDC_IHIGH_SPIN)->EnableWindow(bb);
	GetDlgItem(IDC_JLOW_SPIN)->EnableWindow(bb);
	GetDlgItem(IDC_JHIGH_SPIN)->EnableWindow(bb);
   if (m_pDoc->GetGridType() == MV_STRUCTURED_GRID || m_pDoc->GetGridType() == MV_LAYERED_GRID)
   {
	   GetDlgItem(IDC_KLOW)->EnableWindow(b);
	   GetDlgItem(IDC_KHIGH)->EnableWindow(b);
	   GetDlgItem(IDC_KLOW_SPIN)->EnableWindow(b);
	   GetDlgItem(IDC_KHIGH_SPIN)->EnableWindow(b);
   }
	m_Parent->m_ApplyButton.EnableWindow(b);

	if (b)
	{
		Apply();
	}
	else
	{
		m_pDoc->SubgridOff();
	}
}

void CSubgridPage::OnDeltaposIlowSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_col_min = __min(m_col_upper_limit, m_col_min + 1);
			if (m_col_min > m_col_max)
			{
				m_col_max = m_col_min;
			}
		}
		else
		{
			m_col_min = __max(1, m_col_min - 1);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CSubgridPage::OnDeltaposIhighSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_col_max = __min(m_col_upper_limit, m_col_max + 1);
		}
		else
		{
			m_col_max = __max(1, m_col_max - 1);
			if (m_col_max < m_col_min)
			{
				m_col_min = m_col_max;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CSubgridPage::OnDeltaposJlowSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_row_min = __min(m_row_upper_limit, m_row_min + 1);
			if (m_row_min > m_row_max)
			{
				m_row_max = m_row_min;
			}
		}
		else
		{
			m_row_min = __max(1, m_row_min - 1);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CSubgridPage::OnDeltaposJhighSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_row_max = __min(m_row_upper_limit, m_row_max + 1);
		}
		else
		{
			m_row_max = __max(1, m_row_max - 1);
			if (m_row_max < m_row_min)
			{
				m_row_min = m_row_max;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CSubgridPage::OnDeltaposKlowSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_lay_min = __min(m_lay_upper_limit, m_lay_min + 1);
			if (m_lay_min > m_lay_max)
			{
				m_lay_max = m_lay_min;
			}
		}
		else
		{
			m_lay_min = __max(1, m_lay_min - 1);
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

void CSubgridPage::OnDeltaposKhighSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if (CustomUpdateData(TRUE))
	{
		if (pNMUpDown->iDelta < 0)
		{
			m_lay_max = __min(m_lay_upper_limit, m_lay_max + 1);
		}
		else
		{
			m_lay_max = __max(1, m_lay_max - 1);
			if (m_lay_max < m_lay_min)
			{
				m_lay_min = m_lay_max;
			}
		}
		CustomUpdateData(FALSE);
		Apply();
	}
	*pResult = 0;
}

BOOL CSubgridPage::OnSetActive() 
{
	BOOL b = ((CButton *) GetDlgItem(IDC_ACTIVATE_SUBGRID))->GetCheck();
	m_Parent->m_ApplyButton.EnableWindow(m_IsActive && b);
	return CPropertyPage::OnSetActive();
}
