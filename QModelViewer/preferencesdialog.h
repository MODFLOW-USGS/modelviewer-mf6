#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

enum class MouseMode;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(MouseMode mouseMode, QWidget *parent = nullptr);
    ~PreferencesDialog();

    MouseMode interactorStyle;

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
