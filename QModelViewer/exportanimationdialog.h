#ifndef EXPORTANIMATIONDIALOG_H
#define EXPORTANIMATIONDIALOG_H

#include <QDialog>

namespace Ui
{
class ExportAnimationDialog;
}

enum class AnimationType;

class ExportAnimationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportAnimationDialog(QWidget *parent = nullptr);
    ~ExportAnimationDialog();

    QStringList          timePointLabels;
    double               MBPerFile;
    bool                 preview;

    // Dialog Data
    double               elevate;
    int                  endIndex;
    QString              filePrefix;
    QString              fileStartNumber;
    QString              defaultOutputFolder;
    QString              outputFolder;
    double               rotate;
    int                  startIndex;
    int                  numberOfSteps;
    AnimationType        animationType;

public slots:
    void done(int r) override;

protected slots:

    void onBrowse();
    void onInitDialog();
    void onChangeComboBoxAnimationType(int index);
    void onChangeStartIndex(int index);
    void onChangeEndIndex(int index);
    void onChangeNumberOfSteps(int value);
    void onPreview();

protected:

    bool validate();

    void updateFiles(int numFiles);

    void updateTimeSpan();

    void showEvent(QShowEvent *event) override;

    void doAnimationTypeChange();

private:
    Ui::ExportAnimationDialog *ui;
};

#endif // EXPORTANIMATIONDIALOG_H
