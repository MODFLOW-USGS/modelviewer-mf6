// ModelViewer.cpp : Defines the class behaviors for the application.
//

#include "ModelViewer.h"

#include "MainFrm.h"
#include "MvDoc.h"
#include "MvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelViewerApp

BEGIN_MESSAGE_MAP(CModelViewerApp, CWinApp)
    //{{AFX_MSG_MAP(CModelViewerApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_CUSTOM_FILE_CLOSE, OnCustomFileClose)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelViewerApp construction

CModelViewerApp::CModelViewerApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_Startup = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CModelViewerApp object

CModelViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CModelViewerApp initialization

BOOL CModelViewerApp::InitInstance()
{
    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    //                      3dControls no longer needed in Visual C++ 2010
    //#ifdef _AFXDLL
    //	Enable3dControls();			// Call this when using MFC in a shared DLL
    //#else
    //	Enable3dControlsStatic();	// Call this when linking to MFC statically
    //#endif

#if defined(MV_DEBUG_MEMORY_LEAKS) && defined(_CRTDBG_MAP_ALLOC)
    // Delay calling _CrtDumpMemoryLeaks until vtk SystemToolsManager dtor
    // Requires modified VTK-6.3.0\Utilities\KWSys\vtksys\SystemTools.cxx
    AfxEnableMemoryLeakDump(FALSE);
    // Use the following to set breakpoint
    // 
    // _CrtSetBreakAlloc(1030);
    // 
    // This should be the only memory leak
    char* ptr = new char[10];
#endif

    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey(_T("USGS"));

    LoadStdProfileSettings(); // Load standard INI file options (including MRU)

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CMvDoc),
        RUNTIME_CLASS(CMainFrame), // main SDI frame window
        RUNTIME_CLASS(CMvView));
    AddDocTemplate(pDocTemplate);

    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update it.
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();

    // Change extension for help file to use HTML Help
    CString strHelpFile = m_pszHelpFilePath;
    strHelpFile.Replace(".HLP", ".chm");
    free((void*)m_pszHelpFilePath);
    m_pszHelpFilePath = _tcsdup(strHelpFile);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg()
    : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
    // Inactivates help when F1 is pressed
    // return CDialog::OnHelpInfo(pHelpInfo);
    return 0;
}

// App command to run the dialog
void CModelViewerApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewerApp message handlers

BOOL CModelViewerApp::OnIdle(LONG lCount)
{
    CMainFrame* pFrame     = (CMainFrame*)AfxGetMainWnd();
    CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

    if (pStatusBar)
    {
        CMvDoc* pDoc = (CMvDoc*)(pFrame->GetActiveView()->GetDocument());
        pStatusBar->SetPaneText(pStatusBar->CommandToIndex(ID_INDICATOR_MODEL_NAME),
                                pDoc->GetModelName());
        pStatusBar->SetPaneText(pStatusBar->CommandToIndex(ID_INDICATOR_DATA_NAME),
                                pDoc->GetDataName());
    }

    return CWinApp::OnIdle(lCount);
}

void CModelViewerApp::OnCustomFileClose()
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CMvDoc*     pDoc   = (CMvDoc*)(pFrame->GetActiveView()->GetDocument());
    pDoc->PrepareToClose();
    CWinApp::OnFileNew();
}
