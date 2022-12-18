#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>

namespace Ui {
class DataDialog;
}

class MvDoc;

class DataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataDialog(QWidget *parent, MvDoc *doc);
    ~DataDialog();

    void        activate(bool b);

    void        setDataTypeLabels(QStringList labels, int idx);

    void        setCurrentIndex(int index);

    void        setScalarDataRange(double *range);
    void        setVectorMagnitudeRange(double *range);
    void        setPathlineTimeRange(double *range);

    void        reinitialize();

    void        hideEvent(QHideEvent *event) override;
    void        showEvent(QShowEvent *e) override;

private:
    Ui::DataDialog *ui;
    MvDoc *         doc;
    QRect *         _geometry;
};

#endif // DATADIALOG_H
