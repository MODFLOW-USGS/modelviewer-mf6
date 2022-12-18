#ifndef MODFLOW6DATAFILESDIALOG_H
#define MODFLOW6DATAFILESDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {
class Modflow6DataFilesDialog;
}

class Modflow6DataFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Modflow6DataFilesDialog(QWidget *parent = nullptr);
    ~Modflow6DataFilesDialog();

public Q_SLOTS:
    void done(int) override;

    void onSpecifyModflow6DataFiles();
    void onSpecifyModflow6NameFile();

    void onBrowseModelNameFile();
    void onBrowseModelGridFile();
    void onBrowseModelHeadFile();
    void onBrowseModelBudgetFile();

public:

    bool    specifyNameFile;
    QString gwfModelNameFile;
    QString headOrConcFile;
    QString gridFile;
    QString budgetFile;

protected:
    bool validate();


private:
    Ui::Modflow6DataFilesDialog *ui;
};

#endif // MODFLOW6DATAFILESDIALOG_H
