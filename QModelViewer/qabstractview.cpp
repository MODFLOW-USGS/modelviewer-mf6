#include "qabstractview.h"

#include "mvGUISettings.h"

#include <vtkCamera.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkUnsignedCharArray.h>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QVTKOpenGLNativeWidget.h>

#include <cassert>

QAbstractView::QAbstractView(QObject *parent)
    : QObject{parent}
    , viewpointSaved{false}
{
    renderer = vtkRenderer::New();
    renderer->SetBackground(1, 1, 1);

#ifdef NDEBUG
    renderer->GlobalWarningDisplayOff();
#else
    //renderer->SetDebug(true);
#endif

    widget    = new QVTKOpenGLNativeWidget();
    widget->renderWindow()->AddRenderer(renderer);

    // default style is not vtkInteractorStyleSwitch
    widget->interactor()->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleSwitch>::New());

    headlight = vtkLight::New();
    headlight->SetLightTypeToHeadlight();

    auxiliaryLight = vtkLight::New();
    auxiliaryLight->SetIntensity(0);

    renderer->AddLight(headlight);
    renderer->AddLight(auxiliaryLight);
}

QAbstractView::~QAbstractView()
{
    renderer->SetRenderWindow(nullptr);
    renderer->RemoveLight(headlight);
    renderer->RemoveLight(auxiliaryLight);

    renderer->Delete();
    headlight->Delete();
    auxiliaryLight->Delete();

    delete widget;
}


void QAbstractView::onUpdate(QAbstractView* sender, QObject* /*hint*/)
{
    assert(sender != this);
    widget->update();
}

void QAbstractView::getViewSettings(mvGUISettings* gui)
{
    /* void CMvView::GetViewSettings(mvGUISettings *gui)
    * 
    m_Renderer->GetBackground(gui->background);
    m_AuxiliaryLight->GetPosition(gui->auxiliaryLightDirection);
    vtkCamera* camera = m_Renderer->GetActiveCamera();
    camera->GetPosition(gui->cameraPosition);
    camera->GetFocalPoint(gui->focalPoint);
    camera->GetViewUp(gui->viewUp);
    gui->parallelProjection = camera->GetParallelProjection();
    gui->parallelScale      = camera->GetParallelScale();
    */
    this->renderer->GetBackground(gui->background);
    this->auxiliaryLight->GetPosition(gui->auxiliaryLightDirection);
    vtkCamera* camera = this->renderer->GetActiveCamera();
    camera->GetPosition(gui->cameraPosition);
    camera->GetFocalPoint(gui->focalPoint);
    camera->GetViewUp(gui->viewUp);
    gui->parallelProjection = camera->GetParallelProjection();
    gui->parallelScale      = camera->GetParallelScale();
}

void QAbstractView::applyViewSettings(mvGUISettings* gui)
{
    vtkCamera* camera = this->renderer->GetActiveCamera();
    camera->SetPosition(gui->cameraPosition);
    camera->SetFocalPoint(gui->focalPoint);
    camera->SetViewUp(gui->viewUp);
    camera->OrthogonalizeViewUp();
    camera->SetParallelProjection(gui->parallelProjection);
    camera->SetParallelScale(gui->parallelScale);

    PlaceHeadlightWithCamera();

    setHeadlightIntensity(gui->headlightIntensity);
    setAuxiliaryLightIntensity(gui->auxiliaryLightIntensity);
    auxiliaryLight->SetPosition(gui->auxiliaryLightDirection);
    switchOnHeadlight(gui->headlightOn);
    switchOnAuxiliaryLight(gui->auxiliaryLightOn);


    this->renderer->ResetCameraClippingRange();
    if (gui->customBackground)
    {
        this->renderer->SetBackground(gui->background);
    }
    else
    {
        this->renderer->SetBackground(1, 1, 1);
    }
}

void QAbstractView::resetViewPoint()
{
    // if (!this->renderer->GetViewProps()->GetNumberOfItems()) return;
    this->renderer->ResetCamera();
    this->renderer->ResetCameraClippingRange();
}

void QAbstractView::resetCameraClippingRange()
{
    this->renderer->ResetCameraClippingRange();
}

/////////////////////////////////////////////////////////////////////////////
// Protected methods

void QAbstractView::PlaceHeadlightWithCamera()
{
    vtkCamera*          camera = this->renderer->GetActiveCamera();
    vtkLightCollection* lights = this->renderer->GetLights();
    lights->InitTraversal();
    vtkLight* light = lights->GetNextItem();
    assert(light == this->headlight);
    light->SetPosition(camera->GetPosition());
    light->SetFocalPoint(camera->GetFocalPoint());
}

void QAbstractView::addActor(vtkProp* p)
{
    renderer->AddActor(p);
}

void QAbstractView::addViewProp(vtkProp* p)
{
    renderer->AddViewProp(p);
}

void QAbstractView::removeAllViewProps()
{
    renderer->RemoveAllViewProps();
}

void QAbstractView::setProjectionToPerspective()
{
    renderer->GetActiveCamera()->ParallelProjectionOff();
}

void QAbstractView::setProjectionToParallel()
{
    renderer->GetActiveCamera()->ParallelProjectionOn();
}

void QAbstractView::switchOnHeadlight(bool switchOn)
{
    if (switchOn)
    {
        headlight->SetIntensity(headlightIntensity);
    }
    else
    {
        headlightIntensity = headlight->GetIntensity();
        headlight->SetIntensity(0);
    }
}

void QAbstractView::setHeadlightIntensity(double intensity)
{
    headlight->SetIntensity(intensity);
    headlightIntensity = intensity;
}

void QAbstractView::switchOnAuxiliaryLight(bool switchOn)
{
    if (switchOn)
    {
        auxiliaryLight->SetIntensity(auxiliaryLightIntensity);
    }
    else
    {
        auxiliaryLightIntensity = auxiliaryLight->GetIntensity();
        auxiliaryLight->SetIntensity(0);
    }
}

void QAbstractView::setAuxiliaryLightIntensity(double intensity)
{
    auxiliaryLight->SetIntensity(intensity);
    auxiliaryLightIntensity = intensity;
}

void QAbstractView::setAuxiliaryLightPosition(double x, double y, double z)
{
    auxiliaryLight->SetPosition(x, y, z);
}

void QAbstractView::setInteractorStyle(MouseMode mouseMode)
{
    // This assumes that the interactor style is an vtkInteractorStyleSwitch (see ctor)
    vtkInteractorStyleSwitch* iss = vtkInteractorStyleSwitch::SafeDownCast(widget->interactor()->GetInteractorStyle());
    if (iss)
    {
        if (mouseMode == MouseMode::mmJoystick)
        {
            iss->SetCurrentStyleToJoystickCamera();
        }
        else
        {
            iss->SetCurrentStyleToTrackballCamera();
        }
    }
}

void QAbstractView::discardSavedViewpoint()
{
    ////doResetViewpoint = false; @todo check
    viewpointSaved = false;
}

void QAbstractView::setBackgroundColor(double red, double green, double blue)
{
    renderer->SetBackground(red, green, blue);
}

void QAbstractView::rotateCamera(double angle)
{
    if (angle != 0)
    {
        vtkLightCollection* lights = renderer->GetLights();
        lights->InitTraversal();
        vtkLight*  light  = lights->GetNextItem();
        assert(light == this->headlight);
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->Azimuth(angle);
        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
    }
}

void QAbstractView::elevateCamera(double angle)
{
    if (angle != 0)
    {
        vtkLightCollection* lights = renderer->GetLights();
        lights->InitTraversal();
        vtkLight*  light  = lights->GetNextItem();
        assert(light == this->headlight);
        vtkCamera* camera = renderer->GetActiveCamera();
        camera->Elevation(angle);
        camera->OrthogonalizeViewUp();
        light->SetPosition(camera->GetPosition());
        light->SetFocalPoint(camera->GetFocalPoint());
    }
}

QImage QAbstractView::getImage()
{
    vtkRenderWindow* rw = widget->renderWindow();
    //	rw->OffScreenRenderingOn();
    rw->Render();

    int*                  vtk_size = rw->GetSize();
    int                   w        = *(vtk_size);
    int                   h        = *(vtk_size + 1);

    QImage                img(w, h, QImage::Format_RGB32);
    vtkUnsignedCharArray* pixels = vtkUnsignedCharArray::New();
    pixels->SetArray(img.bits(), w * h * 4, 1);

    rw->GetRGBACharPixelData(0, 0, w - 1, h - 1, 1, pixels);
    pixels->Delete();
    img = img.rgbSwapped();
    img = img.mirrored();

    //	rw->OffScreenRenderingOff();
    return img;
}

void QAbstractView::copySnapshotToClipboard()
{
    QPixmap     pixmap    = snapshot();
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

QPixmap QAbstractView::snapshot()
{
    QImage                    img    = getImage();
    QPixmap                   pixmap = QPixmap::fromImage(img);
    pixmap.setDevicePixelRatio(widget->devicePixelRatioF());
    return pixmap;
}

void QAbstractView::resetCamera()
{
    renderer->ResetCamera();
    onUpdate(nullptr, nullptr);
}
