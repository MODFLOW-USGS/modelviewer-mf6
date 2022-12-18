#ifndef DATASELECTIONDIALOG_H
#define DATASELECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class DataSelectionDialog;
}

class DataSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit DataSelectionDialog(QWidget *parent = nullptr);
    explicit DataSelectionDialog(const QStringList &timePointLabels, const QStringList &dataTypeLabels, QWidget *parent = nullptr);
    ~DataSelectionDialog();

    int selectedTimePoint;
    int selectedDataType;

    //QStringList timePointLabels;
    //QStringList dataTypeLabels;

private:
    Ui::DataSelectionDialog *ui;
};

#endif // DATASELECTIONDIALOG_H
