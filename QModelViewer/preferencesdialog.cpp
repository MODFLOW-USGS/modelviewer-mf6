#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <qabstractview.h>
#include <QSettings>

PreferencesDialog::PreferencesDialog(MouseMode mouseMode, QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    interactorStyle = mouseMode;
    if (mouseMode == MouseMode::mmTrackball)
    {
        ui->radioButtonTrackball->setChecked(true);
    }
    else if (mouseMode == MouseMode::mmJoystick)
    {
        ui->radioButtonJoystick->setChecked(true);
    }

    // lamda lamda lamda

    connect(ui->radioButtonTrackball, &QAbstractButton::clicked,
            [=]() {
            interactorStyle = MouseMode::mmTrackball;
        }
    );

    connect(ui->radioButtonJoystick, &QAbstractButton::clicked,
            [=]() {
            interactorStyle = MouseMode::mmJoystick;
            }
    );
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}
