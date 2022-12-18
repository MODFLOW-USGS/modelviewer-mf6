#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "mv_version.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    if (QString(MV_SHORT_SHA1).length())
    {
        ui->labelVersion->setText(QString(tr("Version %1.%2.%3%4 (%5)")).arg(MV_VERSION_MAJOR).arg(MV_VERSION_MINOR).arg(MV_VERSION_PATCH).arg(MV_VERSION_PRERELEASE).arg(MV_SHORT_SHA1));
    }
    else
    {
        ui->labelVersion->setText(QString(tr("Version %1.%2.X-prerelease")).arg(MV_VERSION_MAJOR).arg(MV_VERSION_MINOR));
    }

    if (QString(MV_DATE_COMPILED).length())
    {
        ui->labelDate->setText(MV_DATE_COMPILED);
    }

    connect(ui->pushButtonOK, &QAbstractButton::clicked,
            [=]() {
                accept();
            });
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
