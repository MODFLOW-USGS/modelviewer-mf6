#ifndef MVVIEW_H
#define MVVIEW_H

//#include <QObject>
#include "qabstractview.h"
#include <QPointer>

#include <vtkNew.h>

class vtkLight;
class vtkRenderer;
///class QVTKOpenGLNativeWidget; //    vtkMFCWindow;
class vtkGenericOpenGLRenderWindow;
class vtkProp;

class mvGUISettings;
//class MvDoc;

// enum forward declarations
enum class ResolutionType;
enum class SideType;

#include "mvdoc.h"

// should extend vtkViewport  MAYBE INSTEAD EXTEND vtkRenderer?

class MvView : public QAbstractView
{
    Q_OBJECT

//public:
//    // vtkViewport abstract methods
//
//    vtkWindow* GetVTKWindow() override;
//
//    vtkAssemblyPath* PickProp(double selectionX, double selectionY) override { return nullptr; }
//
//    vtkAssemblyPath* PickProp(double selectionX1, double selectionY1, double selectionX2, double selectionY2) override { return nullptr; }

public:
    explicit MvView(QObject *parent = nullptr);
    ~MvView() override;

    MvDoc*                  GetDocument();

    void                    resetExportImageParameters();
    //void                    RotateCamera(double angle);
    //void                    ElevateCamera(double angle);
    //void                    ResetViewpoint() { m_DoResetViewpoint = TRUE; }
    //void                    ResetCameraClippingRange();
    ////////void                    SwitchOnHeadlight(BOOL switchOn);
    ////////void                    SetHeadlightIntensity(double intensity);
    ////////void                    SwitchOnAuxiliaryLight(BOOL switchOn);
    ////////void                    SetAuxiliaryLightIntensity(double intensity);
    //void                    SetAuxiliaryLightPosition(double x, double y, double z);
    //void                    SetBackgroundColor(double red, double green, double blue);
    //void                    GetViewSettings(mvGUISettings* gui);
    //void                    applyViewSettings(mvGUISettings* gui);  // void ApplyViewSettings(mvGUISettings* gui);

    QVTKOpenGLNativeWidget* mainWidget();
    //void                    addActor(vtkProp* p);
    //void                    addViewProp(vtkProp* p);

    void                    onUpdate(QAbstractView* sender, QObject* hint) override;   // CMvView::OnUpdate(CView *pSender, LPARAM lHint, CObject *pHint)



    void                    onFileExportAsBmp(QWidget* parent);

    void                    onFileExportAsPng(QWidget* parent);

    void                    onFileExportAnimation(QWidget* parent);


    void                    onViewFromNextDirection();

    void                    onViewFromNx();
    void                    onViewFromNy();
    void                    onViewFromNz();
    void                    onViewFromPx();
    void                    onViewFromPy();
    void                    onViewFromPz();

    void                    onUpdateCopyDisplay(QAction* action);

    void                    onUpdateResetViewpoint(QAction* action);

    void                    onUpdateViewFromNextDirection(QAction* action);

    void                    onUpdateViewFrom(QAction* action);

    void                    invalidate(bool erase = true);

    void                    onSaveViewpoint();
    void                    onUpdateSaveViewpoint(QAction* action);

    void                    onRecallViewpoint();
    void                    onUpdateRecallViewport(QAction* action);

protected:

    // Paramters relating to camera position
    double         savedCameraPosition[3];
    double         savedFocalPoint[3];
    double         savedViewUp[3];
    int            viewFromDirection;
    bool           doResetViewpoint;


    //void           PlaceHeadlightWithCamera();
    void           WriteBmp(const char* filename, bool useScreenResolution);
    void           WritePng(const char* filename, bool useScreenResolution);


    // Parameters for exporting bitmap
    ResolutionType bitmapResolutionOption;        // m_BitmapResolutionOption
    SideType       bitmapSideOption;              // m_BitmapSideOption
    double         bitmapWidthInInches;           // m_BitmapWidthInInches
    double         bitmapHeightInInches;          // m_BitmapHeightInInches

    // parameters for exporting image sequence
    int            startIndex;
    int            endIndex;
    int            numberOfSteps;
    double         rotate;
    double         elevate;
    AnimationType  animationType;
    QString        outputFolder;
    QString        filePrefix;
    QString        fileStartNumber;

    //int            viewFromDirection;
    //bool           doResetViewpoint;


signals:

private:
    friend class MainWindow;
};

#ifndef _DEBUG // debug version in MvView.cpp
inline MvDoc* MvView::GetDocument()
{
    return dynamic_cast<MvDoc*>(parent());
}
#endif

#endif // MVVIEW_H
