#ifndef CROPDIALOG_H
#define CROPDIALOG_H

#include <QDialog>

namespace Ui {
class CropDialog;
}

class MvDoc;

class CropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CropDialog(QWidget* parent, MvDoc* doc);
    ~CropDialog();

    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* e) override;

    void   activate(bool b);
    void   onTabChanged(int idx);
    void   reinitialize();
    void   setCurrentTabIndex(int index);

    // Controls (CCropControlsPage)
    void   activateControls(bool b);
    void   applyControls();
    void   defaultControls();
    void   defaultControlValues();
    void   reinitializeControls();
    bool   updateDataControls(bool saveAndValidate);

    double mXMin;
    double mXMax;
    double mXDelta;
    bool   mXSync;
    double mYMin;
    double mYMax;
    double mYDelta;
    bool   mYSync;
    double mZMin;
    double mZMax;
    double mZDelta;
    bool   mZSync;
    int    mCropAngle;

    // Pieces (CCropOptionsPage)
    void   activatePieces(bool b);
    void   defaultPieces();
    void   defaultPiecesValues();
    void   reinitializePieces();
    bool   updateDataPieces(bool saveAndValidate);
    void   updateLabelsPieces();

    bool   showCroppedAwayPieces;
    int    red;
    int    green;
    int    blue;
    int    opacity;

public Q_SLOTS:
    void onApply();
    void onDefault();

private:
    Ui::CropDialog *ui;
    MvDoc*          doc;
    QRect*          _geometry;
};

#endif // CROPDIALOG_H
