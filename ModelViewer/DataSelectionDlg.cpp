// DataSelectionDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "DataSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSelectionDlg dialog

CDataSelectionDlg::CDataSelectionDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDataSelectionDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDataSelectionDlg)
    m_SelectedDataType  = -1;
    m_SelectedTimePoint = -1;
    //}}AFX_DATA_INIT
}

void CDataSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDataSelectionDlg)
    DDX_CBIndex(pDX, IDC_DATA_TYPE_CHOOSER, m_SelectedDataType);
    DDX_CBIndex(pDX, IDC_TIME_CHOOSER, m_SelectedTimePoint);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDataSelectionDlg, CDialog)
    //{{AFX_MSG_MAP(CDataSelectionDlg)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSelectionDlg message handlers

BOOL CDataSelectionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CComboBox* cb = (CComboBox*)GetDlgItem(IDC_TIME_CHOOSER);
    for (int i = 0; i < m_NumberOfTimePoints; i++)
    {
        cb->AddString(m_TimePointLabels[i]);
    }
    if (m_InitialDisplayTimePoint > 0 && m_InitialDisplayTimePoint < m_NumberOfTimePoints)
    {
        cb->SetCurSel(m_InitialDisplayTimePoint);
    }
    else
    {
        cb->SetCurSel(0);
    }

    cb = (CComboBox*)GetDlgItem(IDC_DATA_TYPE_CHOOSER);
    for (int i = 0; i < m_NumberOfScalarDataTypes; i++)
    {
        cb->AddString(m_DataTypeLabels[i]);
    }
    cb->SetCurSel(0);

    if (m_TimeLabelOption == 0)
    {
        GetDlgItem(IDC_TO_VISUALIZE)->SetWindowText("Time to visualize:");
    }
    else
    {
        GetDlgItem(IDC_TO_VISUALIZE)->SetWindowText("Time step:");
    }

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataSelectionDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // Inactivates help when F1 is pressed
    // return CDialog::OnHelpInfo(pHelpInfo);
    return 0;
}
