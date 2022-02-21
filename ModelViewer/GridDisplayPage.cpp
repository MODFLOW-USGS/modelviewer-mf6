// GridDisplayPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "GridDisplayPage.h"
#include "afxdialogex.h"
#include "MvDoc.h"
#include "GridDlg.h"


// CGridDisplayPage dialog

IMPLEMENT_DYNAMIC(CGridDisplayPage, CPropertyPage)

CGridDisplayPage::CGridDisplayPage()
	: CPropertyPage(CGridDisplayPage::IDD)
{
   m_GridDisplayMode = 0;
   m_UnstructuredGrid = FALSE;
}

CGridDisplayPage::~CGridDisplayPage()
{
}

void CGridDisplayPage::DoDataExchange(CDataExchange* pDX)
{
   CPropertyPage::DoDataExchange(pDX);
   DDX_Radio(pDX, IDC_GRID_DISPLAY_INTERPOLATED, m_GridDisplayMode);
}


BEGIN_MESSAGE_MAP(CGridDisplayPage, CPropertyPage)
   ON_BN_CLICKED(IDC_GRID_DISPLAY_INTERPOLATED, &CGridDisplayPage::OnGridDisplayInterpolated)
   ON_BN_CLICKED(IDC_GRID_DISPLAY_STAIRSTEPPED, &CGridDisplayPage::OnGridDisplayStairstepped)
END_MESSAGE_MAP()


void CGridDisplayPage::Activate(BOOL b)
{
   if (!m_UnstructuredGrid)
   {
	   GetDlgItem(IDC_GRID_DISPLAY_INTERPOLATED)->EnableWindow(b);
	   GetDlgItem(IDC_GRID_DISPLAY_STAIRSTEPPED)->EnableWindow(b);
   }
}

BOOL CGridDisplayPage::OnInitDialog()
{
   CPropertyPage::OnInitDialog();

	Reinitialize();

   return TRUE;  // return TRUE unless you set the focus to a control
   // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridDisplayPage::Reinitialize()
{
   m_GridDisplayMode = 0;
   UpdateData(FALSE);
   Activate(FALSE);
}

void CGridDisplayPage::OnGridDisplayInterpolated()
{
   m_pDoc->SetGridDisplayToInterpolated();
}

void CGridDisplayPage::OnGridDisplayStairstepped()
{
	m_pDoc->SetGridDisplayToStairstepped();
}
