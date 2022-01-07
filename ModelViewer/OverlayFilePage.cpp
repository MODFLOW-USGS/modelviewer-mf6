// OverlayFilePage.cpp : implementation file
//

#include "stdafx.h"
#include "modelviewer.h"
#include "OverlayFilePage.h"
#include "mvUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayFilePage property page

IMPLEMENT_DYNCREATE(COverlayFilePage, CPropertyPage)

COverlayFilePage::COverlayFilePage() : CPropertyPage(COverlayFilePage::IDD)
{
	//{{AFX_DATA_INIT(COverlayFilePage)
	m_Filename = _T("");
	//}}AFX_DATA_INIT
}

COverlayFilePage::~COverlayFilePage()
{
}

void COverlayFilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COverlayFilePage)
	DDX_Control(pDX, IDC_OVERLAY_TYPE, m_TypeComboBox);
	DDX_Text(pDX, IDC_OVERLAY_FILE, m_Filename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COverlayFilePage, CPropertyPage)
	//{{AFX_MSG_MAP(COverlayFilePage)
	ON_BN_CLICKED(IDC_BROWSE_OVERLAY, OnBrowseOverlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayFilePage message handlers
void COverlayFilePage::Reinitialize()
{
	m_Filename = _T("");
	UpdateData(FALSE);
	Activate(FALSE);
}

void COverlayFilePage::Activate(BOOL b)
{
	GetDlgItem(IDC_BROWSE_OVERLAY)->EnableWindow(b);
	GetDlgItem(IDC_OVERLAY_FILE)->EnableWindow(b);
	GetDlgItem(IDC_OVERLAY_TYPE)->EnableWindow(b);
}

BOOL COverlayFilePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_TypeComboBox.ResetContent();
	m_TypeComboBox.AddString("ESRI Shapefile");
	m_TypeComboBox.AddString("DXF File");
	m_TypeComboBox.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COverlayFilePage::OnBrowseOverlay() 
{
	char filename[MAX_PATH];
	GetDlgItem(IDC_OVERLAY_FILE)->GetWindowText(filename, MAX_PATH);
	char *initialFile = 0;
	if (strlen(filename) > 0)
	{
		mvUtil::TrimLeft(filename);
		mvUtil::TrimRight(filename);
		initialFile = filename;
	}
	
	CFileDialog fileDlg(TRUE, NULL, initialFile, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		"All Files (*.*)|*.*||");
		"All Files (*.*)|*.*|ESRI Shapefile Files (*.shp)|*.shp|DXF Files (*.dxf)|*.dxf||");
	fileDlg.m_ofn.lpstrTitle = "Overlay File";
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	CString file = fileDlg.GetPathName();
	GetDlgItem(IDC_OVERLAY_FILE)->SetWindowText(file);
	if (file.Right(4).CompareNoCase(".shp")==0)
	{
		m_TypeComboBox.SetCurSel(0);
	}
	else if (file.Right(4).CompareNoCase(".dxf")==0)
	{
		m_TypeComboBox.SetCurSel(1);
	}
}
