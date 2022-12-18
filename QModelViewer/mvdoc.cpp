#include "mvdoc.h"

#include <algorithm>

#if defined(Q_OS_WIN) && !defined(NDEBUG)
#include <windows.h>        // for testing RGB macro
#endif

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QThread>


#include <vtkPropCollection.h>

#include "mainwindow.h"
#include "datafilesdialog.h"

#include "Modflow6DataSource.h"

#include "mvview.h"

#include "mvGUISettings.h"
#include "mvManager.h"

#include "colorbardialog.h"
#include "datadialog.h"
#include "dataselectiondialog.h"
#include "geometrydialog.h"
#include "griddialog.h"
#include "lightingdialog.h"
#include "overlaydialog.h"
#include "soliddialog.h"
#include "isosurfacedialog.h"
#include "vectordialog.h"
#include "modelfeaturesdialog.h"
#include "cropdialog.h"
#include "animationdialog.h"


#include "preferencesdialog.h"


MvDoc::MvDoc(QMainWindow* parent)
    : QObject{parent}
    , _modified{false}
    , _interactorStyle{MouseMode::mmTrackball}
    , numberOfModels{1}
    , _isAnimating{false}
    , _gui{0}
{
    // Initialize variables
    startup         = true;
    projectionMode  = ProjectionType::ptPerspective;
    readyToClose    = false;
    _animationType  = AnimationType::atTime;
    _animationSteps = 10;

    defaultXOrigin  = 0.;
    defaultYOrigin  = 0.;
    defaultAngRot   = 0.;

    QSettings settings;
    _interactorStyle = static_cast<enum MouseMode>(settings.value("interactorStyle", (int)MouseMode::mmTrackball).toInt());

#if TODO
    // Get the models supported by this application
    m_NumberOfModels = mvModelList::GetNumberOfModels();
    m_ModelNames     = new char*[m_NumberOfModels];
    for (int i = 0; i < m_NumberOfModels; i++)
    {
        m_ModelNames[i] = new char[30];
    }
    mvModelList::GetModelNames(m_ModelNames);

    // Load presistent app settings from Windows registry
    LoadPreviousAppSettings();
#endif

    // Create the visualization pipeline manager
    _manager = new mvManager;

    dataDialog          = new DataDialog(parent, this);
    colorBarDialog      = new ColorBarDialog(parent, this);
    lightingDialog      = new LightingDialog(parent, this);
    gridDialog          = new GridDialog(parent, this);
    geometryDialog      = new GeometryDialog(parent, this);
    overlayDialog       = new OverlayDialog(parent, this);
    solidDialog         = new SolidDialog(parent, this);
    isosurfaceDialog    = new IsosurfaceDialog(parent, this);
    vectorDialog        = new VectorDialog(parent, this);
    modelFeaturesDialog = new ModelFeaturesDialog(parent, this);
    cropDialog          = new CropDialog(parent, this);
    animationDialog     = new AnimationDialog(parent, this);

    reinitializeToolDialogs();

    // Note: views have not been created yet they depend on MvDoc
}

MvDoc::~MvDoc()
{
    // Save persistent app settings to Windows registry
    saveCurrentAppSettings();

    // Free memory
    delete _manager;
    delete _gui;

    /***
    for (int i = 0; i < m_NumberOfModels; i++)
    {
        delete[] m_ModelNames[i];
    }
    delete[] m_ModelNames;
***/
    // Note that modeless dialog boxes delete themselves.

#if defined(USE_THREAD_FOR_ANIMATION)
    qDebug() << "thread dtor start";
    animationThread.quit();
    animationThread.wait();
    qDebug() << "thread dtor finish";
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Load and save application settings

void MvDoc::loadPreviousAppSettings()
{
#if TODO
    CString directory = AfxGetApp()->GetProfileString("Settings", "Directory");
    m_InteractorStyle = AfxGetApp()->GetProfileInt("Settings", "Interactor Style", 0);
    m_DefaultModel    = AfxGetApp()->GetProfileString("Settings", "Default Model");
    _chdir(directory.GetBuffer(10));
#endif
}

void MvDoc::saveCurrentAppSettings()
{
#if TODO
    if (m_strPathName.GetLength() > 0)
    {
        int count = m_strPathName.ReverseFind('\\');
        if (m_strPathName.GetAt(count - 1) == ':')
        {
            count++;
        }
        CString directory = m_strPathName.Left(count);

        AfxGetApp()->WriteProfileString("Settings", "Directory",
                                        (LPCTSTR)directory);
    }
    AfxGetApp()->WriteProfileInt("Settings", "Interactor Style", m_InteractorStyle);
    AfxGetApp()->WriteProfileString("Settings", "Default Model", (LPCTSTR)m_DefaultModel);
#endif
}

//const QString& MvDoc::pathName() const
//{
//    return _pathName;
//}

MouseMode MvDoc::interactorStyle() const
{
    return _interactorStyle;
}

QString MvDoc::modelName() const
{
    return _manager->GetModelName();
}

QString MvDoc::activeScalarName() const
{
    return _manager->GetActiveScalarDataName();
}

vtkSmartPointer<vtkPropCollection> MvDoc::propCollection()
{
    return _manager->GetPropCollection();
}

bool MvDoc::isAnimating() const
{
    return _isAnimating;
}

QStringList MvDoc::timePointLabels()
{
    QStringList          slist;
    int                  n      = _manager->GetNumberOfTimePoints();
    char**               labels = _manager->GetTimePointLabels();
    for (int i = 0; i < n; ++i)
    {
        slist.append(labels[i]);
    }
    return slist;
}

QStringList MvDoc::dataTypeLabels()
{
    QStringList slist;
    char**      labels = _manager->GetDataTypeLabels();
    int         n      = _manager->GetNumberOfScalarDataTypes();
    for (int i = 0; i < n; ++i)
    {
        slist.append(labels[i]);
    }
    return slist;
}

/////////////////////////////////////////////////////////////////////////////
// File->

void MvDoc::onFileClose()
{
    if (!maybeSave())
    {
        return;
    }

    delete _manager;
    delete _gui;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    _manager = new mvManager();
    _gui     = new mvGUISettings();

    for (auto view : _views)
    {
        //view->renderer->GetViewProps()->RemoveAllItems();
        view->removeAllViewProps();
        vtkSmartPointer<vtkPropCollection> props = _manager->GetPropCollection();
        props->InitTraversal();
        for (int i = 0; i < props->GetNumberOfItems(); i++)
        {
            view->addViewProp(props->GetNextProp());
        }
        view->discardSavedViewpoint();
        view->applyViewSettings(_gui);
    }

    reinitializeToolDialogs();
    setCurrentFile("");

    QApplication::restoreOverrideCursor();
}

void MvDoc::reinitializeToolDialogs()
{
    dataDialog->reinitialize();
    colorBarDialog->reinitialize();
    lightingDialog->reinitialize();
    gridDialog->reinitialize();
    geometryDialog->reinitialize();
    solidDialog->reinitialize();
    isosurfaceDialog->reinitialize();
    vectorDialog->reinitialize();
    vectorDialog->hide();
    //m_PathlinesDlg->Reinitialize();
    //m_PathlinesDlg->ShowWindow(SW_HIDE);
    modelFeaturesDialog->reinitialize();
    modelFeaturesDialog->hide();
    overlayDialog->reinitialize();
    cropDialog->reinitialize();
    animationDialog->reinitialize();
}

void MvDoc::onFileNew()
{
    MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    assert(mainWindow);

    delete _manager;
    _manager = new mvManager;

    reinitializeToolDialogs();
    setCurrentFile("");


    char selectedModel[20];
    // This version of Model View is customized to display only Modflow 6 results.
    // In other words, the variable "selectedModel" is set to "Modflow 6"
    strcpy(selectedModel, Modflow6DataSource::GetNameStatic());

    // Display dialog box for user to specify data files for the
    // selected model. Note that memory for dataFileList is allocated
    // by the dataFileList method
    char* dataFileList = DataFilesDialog::GetDataFileList(selectedModel, mainWindow);
    if (dataFileList == nullptr)
    {
        return; // User clicked the Cancel button
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Load data
    const char* errorMsg = _manager->LoadData(selectedModel, dataFileList);
    delete[] dataFileList;

    // Check for error in reading data files
    if (errorMsg)
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(mainWindow, "", errorMsg);
        return;
    }

    // Display dialog box for user to select time and type of data to view
    QApplication::restoreOverrideCursor();

    DataSelectionDialog dlg(timePointLabels(), dataTypeLabels(), mainWindow);
    dlg.exec();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    _manager->SetTimePointTo(dlg.selectedTimePoint);
    _manager->SetScalarDataTypeTo(dlg.selectedDataType);
    Q_ASSERT(_manager->GetTimeLabelOption() == 0);      // Modflow6DataSource doesn't override GetTimeLabelOption

    mainWindow->updateStatusBar();

    // Apply default settings and then turn on bounding box
    _manager->ApplyDefaultSettings();

    if (const Modflow6DataSource* modflow6 = dynamic_cast<const Modflow6DataSource*>(_manager->GetDataSource()))
    {
        defaultXOrigin = modflow6->GetXOrigin();
        defaultYOrigin = modflow6->GetYOrigin();
        defaultAngRot  = modflow6->GetAngRot();

        _manager->SetOverlayCoordinatesAtGridOrigin(modflow6->GetXOrigin(), modflow6->GetYOrigin());
        _manager->SetOverlayAngle(modflow6->GetAngRot());
    }

    // If there are more than one time points, turn on the
    // time label
    if (_manager->GetNumberOfTimePoints() > 1)
    {
        _manager->ShowTimeLabel();
    }

    // Create a gui settings object with initial settings. These
    // are used to update the tool dialog boxes and view.
    mvGUISettings* gui = new mvGUISettings;

    // Update the tool (modeless) dialog boxes
    updateToolDialogs(gui);

    for (auto view : _views)
    {
        view->removeAllViewProps();
        vtkSmartPointer<vtkPropCollection> props = _manager->GetPropCollection();
        props->InitTraversal();
        for (int i = 0; i < props->GetNumberOfItems(); i++)
        {
            view->addViewProp(props->GetNextProp());
        }
        view->setProjectionToPerspective();
        view->discardSavedViewpoint();
        view->applyViewSettings(gui);
        view->resetViewPoint();
        view->onUpdate(nullptr, nullptr);
    }
    //delete gui;
    _gui = gui;

    // Mark this document as modified
    setModified(true);

    // Redraw the view;
    //updateAllViews(nullptr);

    QApplication::restoreOverrideCursor();
}

void MvDoc::onFileOpen()
{
    MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    assert(mainWindow);

    if (!maybeSave()) return;

    QString fileName = QFileDialog::getOpenFileName(mainWindow, tr("Open"), QString(), tr("MvMf6 Files (*.mvmf6)"));

    if (fileName.isEmpty())
    {
        return;
    }

    loadFile(fileName);
}

void MvDoc::loadFile(const QString& fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString native = QDir::toNativeSeparators(fileName);
    int     pos    = native.lastIndexOf(QDir::separator());
    if (pos != -1)
    {
        QDir::setCurrent(native.left(pos));
    }

    MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    assert(mainWindow);

    delete _manager;
    delete _gui;

    std::string errorMsg;
    _gui     = new mvGUISettings();
    _manager = new mvManager();
    _manager->Deserialize(QDir::toNativeSeparators(fileName).toLocal8Bit().data(), _gui, errorMsg);
    if (errorMsg.size())
    {
        QApplication::restoreOverrideCursor();
        QMessageBox::information(mainWindow, tr("Error"), errorMsg.c_str());
        return;
    }

    for (auto view : _views)
    {
        view->removeAllViewProps();
        vtkSmartPointer<vtkPropCollection> props = _manager->GetPropCollection();
        props->InitTraversal();
        for (int i = 0; i < props->GetNumberOfItems(); i++)
        {
            view->addViewProp(props->GetNextProp());
        }
        view->discardSavedViewpoint();
        view->applyViewSettings(_gui);
        view->onUpdate(nullptr, nullptr);
    }

    updateToolDialogs(_gui);
    // mainWindow->updateActions();
    mainWindow->updateStatusBar();
    setCurrentFile(fileName);

    QApplication::restoreOverrideCursor();
}

// BOOL CMvDoc::OnSaveDocument(LPCTSTR lpszPathName)
bool MvDoc::saveFile(const QString& fileName)
{
    assert(_gui);
    mvGUISettings settings(*_gui);

    // crop settings
    cropDialog->updateDataControls(true);
    settings.cropBoundsXDelta = cropDialog->mXDelta;
    settings.cropBoundsYDelta = cropDialog->mYDelta;
    settings.cropBoundsZDelta = cropDialog->mZDelta;

    // animation settings
    animationDialog->updateDataOptions(true);
    settings.animationRotate  = animationDialog->rotate;
    settings.animationElevate = animationDialog->elevate;
    settings.animationDelay   = animationDialog->delay;

    // lighting settings
    lightingDialog->updateDataLights(true);
    settings.headlightOn             = lightingDialog->headlightOn ? 1 : 0;
    settings.auxiliaryLightOn        = lightingDialog->auxiliaryLightOn ? 1 : 0;
    settings.headlightIntensity      = lightingDialog->headlightIntensity * 0.01;
    settings.auxiliaryLightIntensity = lightingDialog->auxiliaryLightIntensity * 0.01;
    lightingDialog->updateDataBackground(true);
    settings.customBackground = lightingDialog->customBackground ? 1 : 0;

    _views.front()->getViewSettings(&settings);

    // The visualization pipeline doc->_manager will serialize everything along
    // with the gui settings
    char *errorMsg = _manager->Serialize(fileName.toLocal8Bit().data(), &settings);
    if (errorMsg)
    {
        QWidget* widget = dynamic_cast<QWidget*>(parent());
        assert(widget);
        QMessageBox::information(widget, "", tr(errorMsg));
        return false;
    }
    setCurrentFile(fileName);
    setModified(false);
    return true;
}

bool MvDoc::maybeSave()
{
    if (!this->isModified())
        return true;

    QString shownName = this->_currentFile;
    if (shownName.isEmpty())
    {
        shownName = tr("Untitled");
    }
    else
    {
        QFileInfo info(shownName);
        shownName = info.fileName();
    }

    MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    assert(mainWindow);
    const QMessageBox::StandardButton ret = QMessageBox::question(mainWindow,
                                                                  tr("Model Viewer for Modflow 6"),
                                                                  tr("Save changes to %1?").arg(shownName),
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Yes:
        return mainWindow->onFileSave();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MvDoc::onPreferences()
{
    MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    assert(mainWindow);

    PreferencesDialog dlg(_interactorStyle, mainWindow);
    if (dlg.exec() != QDialog::Accepted)
    {
        return;
    }

    _interactorStyle = dlg.interactorStyle;
    for (auto view : _views)
    {
        view->setInteractorStyle(_interactorStyle);
    }

    QSettings settings;
    settings.setValue("interactorStyle", (int)_interactorStyle);
}

/////////////////////////////////////////////////////////////////////////////
// Time Label

void MvDoc::SetTimeLabelFontSize(int size, bool update /* = true */)
{
    _manager->SetTimeLabelFontSize(size);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

int MvDoc::GetTimeLabelFontSize() const
{
    return _manager->GetTimeLabelFontSize();
}

void MvDoc::SetTimeLabelPosition(double x, double y, bool update /* = true */)
{
    _manager->SetTimeLabelPosition(x, y);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

const double* MvDoc::GetTimeLabelPosition() const
{
    return _manager->GetTimeLabelPosition();
}


/////////////////////////////////////////////////////////////////////////////
// Color Bar

void MvDoc::setColorBarEndPoints(double valueBlue, double valueRed)
{
    _manager->SetColorBarEndPoints(valueBlue, valueRed);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::UseLinearColorBar()
{
    _manager->UseLinearColorBar();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::UseLogColorBar()
{
    _manager->UseLogColorBar();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::SetColorBarSize(int width, int height, int offset, bool update /*= true*/)
{
    _manager->SetColorBarWidth(width);
    _manager->SetColorBarHeight(height);
    _manager->SetColorBarOffset(offset);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

void MvDoc::SetColorBarFontSize(int fontSize, bool update /*= true*/)
{
    _manager->SetColorBarFontSize(fontSize);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

void MvDoc::SetColorBarNumberOfLabels(int numLabels, bool update /*= true*/)
{
    _manager->SetColorBarNumberOfLabels(numLabels);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

void MvDoc::SetColorBarLabelPrecision(int precision, bool update /*= true*/)
{
    _manager->SetColorBarLabelPrecision(precision);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

void MvDoc::SetColorBarTextColor(double red, double green, double blue, bool update /*= true*/)
{
    _manager->SetColorBarTextColor(red, green, blue);
    if (update)
    {
        updateAllViews(nullptr);
        setModified(true);
    }
}

void MvDoc::SetColorBarColorScheme(int Value)
{
    _manager->SetColorBarColorScheme(Value);
    updateAllViews(nullptr);
    setModified(true);
}

int MvDoc::GetColorBarWidth()
{
    return _manager->GetColorBarWidth();
}

int MvDoc::GetColorBarHeight()
{
    return _manager->GetColorBarHeight();
}

int MvDoc::GetColorBarOffset()
{
    return _manager->GetColorBarOffset();
}

int MvDoc::GetColorBarFontSize()
{
    return _manager->GetColorBarFontSize();
}

int MvDoc::GetColorBarColorScheme()
{
    return _manager->GetColorBarColorScheme();
}

std::uint32_t MvDoc::GetColorBarFirstCustomColor()
{
    return _manager->GetColorBarFirstCustomColor();
}

std::uint32_t MvDoc::GetColorBarLastCustomColor()
{
    return _manager->GetColorBarLastCustomColor();
}

void MvDoc::SetColorBarFirstCustomColor(std::uint32_t value)
{
    _manager->SetColorBarFirstCustomColor(value);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::SetColorBarFirstCustomColor(QColor color)
{
#if defined(Q_OS_WIN) && !defined(NDEBUG)
    assert(RGB(color.red(), color.green(), color.blue()) == qRgba(color.red(), color.green(), color.blue(), 0));
#endif
    SetColorBarFirstCustomColor(qRgba(color.red(), color.green(), color.blue(), 0));    
}

void MvDoc::SetColorBarLastCustomColor(std::uint32_t value)
{
    _manager->SetColorBarLastCustomColor(value);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::SetColorBarLastCustomColor(QColor color)
{
#if defined(Q_OS_WIN) && !defined(NDEBUG)
    assert(RGB(color.red(), color.green(), color.blue()) == qRgba(color.red(), color.green(), color.blue(), 0));
#endif
    SetColorBarLastCustomColor(qRgba(color.red(), color.green(), color.blue(), 0));
}

int MvDoc::getColorBarSource()
{
    return _manager->GetColorBarSource();
}

void MvDoc::setColorBarSource(int value)
{
    _manager->SetColorBarSource(value);
}

double MvDoc::getColorBarValueBlue() const
{
    return _manager->GetColorBarValueBlue();
}

double MvDoc::getColorBarValueRed() const
{
    return _manager->GetColorBarValueRed();
}

bool MvDoc::modified() const
{
    return _modified;
}

bool MvDoc::isModified() const
{
    return _modified;
}

void MvDoc::setModified(bool modified)
{
    _modified = modified;
}

void MvDoc::addView(QAbstractView* view)
{
    assert(view);
    auto it = std::find(_views.begin(), _views.end(), view);
    assert(it == _views.end());
    _views.push_back(view);
}

void MvDoc::removeView(QAbstractView* view)
{
    assert(view);
    auto it = std::find(_views.begin(), _views.end(), view);
    assert(it != _views.end());
    _views.remove(view);
}

void MvDoc::updateAllViews(QAbstractView* sender, QObject* hint)
{
    for (auto& view : _views)
    {
        if (view != sender) view->onUpdate(sender, hint);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Show menu @todo Reorder in menu order

void MvDoc::onShowNone()
{
    _manager->HideScalarData();
    isosurfaceDialog->activate(false);
    solidDialog->activate(false);
    cropDialog->activate(false);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowSolid()
{
    _manager->ShowScalarDataAsSolid();
    isosurfaceDialog->activate(false);
    solidDialog->activate(true);
    cropDialog->activate(true);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowIsosurfaces()
{
    _manager->ShowScalarDataAsIsosurfaces();
    isosurfaceDialog->activate(true);
    if (_manager->UsingRegularIsosurfaces())
    {
        isosurfaceDialog->setCurrentTabIndex(0);
    }
    else
    {
        isosurfaceDialog->setCurrentTabIndex(1);
    }
    solidDialog->activate(false);
    cropDialog->activate(true);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowVectors(QWidget* parent)
{
    if (_manager->AreVectorsVisible())
    {
        _manager->HideVectors();
        vectorDialog->activate(false);
    }
    else
    {
        if (_manager->GetVectorScaleFactor() == 0)
        {
            QMessageBox::warning(parent, tr("Warning"), tr("Vector scale factor is currently set to zero!"));
        }
        _manager->ShowVectors();
        vectorDialog->activate(true);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowPathlines()
{
    if (_manager->ArePathlinesVisible())
    {
        _manager->HidePathlines();
        //m_PathlinesDlg->Activate(FALSE);  @todo PATHLINES
    }
    else
    {
        _manager->ShowPathlines();
        //m_PathlinesDlg->Activate(TRUE);   @todo PATHLINES
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowModelFeatures()
{
    if (_manager->AreModelFeaturesVisible())
    {
        _manager->HideModelFeatures();
        modelFeaturesDialog->activate(false);
    }
    else
    {
        _manager->ShowModelFeatures();
        modelFeaturesDialog->activate(true);
        if ((!modelFeaturesDialog->isVisible()) &&
            (modelFeaturesDialog->showFeaturesCount() == 0))
        {
            modelFeaturesDialog->show();
        }
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setCurrentFile(const QString& fileName)
{
    _currentFile = QDir::toNativeSeparators(fileName);
    setModified(false);

    //MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
    MainWindow* mainWindow = qobject_cast<MainWindow*>(parent());
    assert(mainWindow);
    if (mainWindow)
    {
        if (!fileName.isEmpty())
        {
            // default settings use QCoreApplication::organizationName() and QCoreApplication::applicationName()
            QSettings   settings;
            QStringList files = settings.value(MainWindow::recentFilesKey).toStringList();
            files.removeAll(fileName);
            files.prepend(fileName);
            while (files.size() > MainWindow::MaxRecentFiles)
                files.removeLast();

            settings.setValue(MainWindow::recentFilesKey, files);
            mainWindow->updateRecentFileActions();
        }

        mainWindow->setWindowModified(false);

        QString shownName = _currentFile;
        if (shownName.isEmpty())
        {
            shownName = tr("Untitled");
        }
        mainWindow->setWindowFilePath(shownName);
    }
    updateAllViews(nullptr);
}

QString MvDoc::currentFile()
{
    return _currentFile;
}

QDir MvDoc::defaultDir() const
{
    if (!_currentFile.isEmpty())
    {
        QFileInfo fileInfo(_currentFile);
        return fileInfo.absoluteDir();
    }
    return QDir();
}

void MvDoc::onShowAxes()
{
    if (_manager->AreAxesVisible())
    {
        _manager->HideAxes();
        geometryDialog->activateAxes(false);
    }
    else
    {
        _manager->ShowAxes();
        geometryDialog->activateAxes(true);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowGridLines()
{
    if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        if (_manager->AreActivatedGridLinesVisible())
        {
            _manager->HideGridLines();
            gridDialog->activateLines(false);
            gridDialog->enableApplyButton(false);
        }
        else
        {
            _manager->ShowActivatedGridLines();
            gridDialog->activateLines(true);
            gridDialog->enableApplyButton(gridDialog->currentTabIndex() == 0);
            
        }
    }
    else if (_manager->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        if (_manager->IsGridLayerVisible())
        {
            _manager->HideGridLayer();
            gridDialog->activateLines(false);
            gridDialog->enableApplyButton(false);
        }
        else
        {
            _manager->ShowGridLayer();
            gridDialog->activateLines(true);
            gridDialog->enableApplyButton(true);
        }
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowGridShell()
{
    if (_manager->IsGridShellVisible())
    {
        _manager->HideGridShell();
        gridDialog->activateShell(false);
    }
    else
    {
        _manager->ShowGridShell();
        gridDialog->activateShell(true);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowBoundingBox()
{
    if (_manager->IsBoundingBoxVisible())
    {
        _manager->HideBoundingBox();
        geometryDialog->activateBoundingBox(false);
    }
    else
    {
        _manager->ShowBoundingBox();
        geometryDialog->activateBoundingBox(true);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowOverlay()
{
    assert(_manager->HasOverlay());
    if (_manager->IsOverlayVisible())
    {
        _manager->HideOverlay();
    }
    else
    {
        _manager->ShowOverlay();
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowTime()
{
    if (_manager->IsTimeLabelVisible())
    {
        _manager->HideTimeLabel();
    }
    else
    {
        _manager->ShowTimeLabel();
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::onShowColorBar()
{
    if (_manager->IsColorBarVisible())
    {
        _manager->HideColorBar();
    }
    else
    {
        _manager->ShowColorBar();
    }
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Command handlers -- Action Menu

void MvDoc::onPerspectiveProjection()
{
    projectionMode   = ProjectionType::ptPerspective;
    for (auto view : _views)
    {
        view->setProjectionToPerspective();
    }
    updateAllViews(nullptr, nullptr);
}

void MvDoc::onUpdatePerspectiveProjection(QAction* action)
{
    action->setChecked(projectionMode == ProjectionType::ptPerspective);
    action->setEnabled(_manager->GetDataFileList() && !_isAnimating);
}

void MvDoc::onParallelProjection()
{
    projectionMode   = ProjectionType::ptParallel;
    //POSITION pos     = GetFirstViewPosition();
    //CMvView* pView   = (CMvView*)GetNextView(pos);
    //pView->SetProjectionToParallel();
    //UpdateAllViews(NULL);

    for (auto view : _views)
    {
        view->setProjectionToParallel();
    }
    updateAllViews(nullptr, nullptr);
}

void MvDoc::onUpdateParallelProjection(QAction* action)
{
    action->setChecked(projectionMode == ProjectionType::ptParallel);
    action->setEnabled(_manager->GetDataFileList() && !_isAnimating);
}

/////////////////////////////////////////////////////////////////////////////
// Command handlers -- Tools Menu


void MvDoc::setTimePointTo(int timePointIndex)
{
    //QApplication::setOverrideCursor(Qt::WaitCursor);
    _manager->SetTimePointTo(timePointIndex);
    updateAnimation();
}

void MvDoc::updateAnimation()
{
    for (auto view : _views)
    {
        view->onUpdate(nullptr, nullptr);

        // added for animation support in File->Export Animation->Preview and Toolbox->Animation
        // this seems to do what 'GetNextView(pos)->SendMessage(WM_PAINT)' did in the MFC version
        qApp->processEvents();
    }

    if (dataDialog->isVisible())
    {
        double range[2];
        _manager->GetScalarDataRange(range);
        dataDialog->setScalarDataRange(range);
        _manager->GetVectorMagnitudeRange(range);
        dataDialog->setVectorMagnitudeRange(range);
    }

    animationDialog->setAndDisplayCurrentTime(_manager->GetCurrentTimePointIndex());

    if (!_isAnimating)
    {
        bool b;
        switch (_animationType)
        {
        case AnimationType::atTime:
            b = _manager->GetCurrentTimePointIndex() < _manager->GetNumberOfTimePoints() - 1;
            break;
        case AnimationType::atSpace:
            b = true;
            break;
        }

        if (!dataDialog->isVisible())
        {
            double range[2];
            _manager->GetScalarDataRange(range);
            dataDialog->setScalarDataRange(range);
            _manager->GetVectorMagnitudeRange(range);
            dataDialog->setVectorMagnitudeRange(range);
        }

        animationDialog->enableRun(b);
        animationDialog->enableAdvance(b);
        ///QApplication::restoreOverrideCursor();
    }
}

void MvDoc::advanceOneTimePoint()
{
    if (!_isAnimating)
    {
        //QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    }
    _manager->AdvanceOneTimePoint();
    updateAnimationPosition();
}

void MvDoc::updateAnimationPosition()
{
    for(auto view : this->_views)
    {
        view->rotateCamera(animationDialog->rotate);
        view->elevateCamera(animationDialog->elevate);
    }
    updateAnimation();
}


void MvDoc::updateToolDialogs(mvGUISettings* gui)
{
    updateColorBarDialog();
    updateSolidDialog();
    updateIsosurfaceDialog();
    updateLightingDialog(gui);
    updateGeometryDialog();
    updateGridDialog();
    updateAnimationDialog(gui);
    updateVectorDialog();
    updateCropDialog(gui);
    updateDataDialog();
    updatePathlinesDialog();
    updateModelFeaturesDialog();
    updateOverlayDialog();
}

/////////////////////////////////////////////////////////////////////////////
// Scale
/////////////////////////////////////////////////////////////////////////////

void MvDoc::setScale(double xScale, double yScale, double zScale)
{
    _manager->SetScale(xScale, yScale, zScale);

    for (auto view : this->_views)
    {
        view->resetCameraClippingRange();
    }
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Axes
/////////////////////////////////////////////////////////////////////////////

void MvDoc::setAxesRepresentationToLine()
{
    _manager->SetAxesRepresentationToLine();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setAxesRepresentationToTube()
{
    _manager->SetAxesRepresentationToTube();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setAxesProperties(double xPos, double yPos, double zPos,
                               double axesSize, double tubeDiameter)
{
    _manager->SetAxesNormalizedSize(axesSize);
    _manager->SetAxesNormalizedPosition(xPos, yPos, zPos);
    _manager->SetAxesNormalizedTubeDiameter(tubeDiameter);
    updateAllViews(nullptr);
    setModified(true);
}


/////////////////////////////////////////////////////////////////////////////
// Bounding Box
/////////////////////////////////////////////////////////////////////////////

void MvDoc::setBoundingBoxColor(double red, double green, double blue)
{
    _manager->SetBoundingBoxColor(red, green, blue);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setBoundingBoxColor(vtkColor3d color3d)
{
    _manager->SetBoundingBoxColor(color3d.GetRed(), color3d.GetGreen(), color3d.GetBlue());
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Geometry
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onToolboxGeometry()
{
    assert(geometryDialog);
    if (geometryDialog->isVisible())
    {
        geometryDialog->hide();
        //geometryDialog->setVisible(false);
    }
    else
    {
        geometryDialog->show();
        //geometryDialog->setVisible(true);
    }
}

void MvDoc::onUpdateToolboxGeometry(QAction* action)
{
    assert(geometryDialog);
    action->setChecked(geometryDialog->isVisible());
}

void MvDoc::updateGeometryDialog()
{
    // scale
    const double* sc       = _manager->GetScale();
    geometryDialog->xScale = sc[0];
    geometryDialog->yScale = sc[1];
    geometryDialog->zScale = sc[2];
    geometryDialog->updateDataScale(false);
    geometryDialog->activateScale(true);

    // axes
    const double* p                = _manager->GetAxesNormalizedPosition();
    geometryDialog->representation = _manager->GetAxesRepresentation();
    geometryDialog->axesSize       = _manager->GetAxesNormalizedSize();
    geometryDialog->tubeDiameter   = _manager->GetAxesNormalizedTubeDiameter();
    geometryDialog->xPos           = p[0];
    geometryDialog->yPos           = p[1];
    geometryDialog->zPos           = p[2];
    geometryDialog->updateDataAxes(false);
    geometryDialog->activateAxes(_manager->AreAxesVisible());

    // bounding box
    const double* rgb = _manager->GetBoundingBoxColor();
    geometryDialog->boundingBoxColor = (int)(rgb[0] * 2 + 0.1);                     // black = 0, gray = 1, white = 2
    geometryDialog->updateDataBoundingBox(false);
    geometryDialog->activateBoundingBox(_manager->IsBoundingBoxVisible());

    geometryDialog->enableApplyButton(true);
    geometryDialog->setCurrentIndex(0);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Data
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onToolboxData()
{
    assert(dataDialog);
    if (dataDialog->isVisible())
    {
        dataDialog->hide();
    }
    else
    {
        dataDialog->show();
    }
}

void MvDoc::onUpdateToolboxData(QAction* action)
{
    assert(dataDialog);
    action->setChecked(dataDialog->isVisible());
}

void MvDoc::updateDataDialog()
{
    double range[2];
    _manager->GetScalarDataRange(range);
    dataDialog->setScalarDataRange(range);
    if (_manager->HasVectorData())
    {
        _manager->GetVectorMagnitudeRange(range);
        dataDialog->setVectorMagnitudeRange(range);
    }
    else
    {
        dataDialog->setVectorMagnitudeRange(nullptr);
    }
    if (_manager->HasPathlineData())
    {
        _manager->GetPathlineTimeRange(range);
        dataDialog->setPathlineTimeRange(range);
    }
    else
    {
        dataDialog->setPathlineTimeRange(nullptr);
    }
    assert(_manager->GetActiveScalarDataType() >= 0);
    assert(_manager->GetNumberOfScalarDataTypes() > 0);
    assert(_manager->GetNumberOfScalarDataTypes() > _manager->GetActiveScalarDataType());
    dataDialog->setDataTypeLabels(dataTypeLabels(), _manager->GetActiveScalarDataType());
    dataDialog->activate(true);
}

void MvDoc::setScalarDataTypeTo(int index)
{
    assert(index >= 0);
    int n = _manager->GetNumberOfScalarDataTypes();
    assert(index < n);

    _manager->SetScalarDataTypeTo(index);

    //updateColorBarDialog();          // @todo
    //updateSolidDialog();             // @todo
    //updateIsosurfaceDialog();        // @todo
    double range[2];
    _manager->GetScalarDataRange(range);
    dataDialog->setScalarDataRange(range);

    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// utilities
bool MvDoc::hasPathlineData() const
{
    return (_manager->HasPathlineData() != 0);
}


/////////////////////////////////////////////////////////////////////////////
// Toolbox->Color Bar
/////////////////////////////////////////////////////////////////////////////

// afx_msg void OnUpdateColorBarTool(CCmdUI* pCmdUI);
void MvDoc::onUpdateToolboxColorBar(QAction* action)
{
    assert(colorBarDialog);
    action->setChecked(colorBarDialog->isVisible());
}

// afx_msg void OnColorBarTool();
void MvDoc::onToolboxColorBar()
{
    assert(colorBarDialog);
    if (colorBarDialog->isVisible())
    {
        colorBarDialog->hide();
    }
    else
    {
        colorBarDialog->show();
    }
}

// void UpdateColorBarDlg()
void MvDoc::updateColorBarDialog()
{ 
    assert(colorBarDialog);

    // Source
    colorBarDialog->dataSourceIndex = _manager->GetColorBarSource();
    colorBarDialog->updateDataSource(false);

    // Limits
    colorBarDialog->valueBlue        = _manager->GetColorBarValueBlue();
    colorBarDialog->valueRed         = _manager->GetColorBarValueRed();
    colorBarDialog->isColorBarLinear = _manager->IsColorBarLinear();
    colorBarDialog->updateDataLimits(false);

    // Size
    colorBarDialog->width  = _manager->GetColorBarWidth();
    colorBarDialog->height = _manager->GetColorBarHeight();
    colorBarDialog->offset = _manager->GetColorBarOffset();
    colorBarDialog->updateDataSize(false);

    // Labels (text)
    colorBarDialog->fontSize    = _manager->GetColorBarFontSize();
    colorBarDialog->numLabels   = _manager->GetColorBarNumberOfLabels();
    colorBarDialog->precision   = _manager->GetColorBarLabelPrecision();
    const double* rgb           = _manager->GetColorBarTextColor();
    colorBarDialog->colorOption = (int)(rgb[0] * 2 + 0.1);                          // black = 0, gray = 1, white = 2
    colorBarDialog->updateDataLabels(false);

    //CColorBarColorsPage* scheme = m_ColorBarDlg->m_ColorsPage;
    //scheme->InitializeDialog();

    // Colors
    colorBarDialog->updateDataColors(false);

    colorBarDialog->setCurrentIndex(0);
    colorBarDialog->activate(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Lighting
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxLighting(QAction* action)
{
    assert(lightingDialog);
    action->setChecked(lightingDialog->isVisible());
}

void MvDoc::onToolboxLighting()
{
    assert(lightingDialog);
    if (lightingDialog->isVisible())
    {
        lightingDialog->hide();
    }
    else
    {
        lightingDialog->show();
    }
}

void MvDoc::updateLightingDialog(mvGUISettings* gui)
{
    assert(lightingDialog);

    // Lights
    lightingDialog->headlightOn             = (gui->headlightOn != 0);
    lightingDialog->auxiliaryLightOn        = (gui->auxiliaryLightOn != 0);
    lightingDialog->headlightIntensity      = (int)(gui->headlightIntensity * 100 + 0.5);
    lightingDialog->auxiliaryLightIntensity = (int)(gui->auxiliaryLightIntensity * 100 + 0.5);
    lightingDialog->dirX                    = (int)(gui->auxiliaryLightDirection[0] * 10 + 10.5);
    lightingDialog->dirY                    = (int)(gui->auxiliaryLightDirection[1] * 10 + 10.5);
    lightingDialog->dirZ                    = (int)(gui->auxiliaryLightDirection[2] * 10 + 10.5);
    lightingDialog->updateDataLights(false);
    lightingDialog->updateLabelsLights();
    lightingDialog->activateLights(true);

    // Surface
    lightingDialog->diffuse       = (int)(_manager->GetDiffuseLighting() * 100 + 0.5);
    lightingDialog->ambient       = (int)(_manager->GetAmbientLighting() * 100 + 0.5);
    lightingDialog->specular      = (int)(_manager->GetSpecularLighting() * 100 + 0.5);
    lightingDialog->specularPower = (int)(_manager->GetSpecularPower() + 0.5);
    lightingDialog->updateDataSurface(false);
    lightingDialog->updateLabelsSurface();
    lightingDialog->activateSurface(true);

    // Background
    lightingDialog->customBackground = (gui->customBackground != 0);
    lightingDialog->red              = (int)(gui->background[0] * 100 + 0.5);
    lightingDialog->green            = (int)(gui->background[1] * 100 + 0.5);
    lightingDialog->blue             = (int)(gui->background[2] * 100 + 0.5);
    lightingDialog->updateDataBackground(false);
    lightingDialog->updateLabelsBackground();
    lightingDialog->activateBackground(true);

    lightingDialog->setCurrentIndex(0);
    lightingDialog->setDefaultEnabled(true);
}

void MvDoc::setDiffuseLighting(double diffuse)
{
    _manager->SetDiffuseLighting(diffuse);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setAmbientLighting(double ambient)
{
    _manager->SetAmbientLighting(ambient);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setSpecularLighting(double specular)
{
    _manager->SetSpecularLighting(specular);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setSpecularPower(double specularPower)
{
    _manager->SetSpecularPower(specularPower);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::switchOnHeadlight(bool switchOn)
{
    for (auto view : _views)
    {
        view->switchOnHeadlight(switchOn);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setHeadlightIntensity(double intensity)
{
    for (auto view : _views)
    {
        view->setHeadlightIntensity(intensity);
    }
    updateAllViews(nullptr);
    setModified(true);
}


void MvDoc::switchOnAuxiliaryLight(bool switchOn)
{
    for (auto view : _views)
    {
        view->switchOnAuxiliaryLight(switchOn);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setAuxiliaryLightIntensity(double intensity)
{
    for (auto view : _views)
    {
        view->setAuxiliaryLightIntensity(intensity);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setAuxiliaryLightPosition(double x, double y, double z)
{
    for (auto view : _views)
    {
        view->setAuxiliaryLightPosition(x, y, z);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setBackgroundColor(double red, double green, double blue)
{
    for (auto view : _views)
    {
        view->setBackgroundColor(red, green, blue);
    }
    updateAllViews(nullptr);
    setModified(true);
}


/////////////////////////////////////////////////////////////////////////////
// Toolbox->Grid
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxGrid(QAction* action)
{
    assert(gridDialog);
    action->setChecked(gridDialog->isVisible());
}

void MvDoc::onToolboxGrid()
{
    assert(gridDialog);
    if (gridDialog->isVisible())
    {
        gridDialog->hide();
    }
    else
    {
        gridDialog->show();
    }
}

void MvDoc::updateGridDialog()
{
    assert(gridDialog);

    // Using row-col-lay convention of Modflow
    const int* sdim = _manager->GetScalarGridDimensions(); // recall these are dimensions of vtk points
    Q_ASSERT(sdim);

    // Lines
    if (_manager->IsScalarSubgridOn())
    {
        // if subgrid is on, then limit the extent of the grid gridDialog
        const int* voi = _manager->GetScalarSubgridExtent();
        gridDialog->XMin  = voi[0];
        gridDialog->XMax  = voi[1];
        gridDialog->YMin  = voi[2];
        gridDialog->YMax  = voi[3];
        gridDialog->ZMin  = voi[4];
        gridDialog->ZMax  = voi[5];
    }
    else
    {
        if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            gridDialog->XMin = 0;
            gridDialog->XMax = sdim[0] - 1;
            gridDialog->YMin = 0;
            gridDialog->YMax = sdim[1] - 1;
            gridDialog->ZMin = 0;
            gridDialog->ZMax = sdim[2] - 1;
        }
        else
        {
            gridDialog->ZMin = 0;
            gridDialog->ZMax = _manager->GetNumberOfLayersInUnstructuredGrid();
        }
    }
    if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        int p[3];
        _manager->GetGridLinePositions(p);
        gridDialog->positionX = p[0];
        gridDialog->positionY = sdim[1] - p[1] - 1;
        gridDialog->positionZ = sdim[2] - p[2] - 1;

        gridDialog->isActiveX   = _manager->AreGridLinesActive(0);
        gridDialog->isActiveY   = _manager->AreGridLinesActive(1);
        gridDialog->isActiveZ   = _manager->AreGridLinesActive(2);

        const double* rgb    = _manager->GetGridLineColor();
        gridDialog->colorOption = (int)(rgb[0] * 2 + 0.1);                              // black = 0, gray = 1, white = 2
        gridDialog->updateDataLines(false);
        bool activate = _manager->AreActivatedGridLinesVisible();

        gridDialog->activateLines(activate);
    }
    else
    {
        gridDialog->positionZ = _manager->GetGridLayerPosition();
        gridDialog->isActiveX = false;
        gridDialog->isActiveY = false;
        gridDialog->isActiveZ = true;
        // this asserts on ex-gwf-disvmesh
        //assert(gridDialog->isActiveX == (_manager->AreGridLinesActive(0) != 0));
        //assert(gridDialog->isActiveY == (_manager->AreGridLinesActive(1) != 0));
        //assert(gridDialog->isActiveZ == (_manager->AreGridLinesActive(2) != 0));
        const double* rgb       = _manager->GetGridLineColor();
        gridDialog->colorOption = (int)(rgb[0] * 2 + 0.1);                              // black = 0, gray = 1, white = 2
        gridDialog->updateDataLines(false);
        gridDialog->activateLines(_manager->IsGridLayerVisible());
    }

    // Shell
    const double* rgb   = _manager->GetGridShellColor();
    gridDialog->red     = (int)(rgb[0] * 100 + 0.5);
    gridDialog->green   = (int)(rgb[1] * 100 + 0.5);
    gridDialog->blue    = (int)(rgb[2] * 100 + 0.5);
    gridDialog->opacity = (int)(_manager->GetGridShellOpacity() * 100 + 0.5);
    gridDialog->updateDataShell(false);
    gridDialog->updateLabelsShell();
    gridDialog->activateShell(_manager->IsGridShellVisible());


    // Subgrid
    // Subgrid page
    const int* voi = _manager->GetScalarSubgridExtent();
    if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        // structured grid
        gridDialog->col_min            = voi[0] + 1;
        gridDialog->col_max            = voi[1];
        gridDialog->row_min            = sdim[1] - voi[3];
        gridDialog->row_max            = sdim[1] - voi[2] - 1;
        gridDialog->lay_min            = sdim[2] - voi[5];
        gridDialog->lay_max            = sdim[2] - voi[4] - 1;
        gridDialog->col_upper_limit    = sdim[0] - 1;
        gridDialog->row_upper_limit    = sdim[1] - 1;
        gridDialog->lay_upper_limit    = sdim[2] - 1;
        gridDialog->isSubgridActivated = (_manager->IsScalarSubgridOn() != 0);
    }
    else if (_manager->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        gridDialog->col_min            = 0;
        gridDialog->col_max            = 0;
        gridDialog->row_min            = 0;
        gridDialog->row_max            = 0;
        // TO DO: get layer limits from m_Manager
        gridDialog->lay_min            = 1;
        gridDialog->lay_max            = _manager->GetNumberOfLayersInUnstructuredGrid();
        gridDialog->col_upper_limit    = 0;
        gridDialog->row_upper_limit    = 0;
        gridDialog->lay_upper_limit    = _manager->GetNumberOfLayersInUnstructuredGrid();
        gridDialog->isSubgridActivated = false;
    }
    else if (_manager->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        gridDialog->col_min            = 0;
        gridDialog->col_max            = 0;
        gridDialog->row_min            = 0;
        gridDialog->row_max            = 0;
        gridDialog->lay_min            = 0;
        gridDialog->lay_max            = 0;
        gridDialog->col_upper_limit    = 0;
        gridDialog->row_upper_limit    = 0;
        gridDialog->lay_upper_limit    = 0;
        gridDialog->isSubgridActivated = false;
    }
    gridDialog->updateDataSubgrid(false);
    gridDialog->activateSubgrid(true);

}

void MvDoc::setGridLineColor(double red, double green, double blue)
{
    _manager->SetGridLineColor(red, green, blue);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setGridLineColor(vtkColor3d color3d)
{
    setGridLineColor(color3d.GetRed(), color3d.GetGreen(), color3d.GetBlue());
}

void MvDoc::activateGridLines(int slice, bool b)
{
    if (b)
    {
        _manager->ActivateGridLines(slice);
    }
    else
    {
        _manager->DeactivateGridLines(slice);
    }
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setGridLinePositions(int col, int row, int lay)
{
    const int* sdim = _manager->GetScalarGridDimensions(); // returns point dimensions
    Q_ASSERT(sdim);

    int        posX = col;
    int        posY = (sdim[1] - 1) - row;
    int        posZ = (sdim[2] - 1) - lay;
    _manager->SetGridLinePositions(posX, posY, posZ);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setGridLayerPosition(int layerNumber)
{
    _manager->SetGridLayerPosition(layerNumber);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setGridShellColor(double red, double green, double blue)
{
    _manager->SetGridShellColor(red, green, blue);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setGridShellOpacity(double opacity)
{
    _manager->SetGridShellOpacity(opacity);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::applySubgrid(int col_min, int col_max, int row_min, int row_max, int lay_min, int lay_max)
{
    int imin, imax, jmin, jmax, kmin, kmax;
    // convert to vtk indexing and compute extents for points
    if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        const int* sdim = _manager->GetScalarGridDimensions();
        Q_ASSERT(sdim);

        // note that imin, imax, jmin, etc refer to point dimensions
        imin            = col_min - 1;
        imax            = col_max;
        jmin            = sdim[1] - row_max - 1;
        jmax            = sdim[1] - row_min;
        kmin            = sdim[2] - lay_max - 1;
        kmax            = sdim[2] - lay_min;
    }
    else if (_manager->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        imin = 0;
        imax = 0;
        jmin = 0;
        jmax = 0;
        // note that kmin and kmax here refer to cell dimension
        // kmin = m_Manager->GetNumberOfLayersInUnstructuredGrid() - lay_max;
        // kmax = m_Manager->GetNumberOfLayersInUnstructuredGrid() - lay_min-1;
        kmin = lay_min - 1; // top layer in subgrid
        kmax = lay_max - 1; // bottom layer in subgrid
    }
    else
    {
        imin = 0;
        imax = 0;
        jmin = 0;
        jmax = 0;
        kmin = 0;
        kmax = 0;
    }

    _manager->SetScalarSubgridExtent(imin, imax, jmin, jmax, kmin, kmax);
    _manager->ScalarSubgridOn();

    //
    // Original MvMf6 had commented code here that's been removed
    // see CMvDoc::ApplySubgrid
    //

    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::subgridOff()
{
    _manager->ScalarSubgridOff();

    // update the grid lines dlg box
    const int* sdim  = _manager->GetScalarGridDimensions(); // keep in mind these are dimensions of points
    Q_ASSERT(sdim);

    gridDialog->XMin = 0;
    gridDialog->XMax = sdim[0] - 1;
    gridDialog->YMin = 0;
    gridDialog->YMax = sdim[1] - 1;
    gridDialog->ZMin = 0;
    gridDialog->ZMax = sdim[2] - 1;

    // Update the vector dlg box
    if (_manager->HasVectorData())
    {
        const int* vdim = _manager->GetVectorGridDimensions();
        vectorDialog->mColLowerLimit = 1;
        vectorDialog->mColUpperLimit = vdim[0];
        vectorDialog->mRowLowerLimit = 1;
        vectorDialog->mRowUpperLimit = vdim[1];
        vectorDialog->mLayLowerLimit = 1;
        vectorDialog->mLayUpperLimit = vdim[2];
        vectorDialog->updateDataSubsample(false);
    }

    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Overlay
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxOverlay(QAction* action)
{
    assert(overlayDialog);
    action->setChecked(overlayDialog->isVisible());
}

void MvDoc::onToolboxOverlay()
{
    assert(overlayDialog);
    if (overlayDialog->isVisible())
    {
        overlayDialog->hide();
    }
    else
    {
        overlayDialog->show();
    }
}

void MvDoc::updateOverlayDialog()
{
    assert(overlayDialog);

    // Controls
    double x, y;
    _manager->GetOverlayCoordinatesAtGridOrigin(x, y);
    overlayDialog->xOrig          = x;
    overlayDialog->yOrig          = y;
    overlayDialog->scale          = _manager->GetOverlayToGridScale();
    overlayDialog->angle          = _manager->GetOverlayAngle();
    overlayDialog->drape          = _manager->GetOverlayDrape() != 0;
    overlayDialog->trim           = _manager->GetOverlayTrim() != 0;
    overlayDialog->crop           = _manager->GetOverlayCrop() != 0;
    overlayDialog->elev           = _manager->GetOverlayElevation();
    overlayDialog->drapeGap       = _manager->GetOverlayDrapeGap();
    overlayDialog->structuredGrid = _manager->GetIsStructuredGrid();
    overlayDialog->updateDataControls(false);
    overlayDialog->activateControls(_manager->GetDataFileList() != nullptr);

    if (_manager->HasOverlay() != 0)
    {
        // File
        overlayDialog->filename    = _manager->GetOverlayFileName();
        overlayDialog->overlayType = static_cast<OverlayType>(_manager->GetOverlayType() - 1);
        overlayDialog->updateDataFile(false);

        // Bounds
        double xmin, xmax, ymin, ymax;
        _manager->GetOverlayBounds(xmin, xmax, ymin, ymax);
        overlayDialog->setBounds(xmin, xmax, ymin, ymax);

        overlayDialog->setCurrentIndex(0);
        overlayDialog->enableRemoveButton(true);
    }
    else
    {
        // Controls
        overlayDialog->xOrig = defaultXOrigin;
        overlayDialog->yOrig = defaultYOrigin;
        overlayDialog->angle = defaultAngRot;
        overlayDialog->updateDataControls(false);

        // File
        overlayDialog->reinitializeFile();

        // Bounds
        overlayDialog->reinitializeBounds();
        overlayDialog->setCurrentIndex(1);
        overlayDialog->enableRemoveButton(false);
    }
    overlayDialog->activateFile(_manager->GetDataFileList() != nullptr);
    overlayDialog->enableApplyButton(true);
}

bool MvDoc::hasOverlay()
{
    return (_manager->HasOverlay() != 0);
}

void MvDoc::removeOverlay()
{
    _manager->RemoveOverlay();
    updateOverlayDialog();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::applyOverlayControl(const char* filename, int overlayType, double xorig, double yorig,
                                double scale, double angle, int drape, int trim, int crop, double elev, double drapeGap)
{
    _manager->SetOverlayFileName(filename);
    _manager->SetOverlayType(overlayType);
    _manager->SetOverlayCoordinatesAtGridOrigin(xorig, yorig);
    _manager->SetOverlayToGridScale(scale);
    _manager->SetOverlayAngle(angle);
    _manager->SetOverlayDrape(drape);
    _manager->SetOverlayTrim(trim);
    _manager->SetOverlayCrop(crop);
    _manager->SetOverlayElevation(elev);
    _manager->SetOverlayDrapeGap(drapeGap);
    char* errMsg = nullptr;
    if (!_manager->UpdateOverlay(errMsg))
    {
        MainWindow* mainWindow = dynamic_cast<MainWindow*>(parent());
        assert(mainWindow);

        QMessageBox::critical(mainWindow, "", errMsg);
        _manager->HideOverlay();
        overlayDialog->enableRemoveButton(false);

        //overlayDialog->m_BoundsPage->Reinitialize();
        overlayDialog->reinitialize();
        return;
    }
    else
    {
        double xmin, xmax, ymin, ymax;
        _manager->GetOverlayBounds(xmin, xmax, ymin, ymax);
        overlayDialog->setBounds(xmin, xmax, ymin, ymax);
    }
    if (!_manager->IsOverlayVisible())
    {
        _manager->ShowOverlay();
    }
    overlayDialog->enableRemoveButton(true);

    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Solid
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxSolid(QAction* action)
{
    assert(solidDialog);
    action->setChecked(solidDialog->isVisible());
}

void MvDoc::onToolboxSolid()
{
    assert(solidDialog);
    if (solidDialog->isVisible())
    {
        solidDialog->hide();
    }
    else
    {
        solidDialog->show();
    }
}

void MvDoc::updateSolidDialog()
{
    double limits[2];
    _manager->GetSolidThresholdLimits(limits);
    solidDialog->mSolidThresholdMin  = limits[0];
    solidDialog->mSolidThresholdMax  = limits[1];
    solidDialog->mPrimaryScalarMode  = _manager->GetPrimaryScalarMode();
    solidDialog->mApplyThreshold     = _manager->IsSolidThresholdOn();
    solidDialog->mSolidDisplayMode   = _manager->GetSolidDisplayMode();
    solidDialog->mNumberOfColorBands = _manager->GetNumberOfColorBands();
    if (_manager->GetPrimaryScalarMode() == ScalarMode::MV_CELL_SCALARS)
    {
        solidDialog->setRadioButtonBlockyVisible(true);
    }
    else
    {
        solidDialog->setRadioButtonBlockyVisible(false);
    }
    solidDialog->updateData(false);
    solidDialog->activate(_manager->IsSolidVisible());
}

void MvDoc::setSolidDisplayToBlocky()
{
    _manager->SetSolidDisplayToBlocky();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setSolidDisplayToSmooth()
{
    _manager->SetSolidDisplayToSmooth();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setSolidDisplayToBanded()
{
    _manager->SetSolidDisplayToBanded();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::applySolidControl(bool threshold, double minValue, double maxValue, int numberOfColorBands)
{
    _manager->SetSolidThresholdLimits(minValue, maxValue);
    _manager->SetNumberOfColorBands(numberOfColorBands);
    if (threshold)
    {
        _manager->SolidThresholdOn();
    }
    else
    {
        _manager->SolidThresholdOff();
    }
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Isosurface
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxIsosurface(QAction* action)
{
    assert(isosurfaceDialog);
    action->setChecked(isosurfaceDialog->isVisible());
}

void MvDoc::onToolboxIsosurface()
{
    assert(isosurfaceDialog);
    if (isosurfaceDialog->isVisible())
    {
        isosurfaceDialog->hide();
    }
    else
    {
        isosurfaceDialog->show();
    }
}

void MvDoc::updateIsosurfaceDialog()
{
    assert(isosurfaceDialog);

    // Regular
    double                  range[2];
    _manager->GetRegularIsosurfaceRange(range);
    isosurfaceDialog->mIsosurfaceCount = _manager->GetNumberOfRegularIsosurfaces();
    isosurfaceDialog->mValueMin        = range[0];
    isosurfaceDialog->mValueMax        = range[1];
    isosurfaceDialog->updateDataRegular(false);

    // Custom
    const double*          values = _manager->GetCustomIsosurfaceValues();
    for (int i = 0; i < _manager->GetNumberOfCustomIsosurfaces(); i++)
    {
        isosurfaceDialog->mCustomIsosurfaces.push_back(values[i]);

    }
    isosurfaceDialog->updateDataCustom(false);

    // Isosurface Dlg
    if (_manager->UsingRegularIsosurfaces())
    {
        isosurfaceDialog->setCurrentTabIndex(0);
    }
    else
    {
        isosurfaceDialog->setCurrentTabIndex(1);
    }
    isosurfaceDialog->activate(_manager->AreIsosurfacesVisible());
}


void MvDoc::setRegularIsosurfaces(int count, double valueMin, double valueMax)
{
    _manager->SetRegularIsosurfaces(count, valueMin, valueMax);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setCustomIsosurfaces(std::vector<double> values)
{
    _manager->SetCustomIsosurfaces(values.size(), values.data());
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Vector
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxVector(QAction* action)
{
    assert(vectorDialog);
    action->setChecked(vectorDialog->isVisible());
    action->setEnabled(_manager->HasVectorData());
}

void MvDoc::onToolboxVector()
{
    assert(vectorDialog);
    if (vectorDialog->isVisible())
    {
        vectorDialog->hide();
    }
    else
    {
        vectorDialog->show();
    }
}

void MvDoc::updateVectorDialog()
{
    assert(vectorDialog);

    if (_manager->HasVectorData())
    {
        // Subsample (CVectorControlsPage)
        const int* vdim = _manager->GetVectorGridDimensions();
        if (_manager->GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            int extent[6];
            int rate[3];
            _manager->GetVectorSubsampleExtentAndRate(extent, rate);

            vectorDialog->mColMin = extent[0] + 1;
            vectorDialog->mColMax = extent[1] + 1;
            vectorDialog->mColRate = rate[0];
            vectorDialog->mRowMin  = vdim[1] - extent[3];
            vectorDialog->mRowMax  = vdim[1] - extent[2];
            vectorDialog->mRowRate = rate[1];
            vectorDialog->mLayMin  = vdim[2] - extent[5];
            vectorDialog->mLayMax  = vdim[2] - extent[4];
            vectorDialog->mLayRate = rate[2];
            vectorDialog->mStructuredGrid = true;
            if (_manager->IsScalarSubgridOn())
            {
            }
            else
            {
                vectorDialog->setColRange(1, vdim[0]);
                vectorDialog->setRowRange(1, vdim[1]);
                vectorDialog->setLayRange(1, vdim[2]);
            }
            vectorDialog->updateDataSubsample(false);
        }
        else
        {
            vectorDialog->mStructuredGrid = false;
        }

        // Appearance (CVectorOptionsPage)
        const double* rgb           = _manager->GetVectorColor();
        vectorDialog->mScaleFactor  = _manager->GetVectorScaleFactor();
        vectorDialog->mShowGlyph    = _manager->IsVectorGlyphActivated();
        vectorDialog->mColorOption  = static_cast<int>(rgb[0] * 2 + 0.1);       // black = 0, gray = 1, white = 2
        vectorDialog->mLogTransform = _manager->GetLogTransformVector();
        vectorDialog->mLineWidth    = _manager->GetVectorLineWidth();
        vectorDialog->updateDataAppearance(false);

        // Threshold (CVectorThresholdPage)
        double limits[2];
        _manager->GetVectorThresholdLimits(limits);
        vectorDialog->mVectorMin      = limits[0];
        vectorDialog->mVectorMax      = limits[1];
        vectorDialog->mApplyThreshold = _manager->IsVectorThresholdOn();
        vectorDialog->updateDataThreshold(false);

        // Crop (CCropVectorsPage)
        vectorDialog->mCropAngle  = _manager->GetVectorCroppingAngle();
        const double* CropBounds  = _manager->GetVectorCropBounds();
        vectorDialog->mXMin       = CropBounds[0];
        vectorDialog->mXMax       = CropBounds[1];
        vectorDialog->mYMin       = CropBounds[2];
        vectorDialog->mYMax       = CropBounds[3];
        vectorDialog->mZMin       = CropBounds[4];
        vectorDialog->mZMax       = CropBounds[5];
        vectorDialog->updateDataCrop(false);

        // Vector dialog
        vectorDialog->setCurrentTabIndex(0);
        vectorDialog->activate(_manager->AreVectorsVisible());
    }
    else
    {
        vectorDialog->reinitialize();
        vectorDialog->hide();
    }
}

void MvDoc::subsampleVectors(int col_min, int col_max, int col_rate, int row_min, int row_max, int row_rate,
                              int lay_min, int lay_max, int lay_rate)
{
    // convert to indexing starting from zero, and reverse row and layer indexing
    const int* vdim = _manager->GetVectorGridDimensions();
    int        imin = col_min - 1;
    int        imax = col_max - 1;
    int        jmin = vdim[1] - row_max;
    int        jmax = vdim[1] - row_min;
    int        kmin = vdim[2] - lay_max;
    int        kmax = vdim[2] - lay_min;
    _manager->SubsampleVectors(imin, imax, col_rate, jmin, jmax, row_rate, kmin, kmax, lay_rate);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setVectorColor(vtkColor3d color3d)
{
    _manager->SetVectorColor(color3d.GetRed(), color3d.GetGreen(), color3d.GetBlue());
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setLogTransformVector(bool b)
{
    _manager->SetLogTransformVector(b ? 1 : 0);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::activateVectorGlyph(bool b)
{
    _manager->ActivateVectorGlyph(b);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setVectorScaleFactor(double scaleFactor)
{
    _manager->SetVectorScaleFactor(scaleFactor);
    updateAllViews(nullptr);
    setModified(true);
}

double MvDoc::vectorScaleFactor()
{
    return _manager->GetVectorScaleFactor();
}

void MvDoc::setVectorLineWidth(double width)
{
    _manager->SetVectorLineWidth(width);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::enlargeVectorGlyph()
{
    _manager->EnlargeVectorGlyph();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::shrinkVectorGlyph()
{
    _manager->ShrinkVectorGlyph();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::applyVectorThreshold(double minValue, double maxValue)
{
    _manager->SetVectorThresholdLimits(minValue, maxValue);
    _manager->VectorThresholdOn();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::vectorThresholdOff()
{
    _manager->VectorThresholdOff();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::cropVectors(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int cropangle)
{
    _manager->CropVectors(xmin, xmax, ymin, ymax, zmin, zmax, cropangle);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::vectorMagnitudeRange(double* range)
{
    _manager->GetVectorMagnitudeRange(range);
}


/////////////////////////////////////////////////////////////////////////////
// Toolbox->Pathlines
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onToolboxPathlines()
{
    //if (m_PathlinesDlg->IsWindowVisible())
    //{
    //    m_PathlinesDlg->ShowWindow(SW_HIDE);
    //}
    //else
    //{
    //    m_PathlinesDlg->ShowWindow(SW_SHOW);
    //}
}

void MvDoc::onUpdateToolboxPathlines(QAction* action)
{
    // @todo PATHLINES
    action->setEnabled(false);
    //pCmdUI->SetCheck(m_PathlinesDlg->IsWindowVisible());
    //pCmdUI->Enable(m_Manager->HasPathlineData());
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Model Features
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onUpdateToolboxModelFeatures(QAction* action)
{
    assert(modelFeaturesDialog);
    action->setChecked(modelFeaturesDialog->isVisible());
    action->setEnabled(_manager->HasModelFeatures());
}

void MvDoc::onToolboxModelFeatures()
{
    assert(modelFeaturesDialog);
    if (modelFeaturesDialog->isVisible())
    {
        modelFeaturesDialog->hide();
    }
    else
    {
        modelFeaturesDialog->show();
    }
}

/*
    //char**      features = new char*[numFeatureTypes];
    //const char* fstring  = m_Manager->GetModelFeatureLabels();
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    features[i] = new char[41];
    //    strncpy(features[i], fstring + (i * 40), 40);
    //    features[i][40] = '\0';
    //}
*/

std::vector<std::string> MvDoc::modelFeatureLabels()
{
    std::vector<std::string> labels;
    int                      numFeatureTypes = _manager->GetNumberOfModelFeatureTypes();
    const char*              features        = _manager->GetModelFeatureLabels();

    char                     feature[41];
    for (size_t i = 0; i < numFeatureTypes; ++i)
    {
        strncpy(feature, features + (i * 40), 40);
        feature[40] = '\0';
        labels.push_back(feature);
    }
    return labels;
}

void MvDoc::updateModelFeaturesDialog()
{
    // @todo
    assert(modelFeaturesDialog);
    //int i;
    //int numFeatureTypes = m_Manager->GetNumberOfModelFeatureTypes();
    //m_ModelFeaturesDlg->SetNumberOfModelFeatureTypes(numFeatureTypes);
    //int* displayOrder = m_Manager->GetModelFeatureDisplayOrder();
    //m_ModelFeaturesDlg->SetDisplayOrder(displayOrder);
    //char**      features = new char*[numFeatureTypes];
    //const char* fstring  = m_Manager->GetModelFeatureLabels();
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    features[i] = new char[41];
    //    strncpy(features[i], fstring + (i * 40), 40);
    //    features[i][40] = '\0';
    //}
    int              numFeatureTypes = _manager->GetNumberOfModelFeatureTypes();

    //modelFeaturesDialog->SetNumberOfModelFeatureTypes(numFeatureTypes);
    int* displayOrder = _manager->GetModelFeatureDisplayOrder();
    //modelFeaturesDialog->SetDisplayOrder(displayOrder);

    std::vector<int> order;
    for (int i = 0; i < numFeatureTypes; ++i)
    {
        order.push_back(displayOrder[i]);
    }
    modelFeaturesDialog->setDisplayOrder(order);


    //m_ModelFeaturesDlg->m_HideListBox.ResetContent();
    //m_ModelFeaturesDlg->m_ShowListBox.ResetContent();
    //int* temp = new int[numFeatureTypes];
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    temp[i] = -1;
    //}
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    if (displayOrder[i] == -1)
    //    {
    //        m_ModelFeaturesDlg->m_HideListBox.AddString(features[i]);
    //    }
    //    else if (displayOrder[i] > -1 && displayOrder[i] < numFeatureTypes)
    //    {
    //        temp[displayOrder[i]] = i;
    //    }
    //}
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    if (temp[i] > -1)
    //    {
    //        m_ModelFeaturesDlg->m_ShowListBox.AddString(features[temp[i]]);
    //    }
    //}
    //delete[] temp;





    //m_ModelFeaturesDlg->Activate(m_Manager->AreModelFeaturesVisible());
    //if (!m_Manager->HasModelFeatures())
    //{
    //    m_ModelFeaturesDlg->ShowWindow(SW_HIDE);
    //}
    //for (i = 0; i < numFeatureTypes; i++)
    //{
    //    delete[] features[i];
    //}
    //delete[] features;

    //m_ModelFeaturesDlg->ShowGlyphSizeControl(m_Manager->GetModelFeatureDisplayMode() ==
    //                                         MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS);

    // Modflow6DataSource is always MV_DISPLAY_MODEL_FEATURES_AS_CELLS

    modelFeaturesDialog->activate(_manager->AreModelFeaturesVisible());
}

void MvDoc::setModelFeatureDisplayOrder(std::vector<int> displayOrder)
{
    setModelFeatureDisplayOrder(displayOrder.data());
}

void MvDoc::setModelFeatureDisplayOrder(int* displayOrder)
{
    _manager->SetModelFeatureDisplayOrder(displayOrder);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::enlargeModelFeatureGlyphs()
{
    assert(false);
    _manager->EnlargeModelFeatureGlyphs();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::shrinkModelFeatureGlyphs()
{
    assert(false);
    _manager->ShrinkModelFeatureGlyphs();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setModelFeatureColor(const char* modelFeatureName, double* rgba)
{
    _manager->SetModelFeatureColor(modelFeatureName, rgba);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::modelFeatureColor(const char* modelFeatureName, double* rgb)
{
    _manager->GetModelFeatureColor(modelFeatureName, rgb);
}


/////////////////////////////////////////////////////////////////////////////
// Toolbox->Crop
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onToolboxCrop()
{
    assert(cropDialog);
    if (cropDialog->isVisible())
    {
        cropDialog->hide();
    }
    else
    {
        cropDialog->show();
    }
}

void MvDoc::onUpdateToolboxCrop(QAction* action)
{
    assert(cropDialog);
    action->setChecked(cropDialog->isVisible());
}

void MvDoc::updateCropDialog(mvGUISettings* gui)
{
    assert(cropDialog);

    // Controls tab
    const double* cropBounds = _manager->GetCropBounds();
    cropDialog->mXMin        = cropBounds[0];
    cropDialog->mXMax        = cropBounds[1];
    cropDialog->mYMin        = cropBounds[2];
    cropDialog->mYMax        = cropBounds[3];
    cropDialog->mZMin        = cropBounds[4];
    cropDialog->mZMax        = cropBounds[5];
    cropDialog->mXDelta      = gui->cropBoundsXDelta;
    cropDialog->mYDelta      = gui->cropBoundsYDelta;
    cropDialog->mZDelta      = gui->cropBoundsZDelta;
    cropDialog->mXSync       = (cropBounds[0] == cropBounds[1]);
    cropDialog->mYSync       = (cropBounds[2] == cropBounds[3]);
    cropDialog->mZSync       = (cropBounds[4] == cropBounds[5]);
    cropDialog->mCropAngle   = (int)(_manager->GetHorizontalCropAngle() + 0.5);
    cropDialog->updateDataControls(false);

    // Pieces tab
    const double* rgb                 = _manager->GetCroppedAwayPiecesColor();
    cropDialog->showCroppedAwayPieces = _manager->AreCroppedAwayPiecesShown();
    cropDialog->red                   = (int)(rgb[0] * 100 + 0.5);
    cropDialog->green                 = (int)(rgb[1] * 100 + 0.5);
    cropDialog->blue                  = (int)(rgb[2] * 100 + 0.5);
    cropDialog->opacity               = (int)(_manager->GetCroppedAwayPiecesOpacity() * 100 + 0.5);
    cropDialog->updateDataPieces(false);

    // Dialog
    cropDialog->setCurrentTabIndex(0);
    cropDialog->activate(_manager->IsSolidVisible() || _manager->AreIsosurfacesVisible());
}

void MvDoc::crop(double xMin, double xMax, double yMin, double yMax,
                 double zMin, double zMax, double cropAngle)
{
    _manager->Crop(xMin, xMax, yMin, yMax, zMin, zMax, cropAngle);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::showCroppedAwayPieces()
{
    _manager->ShowCroppedAwayPieces();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::hideCroppedAwayPieces()
{
    _manager->HideCroppedAwayPieces();
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setCroppedAwayPiecesColor(double red, double green, double blue)
{
    _manager->SetCroppedAwayPiecesColor(red, green, blue);
    updateAllViews(nullptr);
    setModified(true);
}

void MvDoc::setCroppedAwayPiecesOpacity(double opacity)
{
    _manager->SetCroppedAwayPiecesOpacity(opacity);
    updateAllViews(nullptr);
    setModified(true);
}

/////////////////////////////////////////////////////////////////////////////
// Toolbox->Animation
/////////////////////////////////////////////////////////////////////////////

void MvDoc::onToolboxAnimation()
{
    assert(animationDialog);
    if (animationDialog->isVisible())
    {
        animationDialog->hide();
    }
    else
    {
        animationDialog->show();
    }
}

void MvDoc::onUpdateToolboxAnimation(QAction* action)
{
    assert(animationDialog);
    action->setChecked(animationDialog->isVisible());
}

void MvDoc::updateAnimationDialog(mvGUISettings* gui)
{
    /*
    POSITION pos = GetFirstViewPosition();
    GetNextView(pos)->SendMessage(WM_PAINT);

    double range[2];
    m_Manager->GetScalarDataRange(range);
    m_DataDlg->m_ScalarPage->SetRange(range);
    m_Manager->GetVectorMagnitudeRange(range);
    m_DataDlg->m_VectorPage->SetRange(range);
    m_AnimationDlg->m_ControlsPage->SetAndDisplayCurrentTime(m_Manager->GetCurrentTimePointIndex());

    if (!m_IsAnimating)
    {
        BOOL          b;
        AnimationType at = GetAnimationType();
        switch (at)
        {
        case atTime:
            b = m_Manager->GetCurrentTimePointIndex() < m_Manager->GetNumberOfTimePoints() - 1;
            break;
        case atSpace:
            b = TRUE;
            break;
        }

        m_AnimationDlg->m_ControlsPage->GetDlgItem(IDC_RUN_ANIMATION)->EnableWindow(b);
        m_AnimationDlg->m_ControlsPage->GetDlgItem(IDC_ADVANCE_ANIMATION)->EnableWindow(b);
        EndWaitCursor();
    }
    */

    /*
    assert(animationDialog);

    double range[2];
    _manager->GetScalarDataRange(range);
    dataDialog->setScalarDataRange(range);
    _manager->GetVectorMagnitudeRange(range);
    dataDialog->setVectorMagnitudeRange(range);
    animationDialog->setAndDisplayCurrentTime(_manager->GetCurrentTimePointIndex());
    */

    /*
    // Controls Page
    CAnimationControlsPage* ctrl = m_AnimationDlg->m_ControlsPage;
    ctrl->m_NumberOfTimePoints   = m_Manager->GetNumberOfTimePoints();
    ctrl->m_TimePointLabels      = m_Manager->GetTimePointLabels();
    ctrl->SetAndDisplayCurrentTime(m_Manager->GetCurrentTimePointIndex());
    if (m_Manager->GetTimeLabelOption() == 0)
    {
        ctrl->GetDlgItem(IDC_CURRENT)->SetWindowText("Current time:");
        ctrl->GetDlgItem(IDC_SET_TO)->SetWindowText("Set to time:");
    }
    else
    {
        ctrl->GetDlgItem(IDC_CURRENT)->SetWindowText("Current step:");
        ctrl->GetDlgItem(IDC_SET_TO)->SetWindowText("Set to step:");
    }
    ctrl->SetAnimationType(m_AnimationType);
    ctrl->m_NumberOfSteps = m_AnimationSteps;
    ctrl->UpdateData(FALSE);

    ctrl->Reset();

    // Options Page
    CAnimationOptionsPage* opt = m_AnimationDlg->m_OptionsPage;
    opt->m_Rotate              = gui->animationRotate;
    opt->m_Elevate             = gui->animationElevate;
    opt->m_Delay               = gui->animationDelay;
    opt->UpdateData(FALSE);
    opt->Activate(TRUE);

    // Animation Dlg
    m_AnimationDlg->m_PropertySheet->SetActivePage(0);
    */

    // Controls
    animationDialog->timePointLabels = timePointLabels();
    animationDialog->setAndDisplayCurrentTime(_manager->GetCurrentTimePointIndex());

    if (_manager->GetTimeLabelOption() == 0)
    {
        animationDialog->setCurrentTimeLabel(tr("Current time:"));
        animationDialog->setTimePointsLabel(tr("Set to time:"));
    }
    else
    {
        Q_ASSERT(false);    // currently Modflow6DataSource doesn't override GetTimeLabelOption
        animationDialog->setCurrentTimeLabel(tr("Current step:"));
        animationDialog->setTimePointsLabel(tr("Set to step:"));
    }

    animationDialog->setAnimationType(_animationType);
    animationDialog->numberOfSteps = _animationSteps;
    animationDialog->updateDataControls(false);

    animationDialog->reset();

    // Options
    animationDialog->rotate  = gui->animationRotate;
    animationDialog->elevate = gui->animationElevate;
    animationDialog->delay   = gui->animationDelay;
    animationDialog->updateDataOptions(false);
    animationDialog->activateOptions(true);

    animationDialog->setCurrentTabIndex(0);
}

AnimationType MvDoc::animationType() const
{
    return _animationType;
}

size_t MvDoc::animationSteps() const
{
    return _animationSteps;
}

void MvDoc::setAnimationSteps(int value)
{
    _animationSteps = value;
}

void MvDoc::stopAnimation()
{
    qDebug() << "stopAnimation";
    _isAnimating = false;
}

void MvDoc::setAnimationType(AnimationType value)
{
    _animationType = value;
}

void MvDoc::updateAnimationWithSameTime()
{
    if (!_isAnimating)
    {
        //QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    }
    updateAnimationPosition();
}

void MvDoc::startAnimation()
{
    dataDialog->activate(false);
    colorBarDialog->activate(false);
    lightingDialog->activate(false);
    gridDialog->activate(false);
    geometryDialog->activate(false);
    solidDialog->activate(false);
    isosurfaceDialog->activate(false);
    vectorDialog->activate(false);
    //m_PathlinesDlg->activate(false);      @todo PATHLINES
    modelFeaturesDialog->activate(false);
    cropDialog->activate(false);
    overlayDialog->activate(false);

    _isAnimating = true;

    // AfxBeginThread(ControlFunction, this);
#if defined(USE_THREAD_FOR_ANIMATION)
    // THIS NEEDS MORE WORK @todo
    // https://stackoverflow.com/questions/11033971/qt-thread-with-movetothread
    // https://riptutorial.com/qt/example/17732/basic-usage-of-qthread
    //
    qDebug() << "before moveToThread" << QThread::currentThread();
    QObject* parent = this->parent();
    this->setParent(nullptr);
    
    moveToThread(&animationThread);
    animationThread.setObjectName("workerThread");
    animationThread.start();

    QMetaObject::invokeMethod(this, "animate", Qt::QueuedConnection);

    QThread::msleep(10000);

    this->moveToThread(QApplication::instance()->thread());
    this->setParent(parent);
    qDebug() << "after moveToThread" << QThread::currentThread();
    //this->animate();
#else
    animate();
#endif
}

void MvDoc::animate()
{
    clock_t start, finish;
    double  duration, wait;

    // Animation loop
    if (_animationType == AnimationType::atTime)
    {
        while ((_manager->GetCurrentTimePointIndex() < _manager->GetNumberOfTimePoints() - 1) && _isAnimating)
        {
            start = clock();
            advanceOneTimePoint();
            finish   = clock();
            duration = (finish - start) / (double)CLOCKS_PER_SEC;
            wait     = animationDialog->delay - duration;
            if (wait > 0)
            {
                QThread::msleep(wait * 1000);
            }
        }
    }
    else
    {
        for (size_t index = 0; index < _animationSteps; ++index)
        {
            start = clock();
            updateAnimationWithSameTime();
            finish   = clock();
            duration = (finish - start) / (double)CLOCKS_PER_SEC;
            wait     = animationDialog->delay - duration;
            if (wait > 0)
            {
                QThread::msleep(wait * 1000);
            }
            if (!_isAnimating)
            {
                break;
            }
        }
    }
    if (_isAnimating)
    {
        _isAnimating = false;
        animationDialog->enableStop(false);
    }
    if (_animationType == AnimationType::atTime)
    {
        if (_manager->GetCurrentTimePointIndex() < _manager->GetNumberOfTimePoints() - 1)
        {
            animationDialog->enableRun(true);
            animationDialog->enableAdvance(true);
        }
    }
    else
    {
        animationDialog->enableRun(true);
        animationDialog->enableAdvance(true);
    }
    animationDialog->enableSet(true);
    animationDialog->enableTimePointsCombo(true);
    animationDialog->activateOptions(true);

    dataDialog->activate(true);
    colorBarDialog->activate(true);
    lightingDialog->activate(true);
    gridDialog->activateShell(_manager->IsGridShellVisible());
    gridDialog->activateLines(_manager->AreActivatedGridLinesVisible());
    gridDialog->activateSubgrid(true);
    geometryDialog->activateScale(true);
    geometryDialog->activateAxes(_manager->AreAxesVisible());
    geometryDialog->activateBoundingBox(_manager->IsBoundingBoxVisible());
    solidDialog->activate(_manager->IsSolidVisible());
    isosurfaceDialog->activate(_manager->AreIsosurfacesVisible());
    vectorDialog->activate(_manager->AreVectorsVisible());
    //m_PathlinesDlg->Activate(_manager->ArePathlinesVisible());  @todo PATHLINES
    modelFeaturesDialog->activate(_manager->AreModelFeaturesVisible());
    cropDialog->activate(_manager->IsSolidVisible() || _manager->AreIsosurfacesVisible());
    overlayDialog->activate(true);
    overlayDialog->enableRemoveButton(_manager->HasOverlay());
}


/////////////////////////////////////////////////////////////////////////////
// Toolbox->???


void MvDoc::getPathlineTimeRange(double* range)
{
    _manager->GetPathlineTimeRange(range);
}

void MvDoc::getScalarDataRange(double* range)
{
    _manager->GetScalarDataRange(range);
}

void MvDoc::updatePathlinesDialog()
{
    // @todo PATHLINES
    //assert(pathlinesDialog);
}

GridType MvDoc::gridType()
{
    return _manager->GetGridType();
}
