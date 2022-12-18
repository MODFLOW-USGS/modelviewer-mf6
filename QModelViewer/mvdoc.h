#ifndef MVDOC_H
#define MVDOC_H

#include <cstdint>
#include <list>
#include <vector>
#include <string>

//#define USE_THREAD_FOR_ANIMATION

#include <QDir>
#include <QObject>
#include <QString>

#if defined(USE_THREAD_FOR_ANIMATION)
#include <QThread>
#endif

#include <vtkColor.h>
#include <vtkSmartPointer.h>

class mvGUISettings;
class mvManager;
//class mvView;
class QAbstractView;
class QAction;
class QObject;
class QWidget;
class QMainWindow;


// modeless dialogs
class DataDialog;
class ColorBarDialog;
class LightingDialog;
class GridDialog;
class GeometryDialog;
class OverlayDialog;
class SolidDialog;
class IsosurfaceDialog;
class VectorDialog;
class PathlinesDialog;
class ModelFeaturesDialog;
class CropDialog;
class AnimationDialog;


class vtkPropCollection;

// enum forward declarations
enum class MouseMode;
enum class GridType;

enum class ProjectionType {
    ptPerspective,
    ptParallel
};

enum class AnimationType {
    atTime,
    atSpace
};

class MvDoc : public QObject
{
    Q_OBJECT
public:
    explicit MvDoc(QMainWindow* parent);
    ~MvDoc() override;


    MouseMode                          interactorStyle() const;

    const QString& pathName() const;
    //void setPathName(const QString* pathName);


    QString                            activeScalarName() const;
    QString                            modelName() const;

    // Get methods
    //char**                             GetTimePointLabels();
    //int                                GetNumberOfTimePoints();
    QStringList                        timePointLabels();
    void                               getScalarDataRange(double* range);
    //void                               GetVectorMagnitudeRange(double* range);
    //char*                              GetModelName();
    //char*                              GetDataName();
    GridType                           gridType();



    QStringList                        dataTypeLabels();

    void                               setTimePointTo(int timePointIndex);
    void                               updateAnimation();
    void                               advanceOneTimePoint();
    void                               updateAnimationPosition();
    void                               updateAnimationWithSameTime();

    // File menu
    void                               onFileClose();
    void                               onFileNew();
    void                               onFileOpen();

    void                               loadFile(const QString& fileName);
    bool                               saveFile(const QString& fileName);


    // Time Label
    void                               SetTimeLabelFontSize(int size, bool update = true);
    int                                GetTimeLabelFontSize() const;
    void                               SetTimeLabelPosition(double x, double y, bool update = true);
    const double*                      GetTimeLabelPosition() const;

    // Color Bar
    void                               setColorBarEndPoints(double valueBlue, double valueRed);
    void                               UseLinearColorBar();
    void                               UseLogColorBar();
    void                               SetColorBarSize(int width, int height, int offset, bool update = true);
    void                               SetColorBarFontSize(int fontSize, bool update = true);
    void                               SetColorBarNumberOfLabels(int numLabels, bool update = true);
    void                               SetColorBarLabelPrecision(int precision, bool update = true);
    void                               SetColorBarTextColor(double red, double green, double blue, bool update = true);
    void                               SetColorBarColorScheme(int Value);
    int                                GetColorBarWidth();
    int                                GetColorBarHeight();
    int                                GetColorBarOffset();
    int                                GetColorBarFontSize();
    int                                GetColorBarColorScheme();
    std::uint32_t                      GetColorBarFirstCustomColor();
    std::uint32_t                      GetColorBarLastCustomColor();
    void                               SetColorBarFirstCustomColor(std::uint32_t value);
    void                               SetColorBarFirstCustomColor(QColor color);
    void                               SetColorBarLastCustomColor(std::uint32_t value);
    void                               SetColorBarLastCustomColor(QColor color);

    int                                getColorBarSource();
    void                               setColorBarSource(int value);
    double                             getColorBarValueBlue() const;
    double                             getColorBarValueRed() const;

    
    bool                               modified() const;
    bool                               isModified() const;
    void                               setModified(bool modifed);

    bool                               isAnimating() const;

	void                               addView(QAbstractView* pView);
    void                               removeView(QAbstractView* pView);
    void                               updateAllViews(QAbstractView* pSender, QObject* hint = nullptr);

    vtkSmartPointer<vtkPropCollection> propCollection();

    // 
    QString                            currentFile();

    bool                               maybeSave();

    QDir                               defaultDir() const;

    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Data
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxData();
    void                               onUpdateToolboxData(QAction* action);
    void                               setScalarDataTypeTo(int index);

    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Color Bar
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxColorBar();
    void                               onUpdateToolboxColorBar(QAction* action);
    //void                               setScalarDataTypeTo(int index);

    /////////////////////////////////////////////////////////////////////////////
    // Toolbar->Lighting
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxLighting();
    void                               onUpdateToolboxLighting(QAction* action);
    void                               switchOnHeadlight(bool switchOn);
    void                               setHeadlightIntensity(double intensity);
    void                               switchOnAuxiliaryLight(bool switchOn);
    void                               setAuxiliaryLightIntensity(double intensity);
    void                               setAuxiliaryLightPosition(double x, double y, double z);
    void                               setDiffuseLighting(double diffuse);
    void                               setAmbientLighting(double ambient);
    void                               setSpecularLighting(double specular);
    void                               setSpecularPower(double specularPower);
    void                               setBackgroundColor(double red, double green, double blue);


    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Grid
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxGrid();
    void                               onUpdateToolboxGrid(QAction* action);
    void                               setGridLineColor(double red, double green, double blue);
    void                               setGridLineColor(vtkColor3d color3d);
    void                               activateGridLines(int slice, bool b);
    void                               setGridLinePositions(int col, int row, int lay);
    void                               setGridLayerPosition(int layerNumber);
    void                               setGridShellColor(double red, double green, double blue);
    void                               setGridShellOpacity(double opacity);
    void                               applySubgrid(int col_min, int col_max, int row_min, int row_max, int lay_min, int lay_max);
    void                               subgridOff();



    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Geometry
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxGeometry();
    void                               onUpdateToolboxGeometry(QAction* action);

    // Geometry
    void                               setScale(double xScale, double yScale, double zScale);
    void                               setAxesRepresentationToLine();
    void                               setAxesRepresentationToTube();
    void                               setAxesProperties(double xPos, double yPos, double zPos,
                                                         double axesSize, double tubeDiameter);
    void                               setBoundingBoxColor(double red, double green, double blue);
    void                               setBoundingBoxColor(vtkColor3d color3d);
    //void                               ApplySubgrid(int col_min, int col_max, int row_min, int row_max, int lay_min, int lay_max);
    //void                               SubgridOff();

    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Overlay
    /////////////////////////////////////////////////////////////////////////////
    void                               onUpdateToolboxOverlay(QAction* action);
    void                               onToolboxOverlay();
    bool                               hasOverlay();
    void                               applyOverlayControl(const char* filename, int overlayType, double xorig, double yorig,
                                                           double scale, double angle, int drape, int trim, int crop, double elev, double drapeGap);
    void                               removeOverlay();


    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Solid
    /////////////////////////////////////////////////////////////////////////////
    void                               onUpdateToolboxSolid(QAction* action);
    void                               onToolboxSolid();
    void                               applySolidControl(bool threshold, double minValue, double maxValue, int numberOfColorBands);
    void                               setSolidDisplayToBlocky();
    void                               setSolidDisplayToSmooth();
    void                               setSolidDisplayToBanded();


    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Isosurface
    /////////////////////////////////////////////////////////////////////////////
    void                               onUpdateToolboxIsosurface(QAction* action);
    void                               onToolboxIsosurface();
    void                               setCustomIsosurfaces(std::vector<double> values);
    void                               setRegularIsosurfaces(int count, double valueMin, double valueMax);


    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Vector
    /////////////////////////////////////////////////////////////////////////////
    void                               onUpdateToolboxVector(QAction* action);
    void                               onToolboxVector();
    void                               subsampleVectors(int col_min, int col_max, int col_rate, int row_min, int row_max, int row_rate,
                                                        int lay_min, int lay_max, int lay_rate);
    void                               setVectorColor(vtkColor3d color3d);
    void                               activateVectorGlyph(bool b);
    void                               setVectorScaleFactor(double scaleFactor);
    void                               setVectorLineWidth(double width);
    void                               enlargeVectorGlyph();
    void                               shrinkVectorGlyph();

    void                               applyVectorThreshold(double minValue, double maxValue);
    void                               vectorThresholdOff();
    void                               cropVectors(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int cropangle);
    void                               setLogTransformVector(bool b);
    double                             vectorScaleFactor();
    void                               vectorMagnitudeRange(double* range);



    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Pathlines
    /////////////////////////////////////////////////////////////////////////////
    void                               onUpdateToolboxPathlines(QAction* action);       // OnUpdatePathlinesTool(CCmdUI *pCmdUI)
    void                               onToolboxPathlines();                            // OnPathlinesTool

    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Model Features
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxModelFeatures();
    void                               onUpdateToolboxModelFeatures(QAction* action);
    void                               setModelFeatureDisplayOrder(int* displayOrder);
    void                               setModelFeatureDisplayOrder(std::vector<int> displayOrder);
    void                               enlargeModelFeatureGlyphs();
    void                               shrinkModelFeatureGlyphs();
    void                               setModelFeatureColor(const char* modelFeatureName, double* rgba);
    void                               modelFeatureColor(const char* modelFeatureName, double* rgb);
    std::vector<std::string>           modelFeatureLabels();


    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Crop
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxCrop();
    void                               onUpdateToolboxCrop(QAction* action);
    void                               crop(double xMin, double xMax, double yMin, double yMax,
                                            double zMin, double zMax, double cropAngle);
    void                               setCroppedAwayPiecesColor(double red, double green, double blue);
    void                               setCroppedAwayPiecesOpacity(double opacity);
    void                               showCroppedAwayPieces();
    void                               hideCroppedAwayPieces();

    /////////////////////////////////////////////////////////////////////////////
    // Toolbox->Animation
    /////////////////////////////////////////////////////////////////////////////
    void                               onToolboxAnimation();
    void                               onUpdateToolboxAnimation(QAction* action);
    void                               startAnimation();
    void                               stopAnimation();
#if !defined(USE_THREAD_FOR_ANIMATION)
    void                               animate();
#endif
    size_t                             animationSteps() const;
    void                               setAnimationSteps(int value);    
    AnimationType                      animationType() const;
    void                               setAnimationType(AnimationType value);



    // Pathlines
    //void                               SetPathlineRepresentationToLine();
    //void                               SetPathlineRepresentationToTube();
    //void                               SetPathlineTubeDiameter(double diameter);
    //void                               SetPathlineColorBarEndPoints(double valueBlue, double valueRed);
    //void                               SetPathlineTimeClippingMode(int mode);
    //void                               SetPathlineTimeClippingRange(double minTime, double maxTime);
    void                               getPathlineTimeRange(double* range);
    //void                               SetPathlineLogTransform(int Value);
    bool                               hasPathlineData() const;


public slots:

#if defined(USE_THREAD_FOR_ANIMATION)
    void animate();
#endif

    void onShowIsosurfaces();
    void onShowNone();
    void onShowSolid();

    void onShowVectors(QWidget* parent);
    void onShowPathlines();
    void onShowModelFeatures();

    void onShowAxes();
    void onShowGridLines();
    void onShowGridShell();
    void onShowBoundingBox();
    void onShowOverlay();

    void onShowTime();
    void onShowColorBar();

//public:
//
//    //void          CropVectors(double xmin, double xmax,
//    //                          double ymin, double ymax, double zmin, double zmax, int cropangle);
//
//signals:

private:

#if defined(USE_THREAD_FOR_ANIMATION)
    QThread animationThread;
#endif

    friend class MainWindow;

    void                      setCurrentFile(const QString& fileName);


    //QString                  _pathName;              // CDocument::m_strPathName
    QString                   _currentFile;

    bool                      _modified;            // CDocument::m_bModified


    AnimationType            _animationType;        // m_AnimationType
    size_t                   _animationSteps;        // m_AnimationSteps
    MouseMode                _interactorStyle;      // m_InteractorStyle
    ProjectionType           projectionMode;        // m_ProjectionMode
    size_t                   numberOfModels;        // m_NumberOfModels
    bool                     readyToClose;          // m_ReadyToClose
    std::vector<std::string> modelNames;            // m_ModelNames
    std::string              defaultModel;          // m_DefaultModel
    bool                     startup;               // m_Startup
    bool                     _isAnimating;          // m_IsAnimating

    mvGUISettings*           _gui;

    double                   defaultXOrigin;        // m_default_xorigin
    double                   defaultYOrigin;        // m_default_yorigin
    double                   defaultAngRot;         // m_default_angrot

    // The visualization pipeline manager
    mvManager*               _manager;

    std::list<QAbstractView*> _views;

    // Modeless dialog boxes (menu order)
    DataDialog*               dataDialog;           // ID_DATA_TOOL
    // SEPARATOR
    ColorBarDialog*           colorBarDialog;       // ID_COLOR_BAR_TOOL
    LightingDialog*           lightingDialog;       // ID_LIGHTING_TOOL
    // SEPARATOR
    GridDialog*               gridDialog;           // ID_GRID_TOOL
    GeometryDialog*           geometryDialog;       // ID_GEOMETRY_TOOL
    OverlayDialog*            overlayDialog;        // ID_OVERLAY_TOOL
    // SEPARATOR
    SolidDialog*              solidDialog;          // IDD_SOLID
    IsosurfaceDialog*         isosurfaceDialog;     // ID_ISOSURFACE_TOOL
    VectorDialog*             vectorDialog;         // ID_VECTOR_TOOL
    PathlinesDialog*          pathlinesDialog;      // ID_PATHLINES_TOOL
    ModelFeaturesDialog*      modelFeaturesDialog;  // ID_MODEL_FEATURES_TOOL
    // SEPARATOR
    CropDialog*               cropDialog;           // ID_CROP_TOOL
    AnimationDialog*          animationDialog;      // ID_ANIMATION_TOOL


    // Protected methods;
    void                      loadPreviousAppSettings();
    void                      saveCurrentAppSettings();
    
    void                      reinitializeToolDialogs();

    ///////////////////////////////////////////////////////////////////////////
    // updateToolDialogs

    void                      updateToolDialogs(mvGUISettings* gui);

    friend class ColorBarDialog;

    void                      updateSolidDialog();
    void                      updateIsosurfaceDialog();
    void                      updateColorBarDialog();
    void                      updateLightingDialog(mvGUISettings* gui);
    void                      updateGeometryDialog();
    void                      updateGridDialog();
    void                      updateAnimationDialog(mvGUISettings* gui);
    void                      updateVectorDialog();
    void                      updateCropDialog(mvGUISettings* gui);
    void                      updateDataDialog();
    void                      updatePathlinesDialog();
    void                      updateModelFeaturesDialog();
    void                      updateOverlayDialog();


    void                      onParallelProjection();
    void                      onUpdateParallelProjection(QAction* action);
    void                      onPerspectiveProjection();
    void                      onUpdatePerspectiveProjection(QAction* action);

    void                      onPreferences();
};


#endif // MVDOC_H
