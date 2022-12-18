#ifndef SOLIDDIALOG_H
#define SOLIDDIALOG_H

#include <QDialog>

namespace Ui {
class SolidDialog;
}

class MvDoc;
enum class SolidDisplayType;
enum class ScalarMode;

class SolidDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolidDialog(QWidget* parent, MvDoc* doc);
    ~SolidDialog();

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *e) override;

    void             activate(bool b);
    void             reinitialize();
    void             setRadioButtonBlockyVisible(bool visible);
    bool             updateData(bool saveAndValidate);

    double           mSolidThresholdMin;
    double           mSolidThresholdMax;
    bool             mApplyThreshold;
    SolidDisplayType mSolidDisplayMode;
    int              mNumberOfColorBands;
    ScalarMode       mPrimaryScalarMode;

private slots:
    void onFull();
    void onSmooth();
    void onBlocky();
    void onBanded();
    void onApply();
    void onApplyThreshold(bool checked);

private:
    Ui::SolidDialog *ui;
    MvDoc *          doc;
    QRect *          _geometry;
};

#endif // SOLIDDIALOG_H
