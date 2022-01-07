// ModelFeaturesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "ModelFeaturesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelFeaturesDlg dialog


CModelFeaturesDlg::CModelFeaturesDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CModelFeaturesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelFeaturesDlg)
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CModelFeaturesDlg::IDD;
	m_NumberOfModelFeatureTypes = 0;
	m_DisplayOrder = 0;
}


void CModelFeaturesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelFeaturesDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_HIDE_LIST, m_HideListBox);
	DDX_Control(pDX, IDC_SHOW_LIST, m_ShowListBox);
}


BEGIN_MESSAGE_MAP(CModelFeaturesDlg, CDialog)
	//{{AFX_MSG_MAP(CModelFeaturesDlg)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_LBN_SELCHANGE(IDC_SHOW_LIST, OnSelchangeShowList)
	ON_BN_CLICKED(IDC_SHOW_FEATURE, OnShowFeature)
	ON_BN_CLICKED(IDC_HIDE_FEATURE, OnHideFeature)
	ON_BN_CLICKED(IDC_MOVE_TO_TOP, OnMoveToTop)
	ON_BN_CLICKED(IDC_MOVE_TO_BOT, OnMoveToBot)
	ON_BN_CLICKED(IDC_MOVE_UP, OnMoveUp)
	ON_BN_CLICKED(IDC_MOVE_DOWN, OnMoveDown)
	ON_LBN_SELCHANGE(IDC_HIDE_LIST, OnSelchangeHideList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MODEL_FEATURE_SIZE, OnDeltaposModelFeatureSize)
	ON_BN_CLICKED(IDC_MODEL_FEATURE_COLOR, OnModelFeatureColor)
	ON_LBN_DBLCLK(IDC_HIDE_LIST, OnDblClkHideList)
	ON_LBN_DBLCLK(IDC_SHOW_LIST, OnDblClkShowList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelFeaturesDlg message handlers

BOOL CModelFeaturesDlg::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}


void CModelFeaturesDlg::PostNcDestroy() 
{
	if (m_DisplayOrder != 0)
	{
		delete [] m_DisplayOrder;
	}
	delete this;
}

BOOL CModelFeaturesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.left = 10;
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgWidth;
	wndpl.rcNormalPosition.top = (3*screenHeight)/6;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);
	m_ShowListBox.m_pDoc = this->m_pDoc;
	m_HideListBox.m_pDoc = this->m_pDoc;
	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModelFeaturesDlg::SetNumberOfModelFeatureTypes(int n)
{
	m_NumberOfModelFeatureTypes = n;
}

void CModelFeaturesDlg::SetDisplayOrder(int *displayOrder)
{
	if (m_DisplayOrder != 0)
	{
		delete [] m_DisplayOrder;
	}
	m_DisplayOrder = new int[m_NumberOfModelFeatureTypes];
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		m_DisplayOrder[i] = displayOrder[i];
	}
}

void CModelFeaturesDlg::Reinitialize()
{
	Activate(FALSE);
}

void CModelFeaturesDlg::Activate(BOOL b)
{
	GetDlgItem(IDC_SHOW_LIST)->EnableWindow(b);
	GetDlgItem(IDC_HIDE_LIST)->EnableWindow(b);
	GetDlgItem(IDC_MODEL_FEATURE_SIZE)->EnableWindow(b);
	int showListCount = m_ShowListBox.GetCount();
	int showSel = m_ShowListBox.GetCurSel();
	int hideSel = m_HideListBox.GetCurSel();
	BOOL b1 = showListCount > 1 && showSel > 0;
	BOOL b2 = showListCount > 1 && showSel < showListCount-1 && showSel > -1;
	GetDlgItem(IDC_SHOW_FEATURE)->EnableWindow(b && hideSel > -1);
	GetDlgItem(IDC_HIDE_FEATURE)->EnableWindow(b && showSel > -1);
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(b && b1);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(b && b1);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(b && b2);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(b && b2);
	GetDlgItem(IDC_MODEL_FEATURE_COLOR)->EnableWindow(b && (showSel > -1 || hideSel > -1));
}

void CModelFeaturesDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}

void CModelFeaturesDlg::OnSelchangeHideList() 
{
	GetDlgItem(IDC_SHOW_FEATURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_HIDE_FEATURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODEL_FEATURE_COLOR)->EnableWindow(TRUE);
	m_ShowListBox.SetCurSel(-1);
}

void CModelFeaturesDlg::OnSelchangeShowList() 
{
	GetDlgItem(IDC_SHOW_FEATURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_HIDE_FEATURE)->EnableWindow(TRUE);
	m_HideListBox.SetCurSel(-1);
	int count = m_ShowListBox.GetCount();
	int i = m_ShowListBox.GetCurSel();
	BOOLEAN b1 = (count > 1 && i > 0);
	BOOLEAN b2 = (count > 1 && i < count-1);
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(b1);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(b1);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(b2);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(b2);
	GetDlgItem(IDC_MODEL_FEATURE_COLOR)->EnableWindow(TRUE);
}

void CModelFeaturesDlg::OnShowFeature() 
{
	int k = m_HideListBox.GetCurSel();
	CString aFeature;
	m_HideListBox.GetText(k, aFeature);
	m_HideListBox.DeleteString(k);
	m_ShowListBox.InsertString(0, (LPCTSTR) aFeature);
	m_ShowListBox.SetCurSel(0);
	int n = 0;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		if (m_DisplayOrder[i] >= 0)
		{
			m_DisplayOrder[i]++;
		}
		else if (m_DisplayOrder[i] == -1)
		{
			if (n == k)
			{
				m_DisplayOrder[i] = 0;
			}
			n++;
		}
	}
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(FALSE);
	int count = m_ShowListBox.GetCount();
	BOOLEAN b2 = (count > 1);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(b2);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(b2);
	GetDlgItem(IDC_SHOW_FEATURE)->EnableWindow(FALSE);
	GetDlgItem(IDC_HIDE_FEATURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_MODEL_FEATURE_COLOR)->EnableWindow(TRUE);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);



}

void CModelFeaturesDlg::OnHideFeature() 
{
	int k = m_ShowListBox.GetCurSel();
	CString aFeature;
	m_ShowListBox.GetText(k, aFeature);
	m_ShowListBox.DeleteString(k);
	int n = 0;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++) 
	{
		if (m_DisplayOrder[i] == -1)
		{
			n++;
		}
		else if (m_DisplayOrder[i] == k)
		{
			m_DisplayOrder[i] = -1;
			m_HideListBox.InsertString(n, (LPCTSTR) aFeature);
			m_HideListBox.SetCurSel(n);
		}
		else if (m_DisplayOrder[i] > k)
		{
			m_DisplayOrder[i]--;
		}
	}

	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOW_FEATURE)->EnableWindow(TRUE);
	GetDlgItem(IDC_HIDE_FEATURE)->EnableWindow(FALSE);	
	GetDlgItem(IDC_MODEL_FEATURE_COLOR)->EnableWindow(TRUE);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);
}

void CModelFeaturesDlg::OnMoveToTop() 
{
	int k = m_ShowListBox.GetCurSel();
	CString aFeature;
	m_ShowListBox.GetText(k, aFeature);
	m_ShowListBox.DeleteString(k);
	m_ShowListBox.InsertString(0, (LPCTSTR) aFeature);
	m_ShowListBox.SetCurSel(0);
	int n;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		if (m_DisplayOrder[i] == k)
		{
			n = i;
		}
		else if (m_DisplayOrder[i] >= 0 && m_DisplayOrder[i] < k)
		{
			m_DisplayOrder[i]++;
		}
	}
	m_DisplayOrder[n] = 0;
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(TRUE);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);
}

void CModelFeaturesDlg::OnMoveToBot() 
{
	int lastPos = m_ShowListBox.GetCount() - 1;
	int k = m_ShowListBox.GetCurSel();
	CString aFeature;
	m_ShowListBox.GetText(k, aFeature);
	m_ShowListBox.DeleteString(k);
	m_ShowListBox.InsertString(lastPos, (LPCTSTR) aFeature);
	m_ShowListBox.SetCurSel(lastPos);
	int n;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		if (m_DisplayOrder[i] == k)
		{
			n = i;
		}
		else if (m_DisplayOrder[i] > k)
		{
			m_DisplayOrder[i]--;
		}
	}
	m_DisplayOrder[n] = lastPos;
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(FALSE);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);
}

void CModelFeaturesDlg::OnMoveUp() 
{
	int k = m_ShowListBox.GetCurSel();
	CString aFeature;
	m_ShowListBox.GetText(k, aFeature);
	m_ShowListBox.DeleteString(k);
	int j = k-1;
	m_ShowListBox.InsertString(j, (LPCTSTR) aFeature);
	m_ShowListBox.SetCurSel(j);
	int m, n;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		if (m_DisplayOrder[i] == k)
		{
			n = i;
		}
		else if (m_DisplayOrder[i] == k-1)
		{
			m = i;
		}
	}
	m_DisplayOrder[n] = k - 1;
	m_DisplayOrder[m] = k;
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(j != 0);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(j != 0);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(TRUE);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);
}

void CModelFeaturesDlg::OnMoveDown() 
{
	int lastPos = m_ShowListBox.GetCount() - 1;
	int k = m_ShowListBox.GetCurSel();
	CString aFeature;
	m_ShowListBox.GetText(k, aFeature);
	m_ShowListBox.DeleteString(k);
	int j = k+1;
	m_ShowListBox.InsertString(j, (LPCTSTR) aFeature);
	m_ShowListBox.SetCurSel(j);
	int m, n;
	for (int i=0; i<m_NumberOfModelFeatureTypes; i++)
	{
		if (m_DisplayOrder[i] == k)
		{
			n = i;
		}
		else if (m_DisplayOrder[i] == k+1)
		{
			m = i;
		}
	}
	m_DisplayOrder[n] = k + 1;
	m_DisplayOrder[m] = k;
	GetDlgItem(IDC_MOVE_TO_TOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_UP)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOVE_DOWN)->EnableWindow(j != lastPos);
	GetDlgItem(IDC_MOVE_TO_BOT)->EnableWindow(j != lastPos);
	m_pDoc->SetModelFeatureDisplayOrder(m_DisplayOrder);
}

void CModelFeaturesDlg::OnDeltaposModelFeatureSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta < 0)
	{
		m_pDoc->EnlargeModelFeatureGlyphs();
	}
	else
	{
		m_pDoc->ShrinkModelFeatureGlyphs();
	}
	
	*pResult = 0;
}

void CModelFeaturesDlg::ShowGlyphSizeControl(BOOL b)
{
	if (b)
	{
		GetDlgItem(IDC_MODEL_FEATURE_SIZE_TEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MODEL_FEATURE_SIZE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_MODEL_FEATURE_SIZE_TEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MODEL_FEATURE_SIZE)->ShowWindow(SW_HIDE);
	}
}

void CModelFeaturesDlg::OnModelFeatureColor() 
{

	CString label;
	CListBox *listBox;
	int index = m_ShowListBox.GetCurSel();
	if (index > -1)
	{
		listBox = &m_ShowListBox;
	}
	else
	{
		index = m_HideListBox.GetCurSel();
		if (index > -1)
		{
			listBox = &m_HideListBox;
		}
		else
		{
			return;
		}
	}
	listBox->GetText(index, label);
	double rgba[4];
	m_pDoc->GetModelFeatureColor(label.GetBuffer(40), rgba);
	int r = (int) rgba[0]*255;
	int g = (int) rgba[1]*255;
	int b = (int) rgba[2]*255;
	COLORREF clrInit = RGB(r, g, b);

	CColorDialog dlg(clrInit);
	if (dlg.DoModal() == IDOK)
	{
		long icolor = (long) dlg.GetColor();
		rgba[0] = (icolor%256)/255.0;
		rgba[1] = ((icolor%65536L)/256)/255.0;
		rgba[2] = (icolor/65536L)/255.0;
		rgba[3] = 1;
		m_pDoc->SetModelFeatureColor(label.GetBuffer(40), rgba);
		listBox->Invalidate();
	}
}

void CModelFeaturesDlg::OnDblClkHideList() 
{
	if (m_HideListBox.GetCurSel() != LB_ERR)
	{
		OnShowFeature();
	}
}

void CModelFeaturesDlg::OnDblClkShowList() 
{
	if (m_ShowListBox.GetCurSel() != LB_ERR)
	{
		OnHideFeature();
	}
}
