#include "displaysizedialog.h"
#include "ui_displaysizedialog.h"

#include <QIntValidator>

#include <cassert>

DisplaySizeDialog::DisplaySizeDialog(const QSize &size, QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , 
    ui(new Ui::DisplaySizeDialog)
{
    ui->setupUi(this);

    assert(size.width() >= 0);
    assert(size.height() >= 0);

    ui->widthEdit->setText(QString::number(size.width()));
    ui->heightEdit->setText(QString::number(size.height()));

    intValidator = new QIntValidator(this);
    intValidator->setBottom(0);

    ui->heightEdit->setValidator(intValidator);
    ui->widthEdit->setValidator(intValidator);

    ui->widthEdit->setFocus();
}

DisplaySizeDialog::~DisplaySizeDialog()
{
    delete ui;
}

QSize DisplaySizeDialog::size() const
{
    int width  = ui->widthEdit->text().toInt();
    int height = ui->heightEdit->text().toInt();
    return QSize(width, height);
}
