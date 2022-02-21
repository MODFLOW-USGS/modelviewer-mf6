// ExportAnimationDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "ExportAnimationDlg.h"
#include "shlobj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportAnimationDlg dialog


CExportAnimationDlg::CExportAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportAnimationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportAnimationDlg)
	m_Elevate = 0.0;
	m_EndIndex = -1;
	m_FilePrefix = _T("");
	m_FileStartNumber = _T("");
	m_OutputFolder = _T("");
	m_Rotate = 0.0;
	m_StartIndex = -1;
	m_NumberOfSteps = 0;
	m_AnimationType = -1;
	//}}AFX_DATA_INIT
	m_Preview = FALSE;
}


void CExportAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportAnimationDlg)
	DDX_Text(pDX, IDC_ELEVATE, m_Elevate);
	DDX_CBIndex(pDX, IDC_END_INDEX, m_EndIndex);
	DDX_Text(pDX, IDC_FILE_PREFIX, m_FilePrefix);
	DDX_Text(pDX, IDC_FILE_START_NUMBER, m_FileStartNumber);
	DDX_Text(pDX, IDC_OUTPUT_FOLDER, m_OutputFolder);
	DDX_Text(pDX, IDC_ROTATE, m_Rotate);
	DDX_CBIndex(pDX, IDC_START_INDEX, m_StartIndex);
	DDX_Text(pDX, IDC_EDIT_EXPORT_NUMBER_OF_STEPS, m_NumberOfSteps);
	DDV_MinMaxInt(pDX, m_NumberOfSteps, 1, 1000000);
	DDX_CBIndex(pDX, IDC_COMBO_EXPORT_ANIMATION_TYPE, m_AnimationType);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		float aFloat;
		DDX_Text(pDX, IDC_FILE_START_NUMBER, aFloat);
	}
}


BEGIN_MESSAGE_MAP(CExportAnimationDlg, CDialog)
	//{{AFX_MSG_MAP(CExportAnimationDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_CBN_SELCHANGE(IDC_START_INDEX, OnSelchangeStartIndex)
	ON_CBN_SELCHANGE(IDC_END_INDEX, OnSelchangeEndIndex)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_COMBO_EXPORT_ANIMATION_TYPE, OnSelchangeComboExportAnimationType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportAnimationDlg message handlers

BOOL CExportAnimationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DoAnimationTypeChange();
	
	CComboBox *cb = (CComboBox *) GetDlgItem(IDC_START_INDEX);
	for (int i=0; i<m_NumberOfTimePoints; i++)
	{
		cb->AddString(m_TimePointLabels[i]);
	}
	cb->SetCurSel(m_StartIndex);
	cb = (CComboBox *) GetDlgItem(IDC_END_INDEX);
	for (int i=0; i<m_NumberOfTimePoints; i++)
	{
		cb->AddString(m_TimePointLabels[i]);
	}
	cb->SetCurSel(m_EndIndex);

	int numFiles = m_EndIndex - m_StartIndex + 1;
	char buff[20];
	sprintf(buff, "%d", numFiles);
	GetDlgItem(IDC_NUMBER_OF_FILES)->SetWindowText(buff);

	sprintf(buff, "%d KBytes", numFiles*m_SpaceUsagePerFile);
	GetDlgItem(IDC_DISK_SPACE_REQUIRED)->SetWindowText(buff);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExportAnimationDlg::OnBrowse() 
{
	// Use the shell to display a "Browse for Folder"
	// dialog box

	LPMALLOC lpMalloc;  // pointer to IMalloc

	if (::SHGetMalloc(&lpMalloc) != NOERROR)
	{
		return ; // failed to get allocator
	}

	char szBuffer[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = this->m_hWnd;
	browseInfo.pidlRoot = NULL;   // set root at Desktop
	browseInfo.pszDisplayName = szBuffer;
	browseInfo.lpszTitle = "Please select output folder";
	browseInfo.ulFlags = 0;   // not used
	browseInfo.lpfn = NULL;      // not used
	browseInfo.lParam = 0;      // not used   

	LPITEMIDLIST lpItemIDList;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose
			if (szBuffer[0] == '\0')
			{
			   // SHGetPathFromIDList failed, or
			   // SHBrowseForFolder failed.
			   AfxMessageBox("Unable to access folder", MB_ICONSTOP|MB_OK);
			   return;
			}
     
			// We have a path in szBuffer.
			GetDlgItem(IDC_OUTPUT_FOLDER)->SetWindowText(szBuffer);
		}
		else
		{
			// The thing referred to by lpItemIDList 
			// might not have been a file system object.
			// For whatever reason, SHGetPathFromIDList
			// didn't work.
			AfxMessageBox("Unable to access folder", MB_ICONSTOP|MB_OK);
			return;
		}
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release(); 
	}
}

void CExportAnimationDlg::OnPreview() 
{
	m_Preview = TRUE;
	OnOK();
}

void CExportAnimationDlg::OnSelchangeStartIndex() 
{
	int oldStartIndex = m_StartIndex;
	int oldEndIndex = m_EndIndex;
	UpdateData(TRUE);
	if (m_StartIndex > m_EndIndex)
	{
		AfxMessageBox("Please insure that the starting time step is less than or equal to the ending time step.", MB_ICONEXCLAMATION);
		m_StartIndex = oldStartIndex;
		m_EndIndex = oldEndIndex;
		UpdateData(FALSE);
		return;
	}

	char buff[20];
	int numFiles = m_EndIndex - m_StartIndex + 1;
	sprintf(buff, "%d", numFiles);
	GetDlgItem(IDC_NUMBER_OF_FILES)->SetWindowText(buff);

	sprintf(buff, "%d KBytes", numFiles*m_SpaceUsagePerFile);
	GetDlgItem(IDC_DISK_SPACE_REQUIRED)->SetWindowText(buff);
}

void CExportAnimationDlg::OnSelchangeEndIndex() 
{
	int oldStartIndex = m_StartIndex;
	int oldEndIndex = m_EndIndex;
	UpdateData(TRUE);
	if (m_StartIndex > m_EndIndex)
	{
		AfxMessageBox("Please insure that the starting time step is less than or equal to the ending time step.", MB_ICONEXCLAMATION);
		m_StartIndex = oldStartIndex;
		m_EndIndex = oldEndIndex;
		UpdateData(FALSE);
		return;
	}

	char buff[20];
	int numFiles = m_EndIndex - m_StartIndex + 1;
	sprintf(buff, "%d", numFiles);
	GetDlgItem(IDC_NUMBER_OF_FILES)->SetWindowText(buff);

	sprintf(buff, "%d KBytes", numFiles*m_SpaceUsagePerFile);
	GetDlgItem(IDC_DISK_SPACE_REQUIRED)->SetWindowText(buff);
}

BOOL CExportAnimationDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Inactivates help when F1 is pressed	
	//return CDialog::OnHelpInfo(pHelpInfo);
	return 0;
}

void CExportAnimationDlg::OnSelchangeComboExportAnimationType() 
{
	m_AnimationType = ((CComboBox *) GetDlgItem(IDC_COMBO_EXPORT_ANIMATION_TYPE))
						->GetCurSel();
	DoAnimationTypeChange();
	
}
	
void CExportAnimationDlg::DoAnimationTypeChange()
{
	switch (m_AnimationType)
	{
		case 0:
			GetDlgItem(IDC_EDIT_EXPORT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_INDEX)->EnableWindow(TRUE);
			GetDlgItem(IDC_END_INDEX)->EnableWindow(TRUE);
			break;
		case 1:
			GetDlgItem(IDC_EDIT_EXPORT_NUMBER_OF_STEPS)->EnableWindow(TRUE);
			GetDlgItem(IDC_START_INDEX)->EnableWindow(FALSE);
			GetDlgItem(IDC_END_INDEX)->EnableWindow(FALSE);
			break;
		default:
			GetDlgItem(IDC_EDIT_EXPORT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			GetDlgItem(IDC_START_INDEX)->EnableWindow(TRUE);
			GetDlgItem(IDC_END_INDEX)->EnableWindow(TRUE);
			break;
	}
}

