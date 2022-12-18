#include "mvview.h"

#include "mvdoc.h"
#include "mvGUISettings.h"

#include <QtWidgets>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkBMPWriter.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>

#include <vtkPNGWriter.h>
#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkRenderLargeImage.h>


//#include <vtkViewport.h> // testing

#include "bitmapresolutiondialog.h"
#include "exportanimationdialog.h"

#define MV_VIEW_FROM_POSITIVE_X 0
#define MV_VIEW_FROM_NEGATIVE_X 1
#define MV_VIEW_FROM_POSITIVE_Y 2
#define MV_VIEW_FROM_NEGATIVE_Y 3
#define MV_VIEW_FROM_POSITIVE_Z 4
#define MV_VIEW_FROM_NEGATIVE_Z 5


MvView::MvView(QObject *parent)
    : QAbstractView(parent)
{
    //renderer = vtkRenderer::New();
    //renderer->SetBackground(1, 1, 1);

#ifdef NDEBUG
    //renderer->GlobalWarningDisplayOff();
#endif

    ////widget    = new QVTKOpenGLNativeWidget();

    //headlight = vtkLight::New();
    //headlight->SetLightTypeToHeadlight();

    //auxiliaryLight = vtkLight::New();
    //auxiliaryLight->SetIntensity(0);

    //renderer->AddLight(headlight);
    //renderer->AddLight(auxiliaryLight);

    //widget->renderWindow()->AddRenderer(renderer);

    // camera position
    viewFromDirection    = MV_VIEW_FROM_POSITIVE_Z;
    doResetViewpoint     = false;

    // bitmap exporting
    bitmapResolutionOption = ResolutionType::rtScreen;
    bitmapSideOption       = SideType::stWidth;
    bitmapWidthInInches    = 0;
    bitmapHeightInInches   = 0;

    // mouseMode
    setInteractorStyle(GetDocument()->interactorStyle());

    // export image sequance
    resetExportImageParameters();
}

MvView::~MvView()
{
    //renderer->SetRenderWindow(nullptr);
    //renderer->RemoveLight(headlight);
    //renderer->RemoveLight(auxiliaryLight);

    //renderer->Delete();
    //headlight->Delete();
    //auxiliaryLight->Delete();

    ///delete widget;
}

//#if 1 // use vtkViewport
////
//vtkWindow* MvView::GetVTKWindow()
//{
//    return widget->renderWindow();
//}
//
//#endif

/////////////////////////////////////////////////////////////////////////////
// MvView diagnostics

#ifdef _DEBUG
//void MvView::AssertValid() const
//{
//    CView::AssertValid();
//}
//
//void MvView::Dump(CDumpContext &dc) const
//{
//    CView::Dump(dc);
//}

MvDoc* MvView::GetDocument() // non-debug version is inline
{
    MvDoc* doc = dynamic_cast<MvDoc*>(parent());
    assert(doc);
    return doc;
}
#endif //_DEBUG

QVTKOpenGLNativeWidget* MvView::mainWidget()
{
    return widget;
}

//void MvView::addActor(vtkProp* p)
//{
//    this->renderer->AddActor(p);
//}
//
//void MvView::addViewProp(vtkProp* p)
//{
//    this->renderer->AddViewProp(p);
//}

//void MvView::applyViewSettings(mvGUISettings* gui)
//{
//    /*
//    vtkCamera* camera = m_Renderer->GetActiveCamera();
//    camera->SetPosition(gui->cameraPosition);
//    camera->SetFocalPoint(gui->focalPoint);
//    camera->SetViewUp(gui->viewUp);
//    camera->OrthogonalizeViewUp();
//    camera->SetParallelProjection(gui->parallelProjection);
//    camera->SetParallelScale(gui->parallelScale);
//    PlaceHeadlightWithCamera();
//    SetHeadlightIntensity(gui->headlightIntensity);
//    // m_Headlight->SetIntensity(gui->headlightIntensity);
//    // m_AuxiliaryLight->SetIntensity(gui->auxiliaryLightIntensity);
//    SetAuxiliaryLightIntensity(gui->auxiliaryLightIntensity);
//    m_AuxiliaryLight->SetPosition(gui->auxiliaryLightDirection);
//    SwitchOnHeadlight(gui->headlightOn);
//    SwitchOnAuxiliaryLight(gui->auxiliaryLightOn);
//    m_Renderer->ResetCameraClippingRange();
//    if (gui->customBackground)
//    {
//        m_Renderer->SetBackground(gui->background);
//    }
//    else
//    {
//        m_Renderer->SetBackground(1, 1, 1);
//    }
//    */
//
//    //{{
//    vtkCamera* camera = this->renderer->GetActiveCamera();
//    camera->SetPosition(gui->cameraPosition);
//    camera->SetFocalPoint(gui->focalPoint);
//    camera->SetViewUp(gui->viewUp);
//    camera->OrthogonalizeViewUp();
//    camera->SetParallelProjection(gui->parallelProjection);
//    camera->SetParallelScale(gui->parallelScale);
//    PlaceHeadlightWithCamera();
//    //SetHeadlightIntensity(gui->headlightIntensity);
//    //// m_Headlight->SetIntensity(gui->headlightIntensity);
//    //// m_AuxiliaryLight->SetIntensity(gui->auxiliaryLightIntensity);
//    //SetAuxiliaryLightIntensity(gui->auxiliaryLightIntensity);
//    //m_AuxiliaryLight->SetPosition(gui->auxiliaryLightDirection);
//    //SwitchOnHeadlight(gui->headlightOn);
//    //SwitchOnAuxiliaryLight(gui->auxiliaryLightOn);
//    this->renderer->ResetCameraClippingRange();
//    if (gui->customBackground)
//    {
//        this->renderer->SetBackground(gui->background);
//    }
//    else
//    {
//        this->renderer->SetBackground(1, 1, 1);
//    }
//    //}}
//
//    ////this->renderer->ResetCamera();
//    this->mainWidget()->renderWindow()->Render();
//}

void MvView::onUpdate(QAbstractView* sender, QObject* hint)
{
    //m_Renderer->GetViewProps()->RemoveAllItems();
    //vtkSmartPointer<vtkPropCollection> props = GetDocument()->GetPropCollection();
    //props->InitTraversal();
    //for (int i = 0; i < props->GetNumberOfItems(); i++)
    //{
    //    m_Renderer->AddViewProp(props->GetNextProp());
    //}
    //if (!GetDocument()->IsAnimating())
    //{
    //    BeginWaitCursor();
    //}
    //m_Renderer->ResetCameraClippingRange();
    //if (!GetDocument()->IsAnimating())
    //{
    //    EndWaitCursor();
    //}
    //CView::OnUpdate(pSender, lHint, pHint);

    //this->renderer->GetViewProps()->RemoveAllItems();
    //vtkSmartPointer<vtkPropCollection> props = GetDocument()->propCollection();
    //props->InitTraversal();
    //for (int i = 0; i < props->GetNumberOfItems(); i++)
    //{
    //    this->renderer->AddViewProp(props->GetNextProp());
    //}
    //if (!this->GetDocument()->isAnimating())
    //{
    //    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    //}
    //this->renderer->ResetCameraClippingRange();
    //if (!GetDocument()->isAnimating())
    //{
    //    QGuiApplication::restoreOverrideCursor();
    //}

    //{{ added for File->New
    this->renderer->GetViewProps()->RemoveAllItems();
    vtkSmartPointer<vtkPropCollection> props = GetDocument()->propCollection();
    props->InitTraversal();
    for (int i = 0; i < props->GetNumberOfItems(); i++)
    {
        this->renderer->AddViewProp(props->GetNextProp());
    }
    if (!this->GetDocument()->isAnimating())
    {
        QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    }
    this->renderer->ResetCameraClippingRange();
    if (!GetDocument()->isAnimating())
    {
        QGuiApplication::restoreOverrideCursor();
    }
    //}} added for File->New

    //{{
    //renderer->ResetCamera();
    //}}

    widget->renderWindow()->Render();

    // The following was moved to -- void MvDoc::updateAnimation()
    ////{{
    //// added so that File->Export Animation->Preview would update the view
    //qApp->processEvents();
    ////}}

    QAbstractView::onUpdate(sender, hint);
}

void MvView::resetExportImageParameters()
{
    startIndex        = 0;
    endIndex          = 0;
    rotate            = 0;
    elevate           = 0;
    numberOfSteps     = 10;
    animationType     = AnimationType::atTime;
    outputFolder      = "";
    filePrefix        = "";
    fileStartNumber   = "001";
}

/////////////////////////////////////////////////////////////////////////////
// Protected methods

void MvView::WriteBmp(const char* filename, bool useScreenResolution)
{
    this->widget->renderWindow()->SetUseOffScreenBuffers(true);

    int size[2];
    size[0]             = this->widget->renderWindow()->GetSize()[0];
    size[1]             = this->widget->renderWindow()->GetSize()[1];

    // Parameters for writing bitmap at screen resolution
    int   width         = size[0];
    int   height        = size[1];
    float magnification = 1;

    if (!useScreenResolution)
    {
        if (bitmapResolutionOption == ResolutionType::rt150ppi)
        {
            // 150 ppi
            if (bitmapSideOption == SideType::stWidth)
            {
                width         = bitmapWidthInInches * 150;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = bitmapHeightInInches * 150;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
        else if (bitmapResolutionOption == ResolutionType::rt300ppi)
        {
            // 300 ppi
            if (bitmapSideOption == SideType::stWidth)
            {
                width         = bitmapWidthInInches * 300;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = bitmapHeightInInches * 300;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
    }
    int dataWidth = ((width * 3 + 3) / 4) * 4;



    // temporarily change color bar and text size
    MvDoc*        doc               = GetDocument();
    int           colorBarWidth     = doc->GetColorBarWidth();
    int           colorBarHeight    = doc->GetColorBarHeight();
    int           colorBarOffset    = doc->GetColorBarOffset();
    int           colorBarFontSize  = doc->GetColorBarFontSize();
    int           timeLabelFontSize = doc->GetTimeLabelFontSize();
    const double* pos               = doc->GetTimeLabelPosition();
    double        timeLabelPos[2];
    timeLabelPos[0] = pos[0];
    timeLabelPos[1] = pos[1];
    if (magnification != 1)
    {
        doc->SetColorBarSize((int)(colorBarWidth * magnification),
                             (int)(colorBarHeight * magnification),
                             (int)(colorBarOffset * magnification), false);
        doc->SetColorBarFontSize((int)(colorBarFontSize * magnification), false);
        doc->SetTimeLabelFontSize((int)(timeLabelFontSize * magnification), false);
        doc->SetTimeLabelPosition(timeLabelPos[0] * magnification,
                                  timeLabelPos[1] * magnification, false);
    }

    // render to memory and write bitmap data
    this->widget->renderWindow()->Render();
    {
        // vtkRenderLargeImage
        this->widget->renderWindow()->SetSize(width, height);

        vtkNew<vtkRenderLargeImage> renderLarge;
        renderLarge->SetInput(renderer);
        renderLarge->SetMagnification(1);

        vtkNew<vtkBMPWriter> bmpWriter;
        bmpWriter->SetFileName(filename);
        bmpWriter->SetInputConnection(renderLarge->GetOutputPort());
        bmpWriter->Write();
    }
    if (magnification != 1)
    {
        doc->SetColorBarSize(colorBarWidth, colorBarHeight, colorBarOffset, false);
        doc->SetColorBarFontSize(colorBarFontSize, false);
        doc->SetTimeLabelFontSize(timeLabelFontSize, false);
        doc->SetTimeLabelPosition(timeLabelPos[0], timeLabelPos[1], false);
    }
    if (magnification != 1)
    {
        this->widget->renderWindow()->SetSize(size);
    }
    this->widget->renderWindow()->SetUseOffScreenBuffers(false);
}

void MvView::WritePng(const char* filename, bool useScreenResolution)
{
    widget->renderWindow()->SetUseOffScreenBuffers(true);

    int size[2];
    size[0]             = widget->renderWindow()->GetSize()[0];
    size[1]             = widget->renderWindow()->GetSize()[1];

    // Parameters for writing bitmap at screen resolution
    int   width         = size[0];
    int   height        = size[1];
    float magnification = 1;

    if (!useScreenResolution)
    {
        if (bitmapResolutionOption == ResolutionType::rt150ppi)
        {
            // 150 ppi
            if (bitmapSideOption == SideType::stWidth)
            {
                width         = bitmapWidthInInches * 150;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = bitmapHeightInInches * 150;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
        else if (bitmapResolutionOption == ResolutionType::rt300ppi)
        {
            // 300 ppi
            if (bitmapSideOption == SideType::stWidth)
            {
                width         = bitmapWidthInInches * 300;
                height        = (((long)width) * size[1]) / size[0];
                magnification = ((float)width) / size[0];
            }
            else
            {
                height        = bitmapHeightInInches * 300;
                width         = (((long)height) * size[0]) / size[1];
                magnification = ((float)height) / size[1];
            }
        }
    }
    int           dataWidth         = ((width * 3 + 3) / 4) * 4;

    // temporarily change color bar and text size
    MvDoc*        doc               = GetDocument();
    int           colorBarWidth     = doc->GetColorBarWidth();
    int           colorBarHeight    = doc->GetColorBarHeight();
    int           colorBarOffset    = doc->GetColorBarOffset();
    int           colorBarFontSize  = doc->GetColorBarFontSize();
    int           timeLabelFontSize = doc->GetTimeLabelFontSize();
    const double* pos               = doc->GetTimeLabelPosition();
    double        timeLabelPos[2];
    timeLabelPos[0] = pos[0];
    timeLabelPos[1] = pos[1];
    if (magnification != 1)
    {
        doc->SetColorBarSize((int)(colorBarWidth * magnification),
                             (int)(colorBarHeight * magnification),
                             (int)(colorBarOffset * magnification), false);
        doc->SetColorBarFontSize((int)(colorBarFontSize * magnification), false);
        doc->SetTimeLabelFontSize((int)(timeLabelFontSize * magnification), false);
        doc->SetTimeLabelPosition(timeLabelPos[0] * magnification,
                                  timeLabelPos[1] * magnification, false);
    }

    // render to memory and write bitmap data
    widget->renderWindow()->Render();
    {
        // vtkRenderLargeImage
        widget->renderWindow()->SetSize(width, height);

        vtkNew<vtkRenderLargeImage> renderLarge;
        renderLarge->SetInput(renderer);
        renderLarge->SetMagnification(1);

        vtkNew<vtkPNGWriter> pngWriter;

        pngWriter->SetFileName(filename);
        pngWriter->SetInputConnection(renderLarge->GetOutputPort());
        pngWriter->Write();
    }
    if (magnification != 1)
    {
        doc->SetColorBarSize(colorBarWidth, colorBarHeight, colorBarOffset, false);
        doc->SetColorBarFontSize(colorBarFontSize, false);
        doc->SetTimeLabelFontSize(timeLabelFontSize, false);
        doc->SetTimeLabelPosition(timeLabelPos[0], timeLabelPos[1], false);
    }
    if (magnification != 1)
    {
        widget->renderWindow()->SetSize(size);
    }
    widget->renderWindow()->SetUseOffScreenBuffers(false);
}

void MvView::onFileExportAsBmp(QWidget* parent)
{
    BitmapResolutionDialog dlg(parent);
    dlg.resolutionOption = bitmapResolutionOption;
    dlg.sideOption       = bitmapSideOption;
    dlg.widthInInches    = bitmapWidthInInches;
    dlg.heightInInches   = bitmapHeightInInches;
    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    bitmapResolutionOption = dlg.resolutionOption;
    bitmapSideOption       = dlg.sideOption;
    bitmapWidthInInches    = dlg.widthInInches;
    bitmapHeightInInches   = dlg.heightInInches;

    QString fileName = QFileDialog::getSaveFileName(parent,
                                                tr("Save As"),
                                                tr(""),
                                                tr("Bitmap Files (*.bmp);;All Files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }

    PlaceHeadlightWithCamera();

    vtkLightCollection* lights =  this->renderer->GetLights();
    lights->InitTraversal();
    
    // get headlight
    assert(lights->IsItemPresent(this->headlight));
    vtkLight* light = lights->GetNextItem();
    assert(light == this->headlight);

    // get auxilliary light
    assert(lights->IsItemPresent(this->auxiliaryLight));
    light = lights->GetNextItem();
    assert(light == this->auxiliaryLight);

    vtkCamera* camera = this->renderer->GetActiveCamera();

    camera->Azimuth(this->rotate);
    camera->Elevation(this->elevate);

    if (this->elevate != 0)
    {
        camera->OrthogonalizeViewUp();
    }

    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    WriteBmp(fileName.toLocal8Bit().data(), bitmapResolutionOption == ResolutionType::rtScreen);
    QApplication::restoreOverrideCursor();
}

void MvView::onFileExportAsPng(QWidget* parent)
{
    BitmapResolutionDialog dlg(parent);
    dlg.resolutionOption = bitmapResolutionOption;
    dlg.sideOption       = bitmapSideOption;
    dlg.widthInInches    = bitmapWidthInInches;
    dlg.heightInInches   = bitmapHeightInInches;
    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }
    bitmapResolutionOption = dlg.resolutionOption;
    bitmapSideOption       = dlg.sideOption;
    bitmapWidthInInches    = dlg.widthInInches;
    bitmapHeightInInches   = dlg.heightInInches;

    QString fileName       = QFileDialog::getSaveFileName(parent,
                                                    tr("Save As"),
                                                    tr(""),
                                                    tr("PNG (*.png);;All Files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }

    PlaceHeadlightWithCamera();

    vtkLightCollection* lights = this->renderer->GetLights();
    lights->InitTraversal();

    // get headlight
    assert(lights->IsItemPresent(this->headlight));
    vtkLight* light = lights->GetNextItem();
    assert(light == this->headlight);

    // get auxilliary light
    assert(lights->IsItemPresent(this->auxiliaryLight));
    light = lights->GetNextItem();
    assert(light == this->auxiliaryLight);

    vtkCamera* camera = this->renderer->GetActiveCamera();

    camera->Azimuth(this->rotate);
    camera->Elevation(this->elevate);

    if (this->elevate != 0)
    {
        camera->OrthogonalizeViewUp();
    }

    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());

    QApplication::setOverrideCursor(Qt::WaitCursor);
    WritePng(fileName.toLocal8Bit().data(), bitmapResolutionOption == ResolutionType::rtScreen);
    QApplication::restoreOverrideCursor();
}


void MvView::onFileExportAnimation(QWidget* parent)
{
    //const double          MB_PER_PIXEL = 2.88e-6;
    const double          MB_PER_PIXEL = 8.43e-8;
    int                   width        = this->widget->renderWindow()->GetSize()[0];
    int                   height       = this->widget->renderWindow()->GetSize()[1];
    double                MBPerFile    = (width * height) * MB_PER_PIXEL;

    ExportAnimationDialog dlg(parent);

    dlg.startIndex          = startIndex;
    dlg.endIndex            = endIndex;
    dlg.outputFolder        = outputFolder;
    dlg.defaultOutputFolder = GetDocument()->defaultDir().absolutePath();
    dlg.filePrefix          = filePrefix;
    dlg.fileStartNumber     = fileStartNumber;
    dlg.rotate              = rotate;
    dlg.elevate             = elevate;
    dlg.MBPerFile           = MBPerFile;
    dlg.timePointLabels     = GetDocument()->timePointLabels();
    dlg.numberOfSteps       = numberOfSteps;
    dlg.animationType       = animationType;

    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    startIndex      = dlg.startIndex;
    endIndex        = dlg.endIndex;
    outputFolder    = dlg.outputFolder;
    filePrefix      = dlg.filePrefix.trimmed();
    fileStartNumber = dlg.fileStartNumber.trimmed();
    rotate          = dlg.rotate;
    elevate         = dlg.elevate;
    animationType   = dlg.animationType;
    numberOfSteps   = dlg.numberOfSteps;

    int StartIndex;
    int EndIndex;
    switch (animationType)
    {
    case AnimationType::atTime:
        StartIndex = startIndex;
        EndIndex   = endIndex;
        break;
    case AnimationType::atSpace:
        StartIndex = 0;
        EndIndex   = numberOfSteps - 1;
        break;
    }

    QString path;
    if (!dlg.outputFolder.isEmpty())
    {
        path = dlg.outputFolder + QDir::separator() + filePrefix;
    }
    else
    {
        path = QDir::toNativeSeparators(GetDocument()->defaultDir().absolutePath()) + QDir::separator() + filePrefix;
    }

    if (!dlg.preview)
    {
        // Issue warning for large output (greater than 100 megabytes)
        double diskUsage = (EndIndex - StartIndex + 1) * MBPerFile;
        if (diskUsage > 100)
        {
            QMessageBox::StandardButton reply = 
                QMessageBox::question(parent, "", QString(tr("You are about to write a total of %1 MB to disk. Do you want to continue?")).arg(diskUsage, 0, 'g', 3), QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No)
            {
                return;
            }
        }
    }

    PlaceHeadlightWithCamera();

    vtkLightCollection* lights = renderer->GetLights();
    lights->InitTraversal();
    vtkLight*  light    = lights->GetNextItem();
    vtkLight*  auxlight = lights->GetNextItem();
    vtkCamera* camera   = renderer->GetActiveCamera();
    assert(light == this->headlight);
    assert(auxlight == this->auxiliaryLight);

    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());
    auxlight->SetFocalPoint(camera->GetFocalPoint());

    assert(GetDocument());
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (animationType == AnimationType::atTime)
    {
        GetDocument()->setTimePointTo(startIndex);
    }

    //int fileNumber = atoi((char*)((LPCTSTR)m_FileStartNumber));
    int  fileNumber = atoi(fileStartNumber.toLocal8Bit().data());
    int  len        = fileStartNumber.length();
    int  i          = StartIndex;
    int  fill;

    char b1[10];
    char b2[10];
    QString filename;

    while (1)
    {
        if (!dlg.preview)
        {
            b1[0] = '\0';
            sprintf(b2, "%d", fileNumber);
            fill = len - strlen(b2);
            for (int j = 0; j < fill; j++)
            {
                strcat(b1, "0");
            }
            filename = path + b1 + b2 + ".png";
            WritePng(filename.toLocal8Bit().data(), true);
            fileNumber++;
        }
        i++;
        if (i > EndIndex)
        {
            break;
        }
        camera->Azimuth(rotate);
        camera->Elevation(elevate);

        if (elevate != 0)
        {
            camera->OrthogonalizeViewUp();
        }

        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
        auxlight->SetFocalPoint(camera->GetFocalPoint());

        if (animationType == AnimationType::atTime)
        {
            GetDocument()->advanceOneTimePoint();
        }
        else if (animationType == AnimationType::atSpace)
        {
            GetDocument()->updateAnimation();
        }
    }
    QApplication::restoreOverrideCursor();
}

void MvView::onUpdateCopyDisplay(QAction* action)
{
    action->setEnabled(renderer->VisibleActorCount() > 0 &&
                       !GetDocument()->isAnimating());
}


void MvView::onUpdateResetViewpoint(QAction* action)
{
    action->setEnabled(renderer->VisibleActorCount() > 0 &&
                       !GetDocument()->isAnimating());
}

void MvView::onUpdateViewFrom(QAction* action)
{
    action->setEnabled(renderer->VisibleActorCount() > 0 &&
                       !GetDocument()->isAnimating());
}

void MvView::onViewFromNextDirection()
{
    switch (viewFromDirection)
    {
    case MV_VIEW_FROM_NEGATIVE_X:
        onViewFromNy();
        return;
    case MV_VIEW_FROM_NEGATIVE_Y:
        onViewFromPx();
        return;
    case MV_VIEW_FROM_POSITIVE_X:
        onViewFromPy();
        return;
    case MV_VIEW_FROM_POSITIVE_Y:
        onViewFromNz();
        return;
    case MV_VIEW_FROM_NEGATIVE_Z:
        onViewFromPz();
        return;
    case MV_VIEW_FROM_POSITIVE_Z:
        onViewFromNx();
        return;
    default:
        assert(false);
        break;
    }
}

void MvView::onUpdateSaveViewpoint(QAction* action)
{
    action->setEnabled(renderer->VisibleActorCount() > 0 &&
                       !GetDocument()->isAnimating());
}


void MvView::invalidate(bool erase /* = true */)
{
    //widget->update();
    //widget->repaint();
    widget->renderWindow()->Render();
}

void MvView::onUpdateViewFromNextDirection(QAction* action)
{
    action->setEnabled(renderer->VisibleActorCount() > 0 &&
                       !GetDocument()->isAnimating());
}

void MvView::onViewFromPx()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0] + dist, fp[1], fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_POSITIVE_X;
    invalidate();
}

void MvView::onViewFromNx()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0] - dist, fp[1], fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_NEGATIVE_X;
    invalidate();
}

void MvView::onViewFromPy()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1] + dist, fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_POSITIVE_Y;
    invalidate();
}

void MvView::onViewFromNy()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1] - dist, fp[2]);
    camera->SetViewUp(0, 0, 1);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_NEGATIVE_Y;
    invalidate();
}

void MvView::onViewFromPz()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1], fp[2] + dist);
    camera->SetViewUp(0, 1, 0);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_POSITIVE_Z;
    invalidate();
}


void MvView::onViewFromNz()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    double*    pos    = camera->GetPosition();
    double*    fp     = camera->GetFocalPoint();
    double     dist   = sqrt((pos[0] - fp[0]) * (pos[0] - fp[0]) +
                       (pos[1] - fp[1]) * (pos[1] - fp[1]) + (pos[2] - fp[2]) * (pos[2] - fp[2]));
    camera->SetPosition(fp[0], fp[1], fp[2] - dist);
    camera->SetViewUp(0, -1, 0);
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    viewFromDirection = MV_VIEW_FROM_NEGATIVE_Z;
    invalidate();
}

void MvView::onSaveViewpoint()
{
    vtkCamera* camera     = renderer->GetActiveCamera();
    double*    position   = camera->GetPosition();
    double*    focalPoint = camera->GetFocalPoint();
    double*    viewUp     = camera->GetViewUp();

    for (int i = 0; i < 3; i++)
    {
        savedCameraPosition[i] = position[i];
        savedFocalPoint[i]     = focalPoint[i];
        savedViewUp[i]         = viewUp[i];
    }
    viewpointSaved = true;
}

void MvView::onUpdateRecallViewport(QAction* action)
{
    action->setEnabled(viewpointSaved &&
                       !GetDocument()->isAnimating());
}

void MvView::onRecallViewpoint()
{
    vtkCamera* camera = renderer->GetActiveCamera();
    camera->SetPosition(savedCameraPosition);
    camera->SetFocalPoint(savedFocalPoint);
    camera->SetViewUp(savedViewUp);
    camera->OrthogonalizeViewUp();
    PlaceHeadlightWithCamera();
    renderer->ResetCameraClippingRange();
    invalidate();
}

