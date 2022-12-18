#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MvDoc;
class MvView;

class mvManager;
class mvGUISettings;

class QLabel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void newFile();
    void onFileNew();
    void onFileOpen();        // application MainWindow::open
    void onFileClose();
    //bool save();
    //bool saveAs();
    void openRecentFile();

    bool onFileSave();         // save
    bool onFileSaveAs();       // saveAs
    void onFileExportAsBmp();
    void onFileExportAsPng();

    void onExportAnimation();

    void onPreferences();

    void updateFileActions();
    void updateShowActions();
    void updateActionActions();
    void updateToolboxActions();

    //void about();
    //void documentWasModified();

    void onShowIsosurfaces();
    void onShowNone();
    void onShowSolid();

    void onShowVectors();
    void onShowPathlines();
    void onShowModelFeatures();

    void onShowAxes();
    void onShowGridLines();
    void onShowGridShell();
    void onShowBoundingBox();
    void onShowOverlay();

    void onShowTime();
    void onShowColorBar();

    void onActionCopyDisplay();
    void onActionResetViewpoint();

    void setSize();

    void onViewFromPx();
    void onViewFromNx();
    void onViewFromPy();
    void onViewFromNy();
    void onViewFromPz();
    void onViewFromNz();
    void onViewFromNextDirection();

    // Help
    void onHelpAbout();
    void onHelpContents();

    // Toolbox
    void onToolboxGeometry();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    //bool            isModified() const;
    bool            isAnimating() const;

private:
    friend class MvDoc;

    void            createActions();
    void            createStatusBar();
    void            createMenus();

    //void            updateActions();
    void            updateAllViews();
    void            updateStatusBar();
    void            updateRecentFileActions();

    void            readSettings();
    void            writeSettings();
    void            setCurrentFile(const QString &fileName);
    QString         strippedName(const QString &fullFileName);

    void            setDataName(const QString &name);
    void            setModelName(const QString &name);

    //bool            _modifiedFlag;
    //bool            modifiedFlag();
    //void            setModifiedFlag(bool modifed);

    QLabel *        data;
    QLabel *        model;

    MvDoc *         doc;
    MvView *        view;

    //mvManager *     manager;
    //mvGUISettings * gui;


    //QPlainTextEdit *textEdit;
    //QString         curFile;

    Ui::MainWindow *ui;

    // File
    QAction *       newAct;
    QAction *       openAct;
    QAction *       closeAct;
    QAction *       saveAction;
    QAction *       saveAsAction;

    QAction *       exportAsBmpAction;
    QAction *       exportAsPngAction;
    QAction *       exportAnimationAction;

    QAction *       printAction;
    QAction *       printPreviewAction;
    QAction *       printSetupAction;

    QAction *       preferencesAction;

    QAction *       recentFileSeparatorAction;
    enum { MaxRecentFiles = 4 };
    QAction *       recentFileActions[MaxRecentFiles];
    static const QString recentFilesKey;

    static const QString geometryKey;

    QAction *       exitAct;


    // Show
    QAction *       showSolidAction;
    QAction *       showIsosurfacesAction;
    QAction *       showNoneAction;

    QAction *       showVectorsAction;
    QAction *       showPathlinesAction;
    QAction *       showModelFeaturesAction;

    QAction *       showGridShellAction;
    QAction *       showGridLinesAction;
    QAction *       showAxesAction;
    QAction *       showBoundingBoxAction;
    QAction *       showOverlayAction;

    QAction *       showTimeAction;
    QAction *       showColorBarAction;


    // Action (menu order)
    QAction *       copyDisplayAction;
    QAction *       resetViewpointAction;
    QAction *       setSizeAction;

    QAction *       viewFromPx;
    QAction *       viewFromNx;
    QAction *       viewFromPy;
    QAction *       viewFromNy;
    QAction *       viewFromPz;
    QAction *       viewFromNz;
    QAction *       viewFromNextDirection;

    QAction *       setProjectionToPerspective;
    QAction *       setProjectionToParallel;

    QAction *       saveViewpoint;
    QAction *       recallViewpoint;


    // Toolbox
    QAction *            dataAction;

    QAction *            colorBarAction;
    QAction *            lightingAction;

    QAction *            gridAction;
    QAction *            geometryAction;
    QAction *            overlayAction;

    QAction *            solidAction;
    QAction *            isosurfaceAction;
    QAction *            vectorAction;
    QAction *            modelFeaturesAction;

    QAction *            cropAction;
    QAction *            animationAction;

    // @todo PATHLINES
    QAction *            pathlinesAction;

    // Help
    QAction *            contentsAction;

    QAction *            aboutAction;
};

#endif // MAINWINDOW_H
