#ifndef ANIMATIONDIALOG_H
#define ANIMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AnimationDialog;
}

class MvDoc;

enum class AnimationType;

class AnimationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnimationDialog(QWidget* parent, MvDoc* doc);
    ~AnimationDialog();

    void        hideEvent(QHideEvent* event) override;
    void        showEvent(QShowEvent* e) override;

    void        activate(bool b);
    void        reinitialize();
    void        reset();
    void        setCurrentTabIndex(int index);
    void        setAndDisplayCurrentTime(int timePointIndex);
    void        setTimePointsLabel(QString label);
    void        setCurrentTimeLabel(QString label);    
    void        setAnimationType(AnimationType animationType);

    // Controls
    void        activateControls(bool b);
    void        reinitializeControls();
    bool        updateDataControls(bool saveAndValidate);


    void        enableRun(bool b);
    void        enableStop(bool b);
    void        enableAdvance(bool b);
    void        enableSet(bool b);
    void        enableTimePointsCombo(bool b);

    AnimationType animationType;
    int           numberOfSteps;
    int           setToIndex;
    QStringList   timePointLabels;



    // Options
    void          activateOptions(bool b);
    void          reinitializeOptions();
    bool          updateDataOptions(bool saveAndValidate);

    double        delay;
    double        elevate;
    double        rotate;

protected slots:
    void onChangeComboBoxAnimationType(int index);
    void onRun();
    void onTabChanged(int idx);

protected:
    void doAnimationTypeChange();

private:
    Ui::AnimationDialog *ui;
    MvDoc*               doc;
    QRect*               _geometry;
};

#endif // ANIMATIONDIALOG_H
