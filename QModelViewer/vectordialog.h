#ifndef VECTORDIALOG_H
#define VECTORDIALOG_H

#include <QDialog>

namespace Ui {
class VectorDialog;
}

class MvDoc;

class VectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VectorDialog(QWidget* parent, MvDoc* doc);
    ~VectorDialog();

    void   hideEvent(QHideEvent* event) override;
    void   showEvent(QShowEvent* e) override;

    void   activate(bool b);
    void   reinitialize();
    void   setCurrentTabIndex(int index);

    // Subsample (CVectorControlsPage)
    void   activateSubsample(bool b);
    void   applySubsample();
    void   defaultSubsample();
    void   reinitializeSubsample();
    bool   updateDataSubsample(bool saveAndValidate);

    void   setColRange(int min, int max);
    void   setRowRange(int min, int max);
    void   setLayRange(int min, int max);

    bool   mStructuredGrid;

    int    mColMin;
    int    mColMax;
    int    mColRate;
    int    mRowMin;
    int    mRowMax;
    int    mRowRate;
    int    mLayMin;
    int    mLayMax;
    int    mLayRate;

    int    mColLowerLimit;
    int    mColUpperLimit;
    int    mRowLowerLimit;
    int    mRowUpperLimit;
    int    mLayLowerLimit;
    int    mLayUpperLimit;

    // Appearance (CVectorOptionsPage)
    void   activateAppearance(bool b);
    void   applyAppearance();
    void   defaultAppearance();
    void   reinitializeAppearance();
    bool   updateDataAppearance(bool saveAndValidate);

    double mScaleFactor;
    bool   mShowGlyph;
    int    mColorOption;
    bool   mLogTransform;
    double mLineWidth;


    // Threshold (CVectorThresholdPage)
    void   activateThreshold(bool b);
    void   applyThreshold();
    void   defaultThreshold();
    void   reinitializeThreshold();
    bool   updateDataThreshold(bool saveAndValidate);

    bool   mApplyThreshold;
    double mVectorMin;
    double mVectorMax;


    // Crop (CCropVectorsPage)
    void   activateCrop(bool b);
    void   applyCrop();
    void   defaultCrop();
    void   defaultCropValues();
    void   reinitializeCrop();
    bool   updateDataCrop(bool saveAndValidate);

    double mXMin;
    double mXMax;
    double mXDelta;
    double mYMin;
    double mYMax;
    double mYDelta;
    double mZMin;
    double mZMax;
    double mZDelta;
    int    mCropAngle;

public Q_SLOTS:
    void onApply();
    void onDefault();

private:
    Ui::VectorDialog* ui;
    MvDoc*            doc;
    QRect*            _geometry;
};

#endif // VECTORDIALOG_H
