// ColorBarColorsPage.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "ColorBarColorsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarColorsPage property page

IMPLEMENT_DYNCREATE(CColorBarColorsPage, CPropertyPage)

CColorBarColorsPage::CColorBarColorsPage() : CPropertyPage(CColorBarColorsPage::IDD)
{
	//{{AFX_DATA_INIT(CColorBarColorsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_FirstColor = RGB(90,90,90);
	m_LastColor = RGB(240,240,240);



}

CColorBarColorsPage::~CColorBarColorsPage()
{
}

void CColorBarColorsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBarColorsPage)
	DDX_Control(pDX, IDC_LAST_LIST, m_Last);
	DDX_Control(pDX, IDC_FIRST_LIST, m_First);
	DDX_Control(pDX, IDC_COMBO_COLOR_SCHEME, m_ColorSchemeCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBarColorsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColorBarColorsPage)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR_SCHEME, OnSelchangeComboColorScheme)
	ON_BN_CLICKED(IDC_LowerColorButton, OnLowerColorButton)
	ON_BN_CLICKED(IDC_UpperColorButton, OnUpperColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarColorsPage message handlers

void CColorBarColorsPage::Reinitialize()
{
	Activate(FALSE);
}

void CColorBarColorsPage::Activate(BOOL b)
{
	b = b && (m_pDoc->GetColorBarSource() == 0);
	CComboBox *cb = (CComboBox *)GetDlgItem(IDC_COMBO_COLOR_SCHEME);
	cb->EnableWindow(b);
	if (b)
	{
		int selection = cb->GetCurSel();
		// enable color selection buttons if the custom scheme is choosen.
		GetDlgItem(IDC_LowerColorButton)->EnableWindow(selection == 2);
		GetDlgItem(IDC_UpperColorButton)->EnableWindow(selection == 2);
	}
	else
	{
		GetDlgItem(IDC_LowerColorButton)->EnableWindow(FALSE);
		GetDlgItem(IDC_UpperColorButton)->EnableWindow(FALSE);
	}

}

void CColorBarColorsPage::OnDefault() 
{
	m_ColorSchemeCombo.SetCurSel(0);
	m_pDoc->SetColorBarColorScheme(0);
	m_FirstColor = RGB(90,90,90);
	m_LastColor = RGB(240,240,240);

	CColorBarPreviewListBox *List = (CColorBarPreviewListBox *)GetDlgItem(IDC_FIRST_LIST);
	List->m_Color = m_FirstColor;
	m_pDoc->SetColorBarFirstCustomColor(m_FirstColor);
	List->Invalidate();

	List = (CColorBarPreviewListBox *)GetDlgItem(IDC_LAST_LIST);
	List->m_Color = m_LastColor;
	m_pDoc->SetColorBarLastCustomColor(m_LastColor);
	List->Invalidate();

	Activate(TRUE);
}

void CColorBarColorsPage::Apply() 
{
	// TODO: Is there anything to do?
}

void CColorBarColorsPage::InitializeDialog() 
{
	CComboBox *cb = (CComboBox *) GetDlgItem(IDC_COMBO_COLOR_SCHEME);
	cb->SetCurSel(m_pDoc->GetColorBarColorScheme());
	
	CColorBarPreviewListBox *List = (CColorBarPreviewListBox *)GetDlgItem(IDC_FIRST_LIST);
	CString aFeature = " ";
	if (List->GetCount() == 0)
	{
		List->InsertString(0, (LPCTSTR) aFeature);
	}
	List->m_Color = m_pDoc->GetColorBarFirstCustomColor();
	List->Invalidate();

	List = (CColorBarPreviewListBox *)GetDlgItem(IDC_LAST_LIST);
	if (List->GetCount() == 0)
	{
		List->InsertString(0, (LPCTSTR) aFeature);
	}
	List->m_Color = m_pDoc->GetColorBarLastCustomColor();
	List->Invalidate();

}


BOOL CColorBarColorsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CComboBox *cb = (CComboBox *) GetDlgItem(IDC_COMBO_COLOR_SCHEME);
	
	cb->AddString("Default");
	cb->AddString("Modified");
	cb->AddString("Custom");

	InitializeDialog(); 

	Reinitialize();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBarColorsPage::OnSelchangeComboColorScheme() 
{
	m_pDoc->SetColorBarColorScheme(m_ColorSchemeCombo.GetCurSel());
	Activate(TRUE);
}

void CColorBarColorsPage::OnLowerColorButton() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlgColor(m_pDoc->GetColorBarFirstCustomColor());
	if (dlgColor.DoModal() == IDOK)
	{
		CColorBarPreviewListBox *List = (CColorBarPreviewListBox *)GetDlgItem(IDC_FIRST_LIST);
		List->m_Color = dlgColor.GetColor();
		m_pDoc->SetColorBarFirstCustomColor(dlgColor.GetColor());
		List->Invalidate();
	}
}

void CColorBarColorsPage::OnUpperColorButton() 
{
	// TODO: Add your control notification handler code here
	
	CColorDialog dlgColor(m_pDoc->GetColorBarLastCustomColor());
	if (dlgColor.DoModal() == IDOK)
	{
		CColorBarPreviewListBox *List = (CColorBarPreviewListBox *)GetDlgItem(IDC_LAST_LIST);
		List->m_Color = dlgColor.GetColor();
		m_pDoc->SetColorBarLastCustomColor(dlgColor.GetColor());
		List->Invalidate();
	}
}
