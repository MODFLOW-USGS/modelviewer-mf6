// ImageGenerationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "ImageGenerationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageGenerationDlg dialog


CImageGenerationDlg::CImageGenerationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageGenerationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageGenerationDlg)
	m_ImageQuality = -1;
	//}}AFX_DATA_INIT
}


void CImageGenerationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageGenerationDlg)
	DDX_Radio(pDX, IDC_IMAGE_COARSE, m_ImageQuality);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageGenerationDlg, CDialog)
	//{{AFX_MSG_MAP(CImageGenerationDlg)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageGenerationDlg message handlers

BOOL CImageGenerationDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Inactivates help when F1 is pressed		
	//return CDialog::OnHelpInfo(pHelpInfo);
	return 0;
}
