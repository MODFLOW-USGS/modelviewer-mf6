#ifndef GEOMETRYDIALOG_H
#define GEOMETRYDIALOG_H

#include <QDialog>

class MvDoc;

namespace Ui {
class GeometryDialog;
}

class GeometryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeometryDialog(QWidget *parent, MvDoc *doc);
    ~GeometryDialog();

    void activate(bool b);

    void activateScale(bool b);
    void activateAxes(bool b);
    void activateBoundingBox(bool b);

    void reinitialize();

    void enableApplyButton(bool enabled);
    void setCurrentIndex(int index);

    bool updateData(bool saveAndValidate);
    bool updateDataScale(bool saveAndValidate);
    bool updateDataAxes(bool saveAndValidate);
    bool updateDataBoundingBox(bool saveAndValidate);

public Q_SLOTS:

    void onApply();

public:

    // scale
    double xScale;
    double yScale;
    double zScale;

    // axes
    int    representation;          // 0 = tube, 1 = line
    double axesSize;
    double tubeDiameter;
    double xPos;
    double yPos;
    double zPos;

    // bounding box
    int    boundingBoxColor;        // 0 = black, 1 = grey; 2 = white

protected:

    void onRadioButtonLine();
    void onRadioButtonTube();

    void onTabChanged(int index);

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *e) override;

private:
    Ui::GeometryDialog *ui;
    MvDoc *             doc;
    QRect *             _geometry;
};

#endif // GEOMETRYDIALOG_H
