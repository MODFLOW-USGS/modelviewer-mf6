#ifndef COLORBARDIALOG_H
#define COLORBARDIALOG_H

#include <QDialog>

namespace Ui {
class ColorBarDialog;
}

class MvDoc;

class ColorBarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorBarDialog(QWidget *parent, MvDoc *doc);
    ~ColorBarDialog();

    void   activate(bool b);

    void   reinitialize();

    bool   updateData(bool saveAndValidate);
    bool   updateDataSource(bool saveAndValidate);
    bool   updateDataLimits(bool saveAndValidate);
    bool   updateDataSize(bool saveAndValidate);
    bool   updateDataLabels(bool saveAndValidate);
    bool   updateDataColors(bool saveAndValidate);

    void   applyDataSource();
    void   applyDataLimits();
    void   applyDataSize();
    void   applyDataLabels();
    void   applyDataColors();

    void   hideEvent(QHideEvent *event) override;
    void   showEvent(QShowEvent *e) override;

    void   setCurrentIndex(int index);

    // Source
    int    dataSourceIndex;

    // Limits
    double valueBlue;
    double valueRed;
    bool   isColorBarLinear;

    // Size
    int    width;
    int    height;
    int    offset;

    // Labels (text)
    int    fontSize;
    int    numLabels;
    int    precision;
    int    colorOption;     // @todo (enum?)

    // Colors
    QColor firstColor;
    QColor lastColor;

public Q_SLOTS:

    void onApply();
    void onDefault();

    void onLowerColor();
    void onUpperColor();

    void onLogScale(int state);
    void onReverse();

private:
    void                onDefaultSource();
    void                onDefaultLimits();
    void                onDefaultSize();
    void                onDefaultLabels();
    void                onDefaultColors();

    Ui::ColorBarDialog *ui;
    MvDoc *             doc;
    QRect *             _geometry;
};

#endif // COLORBARDIALOG_H
