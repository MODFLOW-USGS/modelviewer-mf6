// VectorControlsPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "VectorControlsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorControlsPage property page

IMPLEMENT_DYNCREATE(CVectorControlsPage, CPropertyPage)

CVectorControlsPage::CVectorControlsPage()
    : CPropertyPage(CVectorControlsPage::IDD)
{
    //{{AFX_DATA_INIT(CVectorControlsPage)
    //}}AFX_DATA_INIT
    m_col_min      = 0;
    m_col_max      = 0;
    m_col_rate     = 0;
    m_row_min      = 0;
    m_row_max      = 0;
    m_row_rate     = 0;
    m_lay_min      = 0;
    m_lay_max      = 0;
    m_lay_rate     = 0;
    m_ExchangeData = FALSE;
}

CVectorControlsPage::~CVectorControlsPage()
{
}

void CVectorControlsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVectorControlsPage)
    //}}AFX_DATA_MAP
    if (m_ExchangeData)
    {
        DDX_Text(pDX, IDC_IMIN, m_col_min);
        DDX_Text(pDX, IDC_IMAX, m_col_max);
        DDX_Text(pDX, IDC_IRATE, m_col_rate);
        DDX_Text(pDX, IDC_JMIN, m_row_min);
        DDX_Text(pDX, IDC_JMAX, m_row_max);
        DDX_Text(pDX, IDC_JRATE, m_row_rate);
        DDX_Text(pDX, IDC_KMIN, m_lay_min);
        DDX_Text(pDX, IDC_KMAX, m_lay_max);
        DDX_Text(pDX, IDC_KRATE, m_lay_rate);

        pDX->PrepareEditCtrl(IDC_IRATE);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_col_rate <= 0)
            {
                AfxMessageBox("Please enter a positive integer.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
        pDX->PrepareEditCtrl(IDC_JRATE);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_row_rate <= 0)
            {
                AfxMessageBox("Please enter a positive integer.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
        pDX->PrepareEditCtrl(IDC_KRATE);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_lay_rate <= 0)
            {
                AfxMessageBox("Please enter a positive integer.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
        pDX->PrepareEditCtrl(IDC_IMAX);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_StructuredGrid && (m_col_max < m_col_min))
            {
                AfxMessageBox("Please insure that \"Max\" index is greater than or equal to \"Min\" index.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
        pDX->PrepareEditCtrl(IDC_JMAX);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_StructuredGrid && (m_row_max < m_row_min))
            {
                AfxMessageBox("Please insure that \"Max\" index is greater than or equal to \"Min\" index.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
        pDX->PrepareEditCtrl(IDC_KMAX);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_StructuredGrid && (m_lay_max < m_lay_min))
            {
                AfxMessageBox("Please insure that \"Max\" index is greater than or equal to \"Min\" index.", MB_ICONEXCLAMATION);
                pDX->Fail();
            }
        }
    }
}

BEGIN_MESSAGE_MAP(CVectorControlsPage, CPropertyPage)
    //{{AFX_MSG_MAP(CVectorControlsPage)
    ON_NOTIFY(UDN_DELTAPOS, IDC_IMIN_SPIN, OnDeltaposIminSpin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_IMAX_SPIN, OnDeltaposImaxSpin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_JMIN_SPIN, OnDeltaposJminSpin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_JMAX_SPIN, OnDeltaposJmaxSpin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_KMIN_SPIN, OnDeltaposKminSpin)
    ON_NOTIFY(UDN_DELTAPOS, IDC_KMAX_SPIN, OnDeltaposKmaxSpin)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorControlsPage message handlers

BOOL CVectorControlsPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    Reinitialize();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CVectorControlsPage::Reinitialize()
{
    GetDlgItem(IDC_IMIN)->SetWindowText("");
    GetDlgItem(IDC_IMAX)->SetWindowText("");
    GetDlgItem(IDC_IRATE)->SetWindowText("");
    GetDlgItem(IDC_JMIN)->SetWindowText("");
    GetDlgItem(IDC_JMAX)->SetWindowText("");
    GetDlgItem(IDC_JRATE)->SetWindowText("");
    GetDlgItem(IDC_KMIN)->SetWindowText("");
    GetDlgItem(IDC_KMAX)->SetWindowText("");
    GetDlgItem(IDC_KRATE)->SetWindowText("");
    Activate(FALSE);
}

void CVectorControlsPage::Activate(BOOL b)
{
    GetDlgItem(IDC_IMIN)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_IMAX)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_IRATE)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_JMIN)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_JMAX)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_JRATE)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_KMIN)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_KMAX)->EnableWindow(b && m_StructuredGrid);
    GetDlgItem(IDC_KRATE)->EnableWindow(b && m_StructuredGrid);
}

void CVectorControlsPage::OnDeltaposIminSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    if (CustomUpdateData(TRUE))
    {
        if (pNMUpDown->iDelta < 0)
        {
            m_col_min = __min(m_col_max, m_col_min + 1);
        }
        else
        {
            m_col_min = __max(m_col_lower_limit, m_col_min - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDeltaposImaxSpin(NMHDR* pNMHDR, LRESULT* pResult)
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
            m_col_max = __max(m_col_min, m_col_max - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDeltaposJminSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    if (CustomUpdateData(TRUE))
    {
        if (pNMUpDown->iDelta < 0)
        {
            m_row_min = __min(m_row_max, m_row_min + 1);
        }
        else
        {
            m_row_min = __max(m_row_lower_limit, m_row_min - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDeltaposJmaxSpin(NMHDR* pNMHDR, LRESULT* pResult)
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
            m_row_max = __max(m_row_min, m_row_max - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDeltaposKminSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    if (CustomUpdateData(TRUE))
    {
        if (pNMUpDown->iDelta < 0)
        {
            m_lay_min = __min(m_lay_max, m_lay_min + 1);
        }
        else
        {
            m_lay_min = __max(m_lay_lower_limit, m_lay_min - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDeltaposKmaxSpin(NMHDR* pNMHDR, LRESULT* pResult)
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
            m_lay_max = __max(m_lay_min, m_lay_max - 1);
        }
        CustomUpdateData(FALSE);
        Apply();
    }
    *pResult = 0;
}

void CVectorControlsPage::OnDefault()
{
    m_col_min  = m_col_lower_limit;
    m_col_max  = m_col_upper_limit;
    m_row_min  = m_row_lower_limit;
    m_row_max  = m_row_upper_limit;
    m_lay_min  = m_lay_lower_limit;
    m_lay_max  = m_lay_upper_limit;
    m_col_rate = 1;
    m_row_rate = 1;
    m_lay_rate = 1;
    CustomUpdateData(FALSE);
    Apply();
}

BOOL CVectorControlsPage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CVectorControlsPage::Apply()
{
    if (CustomUpdateData(TRUE))
    {
        if (m_col_min > m_col_upper_limit)
        {
            m_col_min = m_col_upper_limit;
            m_col_max = m_col_upper_limit;
        }
        else if (m_col_max < m_col_lower_limit)
        {
            m_col_min = m_col_lower_limit;
            m_col_max = m_col_lower_limit;
        }
        else
        {
            m_col_min = max(m_col_min, m_col_lower_limit);
            m_col_max = min(m_col_max, m_col_upper_limit);
        }
        if (m_row_min > m_row_upper_limit)
        {
            m_row_min = m_row_upper_limit;
            m_row_max = m_row_upper_limit;
        }
        else if (m_row_max < m_row_lower_limit)
        {
            m_row_min = m_row_lower_limit;
            m_row_max = m_row_lower_limit;
        }
        else
        {
            m_row_min = max(m_row_min, m_row_lower_limit);
            m_row_max = min(m_row_max, m_row_upper_limit);
        }
        if (m_lay_min > m_lay_upper_limit)
        {
            m_lay_min = m_lay_upper_limit;
            m_lay_max = m_lay_upper_limit;
        }
        else if (m_lay_max < m_lay_lower_limit)
        {
            m_lay_min = m_lay_lower_limit;
            m_lay_max = m_lay_lower_limit;
        }
        else
        {
            m_lay_min = max(m_lay_min, m_lay_lower_limit);
            m_lay_max = min(m_lay_max, m_lay_upper_limit);
        }
        CustomUpdateData(FALSE);

        m_pDoc->SubsampleVectors(m_col_min, m_col_max, m_col_rate, m_row_min, m_row_max, m_row_rate, m_lay_min, m_lay_max, m_lay_rate);
    }
}
