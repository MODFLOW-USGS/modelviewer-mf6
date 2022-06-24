#pragma once

// CModflow6DataFilesDlg dialog

class CModflow6DataFilesDlg : public CDialog
{
    DECLARE_DYNAMIC(CModflow6DataFilesDlg)

public:
    CModflow6DataFilesDlg(CWnd* pParent = NULL); // standard constructor
    virtual ~CModflow6DataFilesDlg();

    // Dialog Data
    enum { IDD = IDD_MODFLOW6_DATA_FILES };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBrowseGwfModelNameFile();
    CString      m_GwfModelNameFile;
    CString      m_HeadOrConcFile;
    CString      m_GridFile;
    CString      m_BudgetFile;
    virtual BOOL OnInitDialog();
    afx_msg void OnSpecifyModflow6DataFiles();
    afx_msg void OnSpecifyModflow6NameFile();
    int          m_FileSpecificationMethod;
    afx_msg void OnBrowseGwfModelHeadFile();
    afx_msg void OnBrowseGwfModelGridFile();
    afx_msg void OnBrowseGwfModelBudgetFile();
};
