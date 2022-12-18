#ifndef DISPLAYSIZEDIALOG_H
#define DISPLAYSIZEDIALOG_H

#include <QDialog>
#include <QSize>


namespace Ui {
class DisplaySizeDialog;
}

class QIntValidator;

class DisplaySizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisplaySizeDialog(const QSize &size, QWidget *parent = nullptr);
    ~DisplaySizeDialog();

    QSize size() const;

private:
    Ui::DisplaySizeDialog *ui;
    QIntValidator *        intValidator;
};

#endif // DISPLAYSIZEDIALOG_H
