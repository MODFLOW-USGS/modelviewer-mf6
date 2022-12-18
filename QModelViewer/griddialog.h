#ifndef GRIDDIALOG_H
#define GRIDDIALOG_H

#include <QDialog>

namespace Ui {
class GridDialog;
}

class MvDoc;

enum class GridType;

class GridDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GridDialog(QWidget *parent, MvDoc *doc);
    ~GridDialog();

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *e) override;

    void     activate(bool b);
    int      currentTabIndex() const;
    void     enableApplyButton(bool enable);
    void     reinitialize();

    // Lines
    void     activateLines(bool activate);
    bool     updateDataLines(bool saveAndValidate);


    bool structuredGrid;
    bool isActive;  // @todo ???

    bool isActiveX;
    bool isActiveY;
    bool isActiveZ;

    int  XMin;
    int  XMax;
    int  YMin;
    int  YMax;
    int  ZMin;
    int  ZMax;

    int  positionX;
    int  positionY;
    int  positionZ;

    int  colorOption;


    // Shell
    void     activateShell(bool activate);
    bool     updateDataShell(bool saveAndValidate);
    void     updateLabelsShell();

    int red;
    int green;
    int blue;
    int opacity;

    // Subgrid
    void     activateSubgrid(bool activate);
    void     applySubgrid();
    bool     updateDataSubgrid(bool saveAndValidate);

    GridType gridType;
    bool isSubgridActivated;
    int  col_min;
    int  col_max;
    int  row_min;
    int  row_max;
    int  lay_min;
    int  lay_max;
    int  col_upper_limit;
    int  row_upper_limit;
    int  lay_upper_limit;
    bool IsActive;

public slots:
    void onActivateX();
    void onActivateY();
    void onActivateZ();

    void onCheckBoxSubgrid();

private:
    Ui::GridDialog *ui;
    MvDoc *         doc;
    QRect *         _geometry;
};

#endif // GRIDDIALOG_H
