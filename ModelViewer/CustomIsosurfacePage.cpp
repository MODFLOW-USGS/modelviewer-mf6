// CustomIsosurfacePage.cpp : implementation file
//

#include "ModelViewer.h"
#include "CustomIsosurfacePage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomIsosurfacePage property page

IMPLEMENT_DYNCREATE(CCustomIsosurfacePage, CPropertyPage)

CCustomIsosurfacePage::CCustomIsosurfacePage()
    : CPropertyPage(CCustomIsosurfacePage::IDD)
{
    //{{AFX_DATA_INIT(CCustomIsosurfacePage)
    //}}AFX_DATA_INIT

    m_ExchangeData = FALSE;
}

CCustomIsosurfacePage::~CCustomIsosurfacePage()
{
}

void CCustomIsosurfacePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCustomIsosurfacePage)
    DDX_Control(pDX, IDC_ISOSURFACE_LIST, m_IsosurfaceList);
    //}}AFX_DATA_MAP
    if (m_ExchangeData)
    {
        DDX_Text(pDX, IDC_VALUE, m_Value);
    }
}

BEGIN_MESSAGE_MAP(CCustomIsosurfacePage, CPropertyPage)
    //{{AFX_MSG_MAP(CCustomIsosurfacePage)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_DELETE, OnDelete)
    ON_EN_SETFOCUS(IDC_VALUE, OnSetfocusValue)
    ON_LBN_SETFOCUS(IDC_ISOSURFACE_LIST, OnSetfocusIsosurfaceList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomIsosurfacePage message handlers

BOOL CCustomIsosurfacePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    Activate(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomIsosurfacePage::Reinitialize()
{
    m_IsosurfaceList.ResetContent();
    GetDlgItem(IDC_VALUE)->SetWindowText("");
    Activate(FALSE);
}

void CCustomIsosurfacePage::Activate(BOOL b)
{
    GetDlgItem(IDC_VALUE)->EnableWindow(b);
    GetDlgItem(IDC_ADD)->EnableWindow(b);
    GetDlgItem(IDC_DELETE)->EnableWindow(b);
    GetDlgItem(IDC_ISOSURFACE_LIST)->EnableWindow(b);
    m_IsActive = b;
}

BOOL CCustomIsosurfacePage::CustomUpdateData(BOOL b)
{
    m_ExchangeData = TRUE;
    BOOL result    = UpdateData(b);
    m_ExchangeData = FALSE;
    return result;
}

void CCustomIsosurfacePage::OnAdd()
{
    char text[16];
    if (CustomUpdateData(TRUE))
    {
        sprintf(text, "%g", m_Value);
        int count = m_IsosurfaceList.GetCount();
        if (count == 0)
        {
            m_IsosurfaceList.AddString(text);
        }
        else
        {
            int  i        = 0;
            BOOL exceeded = FALSE;
            char item[16];
            while (i < count && !exceeded)
            {
                m_IsosurfaceList.GetText(i, item);
                if (strcmp(item, text) == 0)
                {
                    AfxMessageBox("This value already exists.");
                    return;
                }
                if (atof(item) > m_Value)
                {
                    exceeded = TRUE;
                }
                else
                {
                    i++;
                }
            }
            m_IsosurfaceList.InsertString(i, text);
        }
        GetDlgItem(IDC_VALUE)->SetWindowText("");
    }
}

void CCustomIsosurfacePage::OnDelete()
{
    char text[16];
    m_IsosurfaceList.GetText(m_IsosurfaceList.GetCurSel(), text);
    GetDlgItem(IDC_VALUE)->SetWindowText(text);
    m_IsosurfaceList.DeleteString(m_IsosurfaceList.GetCurSel());
}

void CCustomIsosurfacePage::OnSetfocusValue()
{
    GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
}

void CCustomIsosurfacePage::OnSetfocusIsosurfaceList()
{
    // do this only when the page is activated, this can be testing
    // by checking if the value entry box is active
    if (GetDlgItem(IDC_VALUE)->IsWindowEnabled())
    {
        GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
    }
}

void CCustomIsosurfacePage::Apply()
{
    int count = m_IsosurfaceList.GetCount();
    if (count == 0)
    {
        AfxMessageBox("You must specify at least one isosurface value.");
        return;
    }
    double* values = new double[count];
    char    text[16];
    for (int i = 0; i < count; i++)
    {
        m_IsosurfaceList.GetText(i, text);
        values[i] = atof(text);
    }
    m_pDoc->SetCustomIsosurfaces(count, values);
    delete[] values;
}

BOOL CCustomIsosurfacePage::OnSetActive()
{
    if (m_IsActive)
    {
        Apply();
    }
    return CPropertyPage::OnSetActive();
}
