#ifndef ISOSURFACEDIALOG_H
#define ISOSURFACEDIALOG_H

#include <QDialog>

#include <vector>

namespace Ui {
class IsosurfaceDialog;
}

class MvDoc;
class QListWidgetItem;

class IsosurfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IsosurfaceDialog(QWidget* parent, MvDoc* doc);
    ~IsosurfaceDialog();

    void                hideEvent(QHideEvent* event) override;
    void                showEvent(QShowEvent* e) override;

    void                activate(bool b);
    void                reinitialize();
    bool                updateData(bool saveAndValidate);
    void                setCurrentTabIndex(int index);

    // Regular
    void                activateRegular(bool b);
    void                applyRegular();
    bool                updateDataRegular(bool saveAndValidate);
    void                reinitializeRegular();

    bool                isRegularActive;
    int                 mIsosurfaceCount;
    double              mValueMax;
    double              mValueMin;

    // Custom
    void                activateCustom(bool b);
    void                applyCustom();
    bool                updateDataCustom(bool saveAndValidate);
    void                reinitializeCustom();

    bool                isCustomActive;
    double              mValue;
    QString             mValueAsString;
    std::vector<double> mCustomIsosurfaces;

private slots:
    void onAdd();
    void onDelete();
    void onFull();
    void onApply();
    void itemDoubleClicked(QListWidgetItem* item);
    void currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void onTabChanged(int idx);

private:
    Ui::IsosurfaceDialog* ui;
    MvDoc*                doc;
    QRect*                _geometry;
};

#endif // ISOSURFACEDIALOG_H
