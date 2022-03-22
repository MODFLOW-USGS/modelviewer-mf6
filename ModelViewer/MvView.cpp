// MvView.cpp : implementation of the CMvView class
//

#include "ModelViewer.h"
#include <afxpriv.h>
#include <fstream>

#include "MvDoc.h"
#include "MvView.h"
#include "ImageGenerationDlg.h"
#include "BitmapResolutionDlg.h"
#include "ExportAnimationDlg.h"

#include "vtkRenderer.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkCamera.h"
#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkWin32RenderWindowInteractor.h"
#include "vtkInteractorStyleSwitch.h"
//#include "mvVRMLExporter.h"
#include "mvGUISettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MV_VIEW_FROM_POSITIVE_X 0
#define MV_VIEW_FROM_NEGATIVE_X 1
#define MV_VIEW_FROM_POSITIVE_Y 2
#define MV_VIEW_FROM_NEGATIVE_Y 3
#define MV_VIEW_FROM_POSITIVE_Z 4
#define MV_VIEW_FROM_NEGATIVE_Z 5

using std::ofstream;

/////////////////////////////////////////////////////////////////////////////
// CMvView

IMPLEMENT_DYNCREATE(CMvView, CView)

BEGIN_MESSAGE_MAP(CMvView, CView)
    //{{AFX_MSG_MAP(CMvView)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_COMMAND(ID_COPY_DISPLAY_TO_CLIPBOARD, OnCopyDisplayToClipboard)
    ON_UPDATE_COMMAND_UI(ID_COPY_DISPLAY_TO_CLIPBOARD, OnUpdateCopyDisplayToClipboard)
    ON_COMMAND(ID_EXPORT_ANIMATION, OnExportAnimation)
    ON_UPDATE_COMMAND_UI(ID_EXPORT_ANIMATION, OnUpdateExportAnimation)
    ON_COMMAND(ID_EXPORT_AS_BMP, OnExportAsBmp)
    ON_UPDATE_COMMAND_UI(ID_EXPORT_AS_BMP, OnUpdateExportAsBmp)
    ON_COMMAND(ID_EXPORT_AS_VRML, OnExportAsVrml)
    ON_UPDATE_COMMAND_UI(ID_EXPORT_AS_VRML, OnUpdateExportAsVrml)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnUpdateFilePrintSetup)
    ON_COMMAND(ID_FIT_DISPLAY_IN_WINDOW, OnFitDisplayInWindow)
    ON_UPDATE_COMMAND_UI(ID_FIT_DISPLAY_IN_WINDOW, OnUpdateFitDisplayInWindow)
    ON_COMMAND(ID_RECALL_VIEWPOINT, OnRecallViewpoint)
    ON_UPDATE_COMMAND_UI(ID_RECALL_VIEWPOINT, OnUpdateRecallViewpoint)
    ON_COMMAND(ID_SAVE_VIEWPOINT, OnSaveViewpoint)
    ON_UPDATE_COMMAND_UI(ID_SAVE_VIEWPOINT, OnUpdateSaveViewpoint)
    ON_COMMAND(ID_VIEW_FROM_NEXT_DIRECTION, OnViewFromNextDirection)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_NEXT_DIRECTION, OnUpdateViewFromNextDirection)
    ON_COMMAND(ID_VIEW_FROM_NX, OnViewFromNx)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_NX, OnUpdateViewFromNx)
    ON_COMMAND(ID_VIEW_FROM_NY, OnViewFromNy)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_NY, OnUpdateViewFromNy)
    ON_COMMAND(ID_VIEW_FROM_NZ, OnViewFromNz)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_NZ, OnUpdateViewFromNz)
    ON_COMMAND(ID_VIEW_FROM_PX, OnViewFromPx)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_PX, OnUpdateViewFromPx)
    ON_COMMAND(ID_VIEW_FROM_PY, OnViewFromPy)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_PY, OnUpdateViewFromPy)
    ON_COMMAND(ID_VIEW_FROM_PZ, OnViewFromPz)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FROM_PZ, OnUpdateViewFromPz)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMvView construction/destruction

CMvView::CMvView()
{
    // vtk rendering
    m_Renderer = vtkRenderer::New();
    m_Renderer->SetBackground(1, 1, 1);
#ifdef NDEBUG
    m_Renderer->GlobalWarningDisplayOff();
#endif
    m_RenderWindow = vtkWin32OpenGLRenderWindow::New();
    m_RenderWindow->AddRenderer(m_Renderer);
    m_Interactor = vtkWin32RenderWindowInteractor::New();
    m_Headlight  = vtkLight::New();
    m_Headlight->SetLightTypeToHeadlight();
    m_AuxiliaryLight = vtkLight::New();
    m_AuxiliaryLight->SetIntensity(0);
    m_Renderer->AddLight(m_Headlight);
    m_Renderer->AddLight(m_AuxiliaryLight);

    // camera position
    m_SavedCameraPosition[0]  = 0;
    m_SavedCameraPosition[1]  = 0;
    m_SavedCameraPosition[2]  = 1;
    m_SavedFocalPoint[0]      = 0;
    m_SavedFocalPoint[1]      = 0;
    m_SavedFocalPoint[2]      = 0;
    m_SavedViewUp[0]          = 0;
    m_SavedViewUp[1]          = 1;
    m_SavedViewUp[2]          = 0;
    m_ViewpointSaved          = FALSE;
    m_ViewFromDirection       = MV_VIEW_FROM_POSITIVE_Z;
    m_DoResetViewpoint        = FALSE;

    m_HeadlightIntensity      = 1.0;
    m_AuxiliaryLightIntensity = 0.0;

    // printing
    m_PrintDPI                = 100;
    m_PreviewDPI              = 100;
    m_DPI                     = 100;

    // bitmap exporting
    m_BitmapResolutionOption  = 0;
    m_BitmapSideOption        = 0;
    m_BitmapWidthInInches     = 0;
    m_BitmapHeightInInches    = 0;

    // export image sequance
    ResetExportImageParameters();
}

CMvView::~CMvView()
{
    m_Interactor->Delete();
    m_Renderer->SetRenderWindow(NULL);
    m_Renderer->RemoveLight(m_Headlight);
    m_Renderer->RemoveLight(m_AuxiliaryLight);
    m_RenderWindow->Delete();
    m_Renderer->Delete();
    m_Headlight->Delete();
    m_AuxiliaryLight->Delete();
}

BOOL CMvView::PreCreateWindow(CREATESTRUCT &cs)
{
    cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;
    return CView::PreCreateWindow(cs);
}

void CMvView::ResetExportImageParameters()
{
    m_StartIndex      = 0;
    m_EndIndex        = 0;
    m_Rotate          = 0;
    m_Elevate         = 0;
    m_NumberOfSteps   = 10;
    m_AnimationType   = atTime;
    m_OutputFolder    = "";
    m_FilePrefix      = "";
    m_FileStartNumber = "001";
}

/////////////////////////////////////////////////////////////////////////////
// CMvView drawing

void CMvView::OnDraw(CDC *pDC)
{
    CMvDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // Initialize the interactor the first time this method is called
    if (!m_Interactor->GetInitialized())
    {
        m_Interactor->SetRenderWindow(m_RenderWindow);
        WNDPROC OldProc = (WNDPROC)GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
        m_Interactor->Initialize();
        SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)OldProc);
    }
    // Printing code
    if (pDC->IsPrinting())
    {
        int   size[2];
        float scale;

        BeginWaitCursor();
        memcpy(size, m_RenderWindow->GetSize(), sizeof(int) * 2);

        float cxDIB = (float)size[0]; // Size of DIB - x
        float cyDIB = (float)size[1]; // Size of DIB - y
        CRect rcDest;

        // get size of printer page (in pixels)
        float cxPage  = (float)pDC->GetDeviceCaps(HORZRES);
        float cyPage  = (float)pDC->GetDeviceCaps(VERTRES);
        // get printer pixels per inch
        float cxInch  = (float)pDC->GetDeviceCaps(LOGPIXELSX);
        float cyInch  = (float)pDC->GetDeviceCaps(LOGPIXELSY);
        scale         = cxInch / m_DPI;

        //
        // Best Fit case -- create a rectangle which preserves
        // the DIB's aspect ratio, and fills the page horizontally.
        //
        // The formula in the "->bottom" field below calculates the Y
        // position of the printed bitmap, based on the size of the
        // bitmap, the width of the page, and the relative size of
        // a printed pixel (cyInch / cxInch).
        //
        rcDest.bottom = rcDest.left = 0;
        if ((cyDIB * cxPage / cxInch) > (cxDIB * cyPage / cyInch))
        {
            rcDest.top   = cyPage;
            rcDest.right = (cyPage * cxInch * cxDIB) / (cyInch * cyDIB);
        }
        else
        {
            rcDest.right = cxPage;
            rcDest.top   = (cxPage * cyInch * cyDIB) / (cxInch * cxDIB);
        }

        CRect rcDestLP(rcDest);
        pDC->DPtoLP(rcDestLP);
        int DPI = m_RenderWindow->GetDPI();

        m_RenderWindow->SetupMemoryRendering(rcDest.right / scale,
                                             rcDest.top / scale, pDC->GetSafeHdc());

        m_RenderWindow->Render();

        pDC->SetStretchBltMode(HALFTONE);
        ::SetBrushOrgEx(pDC->GetSafeHdc(), 0, 0, NULL);

        StretchBlt(pDC->GetSafeHdc(), 0, 0,
                   rcDest.right, rcDest.top,
                   m_RenderWindow->GetMemoryDC(),
                   0, 0, rcDest.right / scale, rcDest.top / scale, SRCCOPY);

        m_RenderWindow->ResumeScreenRendering();
        EndWaitCursor();
    }
    // Screen drawing code
    else
    {
        // Resets the camera if necessary
        if (m_DoResetViewpoint)
        {
            m_Renderer->ResetCamera();
            vtkCamera *camera = m_Renderer->GetActiveCamera();
            double    *pos    = camera->GetPosition();
            double    *fp     = camera->GetFocalPoint();
            double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                                     (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
            camera->SetPosition(fp[0], fp[1], fp[2] + dist);
            camera->SetViewUp(0, 1, 0);
            PlaceHeadlightWithCamera();
            m_DoResetViewpoint  = FALSE;
            m_ViewFromDirection = MV_VIEW_FROM_POSITIVE_Z;
        }
        if (!GetDocument()->IsAnimating())
        {
            BeginWaitCursor();
        }
        m_RenderWindow->Render();
        if (!GetDocument()->IsAnimating())
        {
            EndWaitCursor();
        }
    }
    // CView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CMvView printing

void CMvView::OnFilePrintPreview()
{
    CPrintPreviewState *pState = new CPrintPreviewState;
    if (!DoPrintPreview(IDD_PREVIEW_TOOLBAR, this,
                        RUNTIME_CLASS(CPreviewView), pState))
    {
        TRACE0("Error: DoPrintPreview failed.\n");
        AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
        delete pState;
    }
}

BOOL CMvView::OnPreparePrinting(CPrintInfo *pInfo)
{
    pInfo->SetMinPage(1);
    pInfo->SetMaxPage(1);
    return DoPreparePrinting(pInfo);
}

void CMvView::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    if (pInfo->m_bPreview)
    {
        m_DPI = m_PreviewDPI;
    }
    else
    {
        CImageGenerationDlg dlg;
        dlg.m_ImageQuality = (m_PrintDPI == 300) ? 1 : 0;
        dlg.DoModal();
        m_PrintDPI = (dlg.m_ImageQuality == 1) ? 300 : 100;
        m_DPI      = m_PrintDPI;
    }
}

void CMvView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/)
{
    // No cleanup needed after printing
}

void CMvView::OnUpdateFilePrintPreview(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!GetDocument()->IsAnimating());
}

void CMvView::OnUpdateFilePrint(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!GetDocument()->IsAnimating());
}

void CMvView::OnUpdateFilePrintSetup(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(!GetDocument()->IsAnimating());
}

/////////////////////////////////////////////////////////////////////////////
// CMvView diagnostics

#ifdef _DEBUG
void CMvView::AssertValid() const
{
    CView::AssertValid();
}

void CMvView::Dump(CDumpContext &dc) const
{
    CView::Dump(dc);
}

CMvDoc *CMvView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMvDoc)));
    return (CMvDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// The Window Function -- overrides the one implemented in the base class
LRESULT CMvView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_CHAR:
    case WM_TIMER:
        if (m_Interactor->GetInitialized())
        {
            return vtkHandleMessage2(m_hWnd, message, wParam, lParam, m_Interactor);
        }
        break;
    }
    return CView::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// Public methods

void CMvView::SetInteractorStyle(int interactorStyle)
{
    // This assumes that the interactor style is an vtkInteractorStyleSwitch
    vtkInteractorStyleSwitch *iss = vtkInteractorStyleSwitch::SafeDownCast(m_Interactor->GetInteractorStyle());
    if (iss)
    {
        if (interactorStyle == 1)
        {
            iss->SetCurrentStyleToJoystickCamera();
        }
        else
        {
            iss->SetCurrentStyleToTrackballCamera();
        }
    }
}

void CMvView::SetProjectionToPerspective()
{
    m_Renderer->GetActiveCamera()->ParallelProjectionOff();
}

void CMvView::SetProjectionToParallel()
{
    m_Renderer->GetActiveCamera()->ParallelProjectionOn();
}

void CMvView::ApplyViewSettings(mvGUISettings *gui)
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    camera->SetPosition(gui->cameraPosition);
    camera->SetFocalPoint(gui->focalPoint);
    camera->SetViewUp(gui->viewUp);
    camera->OrthogonalizeViewUp();
    camera->SetParallelProjection(gui->parallelProjection);
    camera->SetParallelScale(gui->parallelScale);
    PlaceHeadlightWithCamera();
    SetHeadlightIntensity(gui->headlightIntensity);
    // m_Headlight->SetIntensity(gui->headlightIntensity);
    // m_AuxiliaryLight->SetIntensity(gui->auxiliaryLightIntensity);
    SetAuxiliaryLightIntensity(gui->auxiliaryLightIntensity);
    m_AuxiliaryLight->SetPosition(gui->auxiliaryLightDirection);
    SwitchOnHeadlight(gui->headlightOn);
    SwitchOnAuxiliaryLight(gui->auxiliaryLightOn);
    m_Renderer->ResetCameraClippingRange();
    if (gui->customBackground)
    {
        m_Renderer->SetBackground(gui->background);
    }
    else
    {
        m_Renderer->SetBackground(1, 1, 1);
    }
}

void CMvView::RotateCamera(double angle)
{
    if (angle != 0)
    {
        vtkLightCollection *lights = m_Renderer->GetLights();
        lights->InitTraversal();
        vtkLight  *light  = lights->GetNextItem();
        vtkCamera *camera = m_Renderer->GetActiveCamera();
        camera->Azimuth(angle);
        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
    }
}

void CMvView::ElevateCamera(double angle)
{
    if (angle != 0)
    {
        vtkLightCollection *lights = m_Renderer->GetLights();
        lights->InitTraversal();
        vtkLight  *light  = lights->GetNextItem();
        vtkCamera *camera = m_Renderer->GetActiveCamera();
        camera->Elevation(angle);
        camera->OrthogonalizeViewUp();
        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
    }
}

void CMvView::ResetCameraClippingRange()
{
    m_Renderer->ResetCameraClippingRange();
}

void CMvView::SwitchOnHeadlight(BOOL switchOn)
{
    if (switchOn)
    {
        m_Headlight->SetIntensity(m_HeadlightIntensity);
    }
    else
    {
        m_HeadlightIntensity = m_Headlight->GetIntensity();
        m_Headlight->SetIntensity(0);
    }
}

void CMvView::SetHeadlightIntensity(double intensity)
{
    m_Headlight->SetIntensity(intensity);
    m_HeadlightIntensity = intensity;
}

void CMvView::SwitchOnAuxiliaryLight(BOOL switchOn)
{
    if (switchOn)
    {
        m_AuxiliaryLight->SetIntensity(m_AuxiliaryLightIntensity);
    }
    else
    {
        m_AuxiliaryLightIntensity = m_AuxiliaryLight->GetIntensity();
        m_AuxiliaryLight->SetIntensity(0);
    }
}

void CMvView::SetAuxiliaryLightIntensity(double intensity)
{
    m_AuxiliaryLight->SetIntensity(intensity);
    m_AuxiliaryLightIntensity = intensity;
}

void CMvView::SetAuxiliaryLightPosition(double x, double y, double z)
{
    m_AuxiliaryLight->SetPosition(x, y, z);
}

void CMvView::SetBackgroundColor(double red, double green, double blue)
{
    m_Renderer->SetBackground(red, green, blue);
}

void CMvView::GetViewSettings(mvGUISettings *gui)
{
    m_Renderer->GetBackground(gui->background);
    m_AuxiliaryLight->GetPosition(gui->auxiliaryLightDirection);
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    camera->GetPosition(gui->cameraPosition);
    camera->GetFocalPoint(gui->focalPoint);
    camera->GetViewUp(gui->viewUp);
    gui->parallelProjection = camera->GetParallelProjection();
    gui->parallelScale      = camera->GetParallelScale();
}

/////////////////////////////////////////////////////////////////////////////
// Protected methods

void CMvView::PlaceHeadlightWithCamera()
{
    vtkCamera          *camera = m_Renderer->GetActiveCamera();
    vtkLightCollection *lights = m_Renderer->GetLights();
    lights->InitTraversal();
    vtkLight *light = lights->GetNextItem();
    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());
}

void CMvView::WriteBmp(ofstream *file, BOOL useScreenResolution)
{
    int  *size          = m_RenderWindow->GetSize();

    // Parameters for writing bitmap at screen resolution
    int   width         = size[0];
    int   height        = size[1];
    int   XPelsPerMeter = 0;
    int   YPelsPerMeter = 0;
    float magnification = 1;

    if (!useScreenResolution)
    {
        if (m_BitmapResolutionOption == 1)
        {
            XPelsPerMeter = 5904; // 150 ppi
            YPelsPerMeter = 5904;
            if (m_BitmapSideOption == 0)
            {
                width         = m_BitmapWidthInInches * 150;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = m_BitmapHeightInInches * 150;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
        else if (m_BitmapResolutionOption == 2)
        {
            XPelsPerMeter = 11808; // 300 ppi
            YPelsPerMeter = 11808;
            if (m_BitmapSideOption == 0)
            {
                width         = m_BitmapWidthInInches * 300;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = m_BitmapHeightInInches * 300;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
    }
    int dataWidth = ((width * 3 + 3) / 4) * 4;

    // write the bitmap file header
    file->put((char)66);
    file->put((char)77);
    long temp = (long)(dataWidth * height) + 54L;
    file->put((char)(temp % 256));
    file->put((char)((temp % 65536L) / 256));
    file->put((char)(temp / 65536L));
    for (int row = 0; row < 5; row++)
    {
        file->put((char)0);
    }
    file->put((char)54);
    file->put((char)0);
    file->put((char)0);
    file->put((char)0);

    // write the bitmap info header
    file->put((char)40); // biSize
    file->put((char)0);
    file->put((char)0);
    file->put((char)0);

    file->put((char)(width % 256)); // biWidth
    file->put((char)(width / 256));
    file->put((char)0);
    file->put((char)0);

    file->put((char)(height % 256)); // biHeight
    file->put((char)(height / 256));
    file->put((char)0);
    file->put((char)0);

    file->put((char)1); // biPlanes
    file->put((char)0);
    file->put((char)24); // biBitCount
    file->put((char)0);

    for (int row = 0; row < 8; row++) // biCompression and biSizeImage
    {
        file->put((char)0);
    }

    file->put((char)(XPelsPerMeter % 256)); // biXPelsPerMeter
    file->put((char)(XPelsPerMeter / 256));
    file->put((char)0);
    file->put((char)0);

    file->put((char)(XPelsPerMeter % 256)); // biYPelsPerMeter
    file->put((char)(XPelsPerMeter / 256));
    file->put((char)0);
    file->put((char)0);

    for (int row = 0; row < 8; row++) // biClrUsed and biClrImportant
    {
        file->put((char)0);
    }

    // temporarily change color bar and text size
    CMvDoc       *pDoc              = GetDocument();
    int           colorBarWidth     = pDoc->GetColorBarWidth();
    int           colorBarHeight    = pDoc->GetColorBarHeight();
    int           colorBarOffset    = pDoc->GetColorBarOffset();
    int           colorBarFontSize  = pDoc->GetColorBarFontSize();
    int           timeLabelFontSize = pDoc->GetTimeLabelFontSize();
    const double *pos               = pDoc->GetTimeLabelPosition();
    double        timeLabelPos[2];
    timeLabelPos[0] = pos[0];
    timeLabelPos[1] = pos[1];
    if (magnification != 1)
    {
        pDoc->SetColorBarSize((int)(colorBarWidth * magnification),
                              (int)(colorBarHeight * magnification),
                              (int)(colorBarOffset * magnification), FALSE);
        pDoc->SetColorBarFontSize((int)(colorBarFontSize * magnification), FALSE);
        pDoc->SetTimeLabelFontSize((int)(timeLabelFontSize * magnification), FALSE);
        pDoc->SetTimeLabelPosition(timeLabelPos[0] * magnification,
                                   timeLabelPos[1] * magnification, FALSE);
    }
    // render to memory and write bitmap data
    m_RenderWindow->SetupMemoryRendering(width, height, GetDC()->GetSafeHdc());
    m_RenderWindow->Render();
    file->write((char *)m_RenderWindow->GetMemoryData(), dataWidth * height);
    if (magnification != 1)
    {
        pDoc->SetColorBarSize(colorBarWidth, colorBarHeight, colorBarOffset, FALSE);
        pDoc->SetColorBarFontSize(colorBarFontSize, FALSE);
        pDoc->SetTimeLabelFontSize(timeLabelFontSize, FALSE);
        pDoc->SetTimeLabelPosition(timeLabelPos[0], timeLabelPos[1], FALSE);
    }
    m_RenderWindow->ResumeScreenRendering();
}

/////////////////////////////////////////////////////////////////////////////
// Overrides

void CMvView::OnInitialUpdate()
{
    CView::OnInitialUpdate();
    m_RenderWindow->SetWindowId(m_hWnd);
    m_RenderWindow->WindowInitialize();
    GetDocument()->CreateToolDialogs();
    SetInteractorStyle(GetDocument()->GetInteractorStyle());
}

void CMvView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)
{
    m_Renderer->GetViewProps()->RemoveAllItems();
    vtkSmartPointer<vtkPropCollection> props = GetDocument()->GetPropCollection();
    props->InitTraversal();
    for (int i = 0; i < props->GetNumberOfItems(); i++)
    {
        m_Renderer->AddViewProp(props->GetNextProp());
    }
    if (!GetDocument()->IsAnimating())
    {
        BeginWaitCursor();
    }
    m_Renderer->ResetCameraClippingRange();
    if (!GetDocument()->IsAnimating())
    {
        EndWaitCursor();
    }
    CView::OnUpdate(pSender, lHint, pHint);
}

/////////////////////////////////////////////////////////////////////////////
// Windows message handlers

int CMvView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;
    m_RenderWindow->SetParentId(lpCreateStruct->hwndParent);
    return 0;
}

BOOL CMvView::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

void CMvView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    if (m_Interactor->GetInitialized())
    {
        m_Interactor->SetSize(cx, cy);
    }
}

void CMvView::OnCopyDisplayToClipboard()
{
    LPBITMAPINFOHEADER lpbi;
    DWORD              dwLen;
    HANDLE             hDIB      = NULL;
    int               *size      = m_RenderWindow->GetSize();
    int                dataWidth = ((size[0] * 3 + 3) / 4) * 4;

    if (OpenClipboard())
    {
        BeginWaitCursor();
        EmptyClipboard();

        dwLen                = sizeof(BITMAPINFOHEADER) + dataWidth * size[1];
        hDIB                 = ::GlobalAlloc(GHND, dwLen);
        lpbi                 = (LPBITMAPINFOHEADER)::GlobalLock(hDIB);

        lpbi->biSize         = sizeof(BITMAPINFOHEADER);
        lpbi->biWidth        = size[0];
        lpbi->biHeight       = size[1];
        lpbi->biPlanes       = 1;
        lpbi->biBitCount     = 24;
        lpbi->biCompression  = BI_RGB;
        lpbi->biClrUsed      = 0;
        lpbi->biClrImportant = 0;
        lpbi->biSizeImage    = dataWidth * size[1];

        m_RenderWindow->SetupMemoryRendering(size[0], size[1], GetDC()->GetSafeHdc());
        m_RenderWindow->Render();

        memcpy((LPSTR)lpbi + lpbi->biSize, m_RenderWindow->GetMemoryData(),
               dataWidth * size[1]);

        SetClipboardData(CF_DIB, hDIB);
        ::GlobalUnlock(hDIB);
        CloseClipboard();
        m_RenderWindow->ResumeScreenRendering();
        EndWaitCursor();
    }
}

void CMvView::OnUpdateCopyDisplayToClipboard(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnExportAnimation()
{
    CMvDoc *pDoc = GetDocument();
    char    b1[10], b2[10], buffer[200];
    CString path;

    CRect   rect;
    GetParentFrame()->GetClientRect(rect);
    int                 spaceUsagePerFile = (rect.right * rect.bottom) * .00293;

    CExportAnimationDlg dlg;
    dlg.m_StartIndex         = m_StartIndex;
    dlg.m_EndIndex           = m_EndIndex;
    dlg.m_OutputFolder       = m_OutputFolder;
    dlg.m_FilePrefix         = m_FilePrefix;
    dlg.m_FileStartNumber    = m_FileStartNumber;
    dlg.m_Rotate             = m_Rotate;
    dlg.m_Elevate            = m_Elevate;
    dlg.m_SpaceUsagePerFile  = spaceUsagePerFile;
    dlg.m_NumberOfTimePoints = pDoc->GetNumberOfTimePoints();
    dlg.m_TimePointLabels    = pDoc->GetTimePointLabels();
    dlg.m_NumberOfSteps      = m_NumberOfSteps;
    dlg.m_AnimationType      = m_AnimationType;

    if (dlg.DoModal() != IDOK)
    {
        return;
    }
    m_StartIndex   = dlg.m_StartIndex;
    m_EndIndex     = dlg.m_EndIndex;
    m_OutputFolder = dlg.m_OutputFolder;
    m_FilePrefix   = dlg.m_FilePrefix;
    m_FilePrefix.TrimLeft();
    m_FilePrefix.TrimRight();
    m_FileStartNumber = dlg.m_FileStartNumber;
    m_FileStartNumber.TrimLeft();
    m_FileStartNumber.TrimRight();
    m_Rotate  = dlg.m_Rotate;
    m_Elevate = dlg.m_Elevate;
    switch (dlg.m_AnimationType)
    {
    case 0:
        m_AnimationType = atTime;
        break;
    case 1:
        m_AnimationType = atSpace;
        break;
    default:
        m_AnimationType = atTime;
        break;
    }
    m_NumberOfSteps = dlg.m_NumberOfSteps;

    int StartIndex;
    int EndIndex;
    switch (m_AnimationType)
    {
    case atTime:
        StartIndex = m_StartIndex;
        EndIndex   = m_EndIndex;
        break;

    case atSpace:
        StartIndex = 0;
        EndIndex   = m_NumberOfSteps - 1;
        break;
    }

    if (!dlg.m_OutputFolder.IsEmpty())
    {
        path = dlg.m_OutputFolder + "\\" + m_FilePrefix;
    }
    else
    {
        path = m_FilePrefix;
    }

    if (!dlg.m_Preview)
    {
        // Issue warning for large output (greater than 10 megabytes)
        int diskUsage = (EndIndex - StartIndex + 1) * spaceUsagePerFile;
        if (diskUsage > 10000)
        {
            sprintf(buffer, "You are about to write a total of %d KBytes to disk. Do you want to continue?", diskUsage);
            if (AfxMessageBox(buffer, MB_YESNO) == IDNO)
            {
                return;
            }
        }
    }

    PlaceHeadlightWithCamera();

    vtkLightCollection *lights = m_Renderer->GetLights();
    lights->InitTraversal();
    vtkLight  *light    = lights->GetNextItem();
    vtkLight  *auxlight = lights->GetNextItem();
    vtkCamera *camera   = m_Renderer->GetActiveCamera();

    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());
    auxlight->SetFocalPoint(camera->GetFocalPoint());

    pDoc->BeginWaitCursor();
    if (m_AnimationType == atTime)
    {
        pDoc->SetTimePointTo(m_StartIndex);
    }
    int       fileNumber = atoi((char *)((LPCTSTR)m_FileStartNumber));
    int       len        = m_FileStartNumber.GetLength();
    int       i          = StartIndex;
    int       fill;

    ofstream *file = new ofstream;
    while (1)
    {
        if (!dlg.m_Preview)
        {
            b1[0] = '\0';
            sprintf(b2, "%d", fileNumber);
            fill = len - strlen(b2);
            for (int j = 0; j < fill; j++)
            {
                strcat(b1, "0");
            }
            file->open((LPCTSTR)(path + b1 + b2 + ".bmp"), ios::out | ios::binary);
            WriteBmp(file, TRUE);
            file->close();
            fileNumber++;
        }
        i++;
        if (i > EndIndex)
        {
            break;
        }
        camera->Azimuth(m_Rotate);
        camera->Elevation(m_Elevate);

        if (m_Elevate != 0)
        {
            camera->OrthogonalizeViewUp();
        }
        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
        auxlight->SetFocalPoint(camera->GetFocalPoint());

        if (m_AnimationType == atTime)
        {
            pDoc->AdvanceOneTimePoint();
        }
    }
    delete file;
    pDoc->EndWaitCursor();
}

void CMvView::OnUpdateExportAnimation(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnExportAsBmp()
{
    {
        CBitmapResolutionDlg dlg;
        dlg.m_ResolutionOption = m_BitmapResolutionOption;
        dlg.m_SideOption       = m_BitmapSideOption;
        dlg.m_WidthInInches    = m_BitmapWidthInInches;
        dlg.m_HeightInInches   = m_BitmapHeightInInches;
        if (dlg.DoModal() != IDOK)
        {
            return;
        }
        m_BitmapResolutionOption = dlg.m_ResolutionOption;
        m_BitmapSideOption       = dlg.m_SideOption;
        m_BitmapWidthInInches    = dlg.m_WidthInInches;
        m_BitmapHeightInInches   = dlg.m_HeightInInches;
    }
    CString fileName;
    {
        CFileDialog fileDlg(FALSE, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                            "Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
        if (fileDlg.DoModal() != IDOK)
        {
            return;
        }
        fileName = fileDlg.GetPathName();
    }

    PlaceHeadlightWithCamera();

    vtkLightCollection *lights = m_Renderer->GetLights();
    lights->InitTraversal();
    lights->GetNextItem();
    // get auxilliary light
    vtkLight  *light  = lights->GetNextItem();
    vtkCamera *camera = m_Renderer->GetActiveCamera();

    camera->Azimuth(m_Rotate);
    camera->Elevation(m_Elevate);

    if (m_Elevate != 0)
    {
        camera->OrthogonalizeViewUp();
    }

    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());

    BeginWaitCursor();
    ofstream *file = new ofstream((LPCTSTR)fileName, ios::out | ios::binary);
    WriteBmp(file, m_BitmapResolutionOption == 0);
    file->close();
    delete file;
    EndWaitCursor();
}

void CMvView::OnUpdateExportAsBmp(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnExportAsVrml()
{
    /*
    CFileDialog fileDlg(FALSE, "wrl", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Vrml Worlds (*.wrl)|*.wrl|All Files (*.*)|*.*||");
    if (fileDlg.DoModal() == IDOK)
    {
        // Remove the lights. This causes the VRML exporter to add
        // a headlight to the output
        m_Renderer->RemoveLight(m_Headlight);
        m_Renderer->RemoveLight(m_AuxiliaryLight);

        // Write the vrml world
        mvVRMLExporter *VRMLExporter = mvVRMLExporter::New();
        VRMLExporter->SetRenderWindow(m_RenderWindow);
        VRMLExporter->SetFileName((char *) ((LPCTSTR) fileDlg.GetPathName()));
        VRMLExporter->Write();
        VRMLExporter->Delete();

        // add back the lights
        m_Renderer->AddLight(m_Headlight);
        m_Renderer->AddLight(m_AuxiliaryLight);
    }
    */
}

void CMvView::OnUpdateExportAsVrml(CCmdUI *pCmdUI)
{
    //	pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
    //								!GetDocument()->IsAnimating());
    pCmdUI->Enable(0);
}

void CMvView::OnFitDisplayInWindow()
{
    m_Renderer->ResetCamera();
    Invalidate();
}

void CMvView::OnUpdateFitDisplayInWindow(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnRecallViewpoint()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    camera->SetPosition(m_SavedCameraPosition);
    camera->SetFocalPoint(m_SavedFocalPoint);
    camera->SetViewUp(m_SavedViewUp);
    camera->OrthogonalizeViewUp();
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    Invalidate();
}

void CMvView::OnUpdateRecallViewpoint(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_ViewpointSaved &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnSaveViewpoint()
{
    vtkCamera *camera     = m_Renderer->GetActiveCamera();
    double    *position   = camera->GetPosition();
    double    *focalPoint = camera->GetFocalPoint();
    double    *viewUp     = camera->GetViewUp();

    for (int i = 0; i < 3; i++)
    {
        m_SavedCameraPosition[i] = position[i];
        m_SavedFocalPoint[i]     = focalPoint[i];
        m_SavedViewUp[i]         = viewUp[i];
    }
    m_ViewpointSaved = TRUE;
}

void CMvView::OnUpdateSaveViewpoint(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromNextDirection()
{
    switch (m_ViewFromDirection)
    {
    case MV_VIEW_FROM_NEGATIVE_X:
        OnViewFromNy();
        return;
    case MV_VIEW_FROM_NEGATIVE_Y:
        OnViewFromPx();
        return;
    case MV_VIEW_FROM_POSITIVE_X:
        OnViewFromPy();
        return;
    case MV_VIEW_FROM_POSITIVE_Y:
        OnViewFromNz();
        return;
    case MV_VIEW_FROM_NEGATIVE_Z:
        OnViewFromPz();
        return;
    case MV_VIEW_FROM_POSITIVE_Z:
        OnViewFromNx();
        return;
    }
}

void CMvView::OnUpdateViewFromNextDirection(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromPx()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0] + dist, fp[1], fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_POSITIVE_X;
    Invalidate();
}

void CMvView::OnUpdateViewFromPx(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromNx()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0] - dist, fp[1], fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_NEGATIVE_X;
    Invalidate();
}

void CMvView::OnUpdateViewFromNx(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromPy()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1] + dist, fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_POSITIVE_Y;
    Invalidate();
}

void CMvView::OnUpdateViewFromPy(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromNy()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1] - dist, fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_NEGATIVE_Y;
    Invalidate();
}

void CMvView::OnUpdateViewFromNy(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromPz()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1], fp[2] + dist);
    camera->SetViewUp(0, 1, 0);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_POSITIVE_Z;
    Invalidate();
}

void CMvView::OnUpdateViewFromPz(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::OnViewFromNz()
{
    vtkCamera *camera = m_Renderer->GetActiveCamera();
    double    *pos    = camera->GetPosition();
    double    *fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                             (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1], fp[2] - dist);
    camera->SetViewUp(0, -1, 0);
    PlaceHeadlightWithCamera();
    m_Renderer->ResetCameraClippingRange();
    m_ViewFromDirection = MV_VIEW_FROM_NEGATIVE_Z;
    Invalidate();
}

void CMvView::OnUpdateViewFromNz(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_Renderer->VisibleActorCount() > 0 &&
                   !GetDocument()->IsAnimating());
}

void CMvView::DiscardSavedViewpoint()
{
    m_ViewpointSaved = FALSE;
}