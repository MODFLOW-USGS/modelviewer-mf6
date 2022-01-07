#pragma once


// CGridDisplayPage dialog
class CMvDoc;
class CGridDlg;

class CGridDisplayPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGridDisplayPage)

public:
	CGridDisplayPage();
	virtual ~CGridDisplayPage();
	void Reinitialize();
	void Activate(BOOL b);
	CMvDoc *m_pDoc;
	CGridDlg *m_Parent;
	BOOL m_UnstructuredGrid;

// Dialog Data
	enum { IDD = IDD_GRID_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   virtual BOOL OnInitDialog();
   afx_msg void OnGridDisplayInterpolated();
   afx_msg void OnGridDisplayStairstepped();
   int m_GridDisplayMode;
};
