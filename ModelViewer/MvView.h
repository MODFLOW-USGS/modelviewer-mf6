// MvView.h : interface of the CMvView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVVIEW_H__D3A07DDC_F3BF_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_MVVIEW_H__D3A07DDC_F3BF_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "vtkSmartPointer.h"

using std::ofstream;

class vtkLight;
class vtkRenderer;
#if 1
class vtkMFCWindow; // @replaces vtkWin32OpenGLRenderWindow and vtkWin32RenderWindowInteractor
#else
class vtkWin32OpenGLRenderWindow;
class vtkWin32RenderWindowInteractor;
#endif
class mvGUISettings;

class CMvView : public CView
{
protected: // create from serialization only
    CMvView();
    DECLARE_DYNCREATE(CMvView)

    // Attributes
public:
    CMvDoc* GetDocument();

    // Operations
public:
    void SetPrintDPI(int dpi) { m_PrintDPI = dpi; };
    int  GetPrintDPI() { return m_PrintDPI; };
    void ResetExportImageParameters();
    void RotateCamera(double angle);
    void ElevateCamera(double angle);
    void ResetViewpoint() { m_DoResetViewpoint = TRUE; }
    void ResetCameraClippingRange();
    void SetInteractorStyle(int interactorStyle);
    void SetProjectionToPerspective();
    void SetProjectionToParallel();
    void SwitchOnHeadlight(BOOL switchOn);
    void SetHeadlightIntensity(double intensity);
    void SwitchOnAuxiliaryLight(BOOL switchOn);
    void SetAuxiliaryLightIntensity(double intensity);
    void SetAuxiliaryLightPosition(double x, double y, double z);
    void SetBackgroundColor(double red, double green, double blue);
    void GetViewSettings(mvGUISettings* gui);
    void ApplyViewSettings(mvGUISettings* gui);
    void DiscardSavedViewpoint();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMvView)
public:
    virtual void OnDraw(CDC* pDC); // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();

protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CMvView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    int                             m_HeadlightOn;
    int                             m_AuxiliaryLightOn;
    double                          m_HeadlightIntensity;
    double                          m_AuxiliaryLightIntensity;

    // Objects for vtk rendering
    vtkRenderer*                    m_Renderer;
#if 1
    vtkMFCWindow*                   m_MFCWindow;           // @replaces m_RenderWindow and m_Interactor
#else
    vtkWin32OpenGLRenderWindow*     m_RenderWindow;        // replaced by m_MFCWindow->GetRenderWindow()
    vtkWin32RenderWindowInteractor* m_Interactor;          // replaced by m_MFCWindow->GetInteractor()
#endif
    vtkLight*                       m_Headlight;
    vtkLight*                       m_AuxiliaryLight;

    // Paramters relating to camera position
    double                          m_SavedCameraPosition[3];
    double                          m_SavedFocalPoint[3];
    double                          m_SavedViewUp[3];
    BOOL                            m_ViewpointSaved;
    int                             m_ViewFromDirection;
    BOOL                            m_DoResetViewpoint;

    // Parameters for printing
    int                             m_PrintDPI;
    int                             m_PreviewDPI;
    int                             m_DPI;

    // Parameters for exporting bitmap
    int                             m_BitmapResolutionOption;
    int                             m_BitmapSideOption;
    double                          m_BitmapWidthInInches;
    double                          m_BitmapHeightInInches;

    // parameters for exporting image sequence
    int                             m_StartIndex;
    int                             m_EndIndex;
    int                             m_NumberOfSteps;
    double                          m_Rotate;
    double                          m_Elevate;
    AnimationType                   m_AnimationType;
    CString                         m_OutputFolder;
    CString                         m_FilePrefix;
    CString                         m_FileStartNumber;

    // Protected methods
#if 0
    virtual LRESULT                 WindowProc(UINT message, WPARAM wParam, LPARAM lParam);  // @todo
#endif
    void                            PlaceHeadlightWithCamera();
    void                            WriteBmp(ofstream* file, BOOL useScreenResolution);

    // Generated message map functions
protected:
    //{{AFX_MSG(CMvView)
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCopyDisplayToClipboard();
    afx_msg void OnUpdateCopyDisplayToClipboard(CCmdUI* pCmdUI);
    afx_msg void OnExportAnimation();
    afx_msg void OnUpdateExportAnimation(CCmdUI* pCmdUI);
    afx_msg void OnExportAsBmp();
    afx_msg void OnUpdateExportAsBmp(CCmdUI* pCmdUI);
    afx_msg void OnExportAsVrml();
    afx_msg void OnUpdateExportAsVrml(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
    afx_msg void OnFilePrintPreview();
    afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFilePrintSetup(CCmdUI* pCmdUI);
    afx_msg void OnFitDisplayInWindow();
    afx_msg void OnUpdateFitDisplayInWindow(CCmdUI* pCmdUI);
    afx_msg void OnRecallViewpoint();
    afx_msg void OnUpdateRecallViewpoint(CCmdUI* pCmdUI);
    afx_msg void OnSaveViewpoint();
    afx_msg void OnUpdateSaveViewpoint(CCmdUI* pCmdUI);
    afx_msg void OnViewFromNextDirection();
    afx_msg void OnUpdateViewFromNextDirection(CCmdUI* pCmdUI);
    afx_msg void OnViewFromNx();
    afx_msg void OnUpdateViewFromNx(CCmdUI* pCmdUI);
    afx_msg void OnViewFromNy();
    afx_msg void OnUpdateViewFromNy(CCmdUI* pCmdUI);
    afx_msg void OnViewFromNz();
    afx_msg void OnUpdateViewFromNz(CCmdUI* pCmdUI);
    afx_msg void OnViewFromPx();
    afx_msg void OnUpdateViewFromPx(CCmdUI* pCmdUI);
    afx_msg void OnViewFromPy();
    afx_msg void OnUpdateViewFromPy(CCmdUI* pCmdUI);
    afx_msg void OnViewFromPz();
    afx_msg void OnUpdateViewFromPz(CCmdUI* pCmdUI);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // debug version in MvView.cpp
inline CMvDoc* CMvView::GetDocument()
{
    return (CMvDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MVVIEW_H__D3A07DDC_F3BF_11D3_8105_00C04F61038F__INCLUDED_)
