// BoundingBoxPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "BoundingBoxPage.h"
#include "MvDoc.h"
#include "GeometryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoundingBoxPage property page

IMPLEMENT_DYNCREATE(CBoundingBoxPage, CPropertyPage)

CBoundingBoxPage::CBoundingBoxPage() : CPropertyPage(CBoundingBoxPage::IDD)
{
	//{{AFX_DATA_INIT(CBoundingBoxPage)
	m_ColorOption = -1;
	//}}AFX_DATA_INIT
}

CBoundingBoxPage::~CBoundingBoxPage()
{
}

void CBoundingBoxPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoundingBoxPage)
	DDX_Radio(pDX, IDC_BLACK, m_ColorOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoundingBoxPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBoundingBoxPage)
	ON_BN_CLICKED(IDC_BLACK, OnBlack)
	ON_BN_CLICKED(IDC_GRAY, OnGray)
	ON_BN_CLICKED(IDC_WHITE, OnWhite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoundingBoxPage message handlers

BOOL CBoundingBoxPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reinitialize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBoundingBoxPage::Reinitialize()
{
	m_ColorOption = 0;
	UpdateData(FALSE);
	Activate(FALSE);
}

void CBoundingBoxPage::Activate(BOOL b)
{
	GetDlgItem(IDC_GRAY)->EnableWindow(b);
	GetDlgItem(IDC_WHITE)->EnableWindow(b);
	GetDlgItem(IDC_BLACK)->EnableWindow(b);
}

void CBoundingBoxPage::OnBlack() 
{
	m_pDoc->SetBoundingBoxColor(0, 0, 0);
}

void CBoundingBoxPage::OnGray() 
{
	m_pDoc->SetBoundingBoxColor(0.5, 0.5, 0.5);
}

void CBoundingBoxPage::OnWhite() 
{
	m_pDoc->SetBoundingBoxColor(1, 1, 1);
}

BOOL CBoundingBoxPage::OnSetActive() 
{
	m_Parent->m_ApplyButton.EnableWindow(FALSE);
	return CPropertyPage::OnSetActive();
}
