#ifndef MODELFEATURESDIALOG_H
#define MODELFEATURESDIALOG_H

#include <QDialog>

#include <vector>

namespace Ui {
class ModelFeaturesDialog;
}

class MvDoc;
class ColorListModel;

class ModelFeaturesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModelFeaturesDialog(QWidget *parent, MvDoc *doc);
    ~ModelFeaturesDialog();

    void             hideEvent(QHideEvent *event) override;
    void             showEvent(QShowEvent *e) override;

    void             activate(bool b);
    void             reinitialize();
    int              showFeaturesCount();

    void             onSelchangeHideList(const QModelIndex &current, const QModelIndex &previous);
    void             onSelchangeShowList(const QModelIndex &current, const QModelIndex &previous);

    void             onHideFeature();
    void             onShowFeature();

    void             onMoveToTop();
    void             onMoveToBottom();
    void             onMoveUp();
    void             onMoveDown();
    void             onColor();

    void             setDisplayOrder(const std::vector<int> &mDisplayOrder);

#if !defined(NDEBUG)
    bool validateDisplayOrder();
#endif

    std::vector<int> mDisplayOrder;

    ColorListModel * listModelHide;
    ColorListModel * listModelShow;

private:
    Ui::ModelFeaturesDialog *ui;
    MvDoc *                  doc;
    QRect *                  _geometry;
};

#endif // MODELFEATURESDIALOG_H
