#ifndef QABSTRACTVIEW_H
#define QABSTRACTVIEW_H


#include <QImage>
#include <QPixmap>
#include <QPointer>
#include <QObject>

//#include <vtkViewport.h>

class mvGUISettings;

class vtkLight;
class vtkProp;
class vtkRenderer;
class QVTKOpenGLNativeWidget; //    vtkMFCWindow;
class QAction;

enum class MouseMode {
    mmTrackball,
    mmJoystick
};

// class QAbstractView : public QObject, public vtkViewport
// or
// class QAbstractView : public QObject, public vtkRenderer
class QAbstractView : public QObject
{
    Q_OBJECT
public:
    explicit QAbstractView(QObject *parent = nullptr);

    ~QAbstractView() override;

    virtual void onUpdate(QAbstractView* pSender, QObject* pHint);

    void         getViewSettings(mvGUISettings* gui);
    void         applyViewSettings(mvGUISettings* gui);

    void         resetViewPoint();

    void         addActor(vtkProp* p);
    void         addViewProp(vtkProp* p);

    void         removeAllViewProps();

    void         setInteractorStyle(MouseMode mouseMode);

    void         setProjectionToPerspective();
    void         setProjectionToParallel();

    void         switchOnHeadlight(bool switchOn);
    void         setHeadlightIntensity(double intensity);
    void         switchOnAuxiliaryLight(bool switchOn);
    void         setAuxiliaryLightIntensity(double intensity);
    void         setAuxiliaryLightPosition(double x, double y, double z);

    void         resetCameraClippingRange();
    void         discardSavedViewpoint();

    void         setBackgroundColor(double red, double green, double blue);

    void         rotateCamera(double angle);
    void         elevateCamera(double angle);

    void         copySnapshotToClipboard();
    QImage       getImage();
    QPixmap      snapshot();

    void         resetCamera();

signals:

protected:
    int                              headlightOn;
    int                              auxiliaryLightOn;
    double                           headlightIntensity;
    double                           auxiliaryLightIntensity;

    // Paramters relating to camera position
    bool                             viewpointSaved;

    void         PlaceHeadlightWithCamera();

    vtkRenderer* renderer;
    vtkLight*    headlight;
    vtkLight*    auxiliaryLight;

    QPointer<QVTKOpenGLNativeWidget> widget;
};

#endif // QABSTRACTVIEW_H
