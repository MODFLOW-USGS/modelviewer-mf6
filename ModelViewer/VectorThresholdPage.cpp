// VectorThresholdPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "VectorThresholdPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorThresholdPage property page

IMPLEMENT_DYNCREATE(CVectorThresholdPage, CPropertyPage)

CVectorThresholdPage::CVectorThresholdPage()
    : CPropertyPage(CVectorThresholdPage::IDD)
{
    //{{AFX_DATA_INIT(CVectorThresholdPage)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_ExchangeData   = FALSE;
    m_ApplyThreshold = FALSE;
}

CVectorThresholdPage::~CVectorThresholdPage()
{
}

void CVectorThresholdPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVectorThresholdPage)
    DDX_Control(pDX, IDC_APPLY_VECTOR_THRESHOLD, m_VectorThresholdCheckBox);
    //}}AFX_DATA_MAP

    if (m_ExchangeData)
    {
        DDX_Check(pDX, IDC_APPLY_VECTOR_THRESHOLD, m_ApplyThreshold);
        DDX_Text(pDX, IDC_VECTOR_MIN, m_VectorMin);
        DDX_Text(pDX, IDC_VECTOR_MAX, m_VectorMax);
    }
}

BEGIN_MESSAGE_MAP(CVectorThresholdPage, CPropertyPage)
    //{{AFX_MSG_MAP(CVectorThresholdPage)
    ON_BN_CLICKED(IDC_APPLY_VECTOR_THRESHOLD, OnApplyVectorThreshold)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorThresholdPage message handlers

BOOL CVectorThresholdPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CVectorThresholdPage::Reinitialize()
{
    GetDlgItem(IDC_VECTOR_MIN)->SetWindowText("");
    GetDlgItem(IDC_VECTOR_MAX)->SetWindowText("");
    m_VectorThresholdCheckBox.SetCheck(FALSE);
    m_ApplyThreshold = FALSE;
    Activate(FALSE);
}

void CVectorThresholdPage::Activate(BOOL b)
{
    m_VectorThresholdCheckBox.EnableWindow(b);
    BOOL bb = m_VectorThresholdCheckBox.GetCheck();
    GetDlgItem(IDC_VECTOR_MIN)->EnableWindow(b && bb);
    GetDlgItem(IDC_VECTOR_MAX)->EnableWindow(b && bb);
}

void CVectorThresholdPage::OnDefault()
{
    double range[2];
    m_pDoc->GetVectorMagnitudeRange(range);
    m_VectorMin = range[0];
    m_VectorMax = range[1];
    CustomUpdateData(FALSE);
    Apply();
}

BOOL CVectorThresholdPage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CVectorThresholdPage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        if (m_ApplyThreshold)
        {
            m_pDoc->ApplyVectorThreshold(m_VectorMin, m_VectorMax);
        }
        else
        {
            m_pDoc->VectorThresholdOff();
        }
    }
}

void CVectorThresholdPage::OnApplyVectorThreshold()
{
    m_ApplyThreshold = m_VectorThresholdCheckBox.GetCheck();
    GetDlgItem(IDC_VECTOR_MIN)->EnableWindow(m_ApplyThreshold);
    GetDlgItem(IDC_VECTOR_MAX)->EnableWindow(m_ApplyThreshold);
    Apply();
}
