// ScalarDataPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "ScalarDataPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScalarDataPage property page

IMPLEMENT_DYNCREATE(CScalarDataPage, CPropertyPage)

CScalarDataPage::CScalarDataPage()
    : CPropertyPage(CScalarDataPage::IDD)
{
    //{{AFX_DATA_INIT(CScalarDataPage)
    m_DataType = -1;
    //}}AFX_DATA_INIT
}

CScalarDataPage::~CScalarDataPage()
{
}

void CScalarDataPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CScalarDataPage)
    DDX_Control(pDX, IDC_DATA_TYPE_COMBO, m_DataTypeChooser);
    DDX_CBIndex(pDX, IDC_DATA_TYPE_COMBO, m_DataType);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScalarDataPage, CPropertyPage)
    //{{AFX_MSG_MAP(CScalarDataPage)
    ON_CBN_SELCHANGE(IDC_DATA_TYPE_COMBO, OnSelchangeDataTypeCombo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScalarDataPage message handlers

void CScalarDataPage::Reinitialize()
{
    GetDlgItem(IDC_MIN)->SetWindowText("");
    GetDlgItem(IDC_MAX)->SetWindowText("");
    m_DataTypeChooser.ResetContent();
}

void CScalarDataPage::Activate(BOOL b)
{
    m_DataTypeChooser.EnableWindow(b);
}

void CScalarDataPage::SetRange(double* range)
{
    char buff[20];
    sprintf(buff, "%g", range[0]);
    GetDlgItem(IDC_MIN)->SetWindowText(buff);
    sprintf(buff, "%g", range[1]);
    GetDlgItem(IDC_MAX)->SetWindowText(buff);
}

void CScalarDataPage::OnSelchangeDataTypeCombo()
{
    // TODO: Add your control notification handler code here
    m_pDoc->SetScalarDataTypeTo(m_DataTypeChooser.GetCurSel());
}
