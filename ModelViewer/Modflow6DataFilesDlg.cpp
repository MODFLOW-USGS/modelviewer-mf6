// Modflow6DataFilesDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "Modflow6DataFilesDlg.h"
#include "afxdialogex.h"

// CModflow6DataFilesDlg dialog

IMPLEMENT_DYNAMIC(CModflow6DataFilesDlg, CDialog)

CModflow6DataFilesDlg::CModflow6DataFilesDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CModflow6DataFilesDlg::IDD, pParent)
{
    m_GwfModelNameFile        = _T("");
    m_HeadFile                = _T("");
    m_GridFile                = _T("");
    m_BudgetFile              = _T("");
    m_FileSpecificationMethod = 0;
}

CModflow6DataFilesDlg::~CModflow6DataFilesDlg()
{
}

void CModflow6DataFilesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_GWF_MODEL_NAME_FILE, m_GwfModelNameFile);
    DDX_Text(pDX, IDC_GWF_MODEL_HEAD_FILE, m_HeadFile);
    DDX_Text(pDX, IDC_GWF_MODEL_GRID_FILE, m_GridFile);
    DDX_Text(pDX, IDC_GWF_MODEL_BUDGET_FILE, m_BudgetFile);
    DDX_Radio(pDX, IDC_SPECIFY_MODFLOW6_NAME_FILE, m_FileSpecificationMethod);

    if (m_FileSpecificationMethod == 0)
    {
        m_GwfModelNameFile.TrimLeft();
        m_GwfModelNameFile.TrimRight();

        pDX->PrepareEditCtrl(IDC_NAME_FILE);
        if (pDX->m_bSaveAndValidate)
        {
            if (m_GwfModelNameFile.GetLength() == 0)
            {
                AfxMessageBox("Please specify a Groundwater Flow Model Name File.", MB_ICONEXCLAMATION);
                pDX->Fail();
                return;
            }
        }
        CString nameFile = m_GwfModelNameFile.Right(9);
        if (!nameFile.CompareNoCase("mfsim.nam"))
        {
            AfxMessageBox("\"" + nameFile + "\" is a Simulation Name File. Please specify a Groundwater Flow Model Name File.", MB_ICONEXCLAMATION);
            pDX->Fail();
            return;
        }
    }
    else
    {
    }
}

BEGIN_MESSAGE_MAP(CModflow6DataFilesDlg, CDialog)
    //   ON_BN_CLICKED(IDC_BROWSE_NAME, &CModflow6DataFilesDlg::OnBrowseName)
    ON_BN_CLICKED(IDC_BROWSE_GWF_MODEL_NAME_FILE, &CModflow6DataFilesDlg::OnBrowseGwfModelNameFile)
    ON_BN_CLICKED(IDC_SPECIFY_MODFLOW6_DATA_FILES, &CModflow6DataFilesDlg::OnSpecifyModflow6DataFiles)
    ON_BN_CLICKED(IDC_SPECIFY_MODFLOW6_NAME_FILE, &CModflow6DataFilesDlg::OnSpecifyModflow6NameFile)
    ON_BN_CLICKED(IDC_BROWSE_GWF_MODEL_HEAD_FILE, &CModflow6DataFilesDlg::OnBrowseGwfModelHeadFile)
    ON_BN_CLICKED(IDC_BROWSE_GWF_MODEL_GRID_FILE, &CModflow6DataFilesDlg::OnBrowseGwfModelGridFile)
    ON_BN_CLICKED(IDC_BROWSE_GWF_MODEL_BUDGET_FILE, &CModflow6DataFilesDlg::OnBrowseGwfModelBudgetFile)
END_MESSAGE_MAP()

// CModflow6DataFilesDlg message handlers

void CModflow6DataFilesDlg::OnBrowseGwfModelNameFile()
{
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        "Name Files (*.nam; *.name; *.fil)|*.nam; *.name; *.fil|All Files (*.*)|*.*||");
    fileDlg.m_ofn.lpstrTitle = "GWF Model Name File";
    if (fileDlg.DoModal() != IDOK)
    {
        return;
    }
    CString pathToNameFile = fileDlg.GetPathName();
    pathToNameFile.TrimLeft();
    pathToNameFile.TrimRight();
    CString nameFile = pathToNameFile.Right(9);
    if (!nameFile.CompareNoCase("mfsim.nam"))
    {
        AfxMessageBox("\"" + nameFile + "\" is a Simulation Name File. Please specify a Groundwater Flow Model Name File.", MB_ICONEXCLAMATION);
        return;
    }
    GetDlgItem(IDC_GWF_MODEL_NAME_FILE)->SetWindowText(fileDlg.GetPathName());
}

BOOL CModflow6DataFilesDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_FileSpecificationMethod = 0;
    OnSpecifyModflow6NameFile();

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CModflow6DataFilesDlg::OnSpecifyModflow6DataFiles()
{
    GetDlgItem(IDC_GWF_MODEL_NAME_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_GWF_MODEL_GRID_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_GWF_MODEL_HEAD_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_GWF_MODEL_BUDGET_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_NAME_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_GRID_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_HEAD_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_BUDGET_FILE)->EnableWindow(TRUE);
}

void CModflow6DataFilesDlg::OnSpecifyModflow6NameFile()
{
    GetDlgItem(IDC_GWF_MODEL_NAME_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_GWF_MODEL_GRID_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_GWF_MODEL_HEAD_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_GWF_MODEL_BUDGET_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_NAME_FILE)->EnableWindow(TRUE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_GRID_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_HEAD_FILE)->EnableWindow(FALSE);
    GetDlgItem(IDC_BROWSE_GWF_MODEL_BUDGET_FILE)->EnableWindow(FALSE);
}

void CModflow6DataFilesDlg::OnBrowseGwfModelGridFile()
{
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        "Grid Files (*.grb)|*.grb|All Files (*.*)|*.*||");
    fileDlg.m_ofn.lpstrTitle = "GWF Model Grid File";
    if (fileDlg.DoModal() != IDOK)
    {
        return;
    }
    GetDlgItem(IDC_GWF_MODEL_GRID_FILE)->SetWindowText(fileDlg.GetPathName());
}

void CModflow6DataFilesDlg::OnBrowseGwfModelHeadFile()
{
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        "Head Files (*.hds; *.bhd; *.hdb; *.hed)|*.hds; *.bhd; *.hdb; *.hed|All Files (*.*)|*.*||");
    fileDlg.m_ofn.lpstrTitle = "GWF Model Head File";
    if (fileDlg.DoModal() != IDOK)
    {
        return;
    }
    GetDlgItem(IDC_GWF_MODEL_HEAD_FILE)->SetWindowText(fileDlg.GetPathName());
}

void CModflow6DataFilesDlg::OnBrowseGwfModelBudgetFile()
{
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                        "Budget Files (*.cbc; *.cbb; *.ccf; *.bud; *.bin)|*.cbc; *.cbb; *.ccf; *.bud; *.bin|All Files (*.*)|*.*||");
    fileDlg.m_ofn.lpstrTitle = "GWF Model Budget File";
    if (fileDlg.DoModal() != IDOK)
    {
        return;
    }
    GetDlgItem(IDC_GWF_MODEL_BUDGET_FILE)->SetWindowText(fileDlg.GetPathName());
}
