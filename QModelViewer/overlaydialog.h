#ifndef OVERLAYDIALOG_H
#define OVERLAYDIALOG_H

#include <QDialog>

namespace Ui {
class OverlayDialog;
}

class MvDoc;
enum class OverlayType;

class OverlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OverlayDialog(QWidget *parent, MvDoc *doc);
    ~OverlayDialog();

    void    hideEvent(QHideEvent *event) override;
    void    showEvent(QShowEvent *e) override;

    void        enableApplyButton(bool enable);
    void        enableRemoveButton(bool enable);
    void        reinitialize();
    void        setCurrentIndex(int index);
    void        activate(bool activate);

    // Controls
    void        activateControls(bool activate);
    void        reinitializeControls();
    bool        updateDataControls(bool saveAndValidate);

    double      xOrig;
    double      yOrig;
    double      scale;
    double      elev;
    double      drapeGap;
    bool        trim;
    double      angle;
    bool        drape;
    bool        crop;
    bool        structuredGrid;

    // File
    void        activateFile(bool activate);
    void        reinitializeFile();
    bool        updateDataFile(bool saveAndValidate);

    QString     filename;
    OverlayType overlayType;

    // Bounds
    void        reinitializeBounds();
    void        setBounds(double xmin, double xmax, double ymin, double ymax);

private slots:
    void onApply();
    void onBrowse();
    void onRemove();
    void onFlat();
    void onDrape();
    void onTrim(bool checked);
    void onCrop(bool checked);

private:
    Ui::OverlayDialog *ui;
    MvDoc *            doc;
    QRect *            _geometry;
};

#endif // OVERLAYDIALOG_H
