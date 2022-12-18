#include "datadialog.h"
#include "ui_datadialog.h"

#include <mvdoc.h>

DataDialog::DataDialog(QWidget *parent, MvDoc *doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::DataDialog)
    , doc(doc)
    , _geometry(nullptr)
{
    ui->setupUi(this);

    // @todo PATHLINES
    ui->tabWidget->removeTab(2);

    connect(ui->comboBoxDataType, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int idx) { doc->setScalarDataTypeTo(idx); });
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

DataDialog::~DataDialog()
{
    delete ui;
    delete _geometry;
}

void DataDialog::hideEvent(QHideEvent *e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void DataDialog::showEvent(QShowEvent *e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void DataDialog::reinitialize()
{
    // scalar
    QSignalBlocker comboBlocker(ui->comboBoxDataType);

    ui->lineEditScalarMin->setText("");
    ui->lineEditScalarMax->setText("");
    ui->comboBoxDataType->clear();

    // vector
    ui->lineEditVectorMin->setText("");
    ui->lineEditVectorMax->setText("");

    // pathlines
    ui->lineEditPathlineMin->setText("");
    ui->lineEditPathlineMax->setText("");

    activate(false);
}

void DataDialog::setScalarDataRange(double *range) // std::pair<double> range
{
    ui->lineEditScalarMin->setText(QString(tr("%1")).arg(range[0], 0, 'g'));
    ui->lineEditScalarMax->setText(QString(tr("%1")).arg(range[1], 0, 'g'));
}

void DataDialog::setVectorMagnitudeRange(double *range) // std::pair<double> range
{
    if (range)
    {
        ui->lineEditVectorMin->setText(QString(tr("%1")).arg(range[0], 0, 'g'));
        ui->lineEditVectorMax->setText(QString(tr("%1")).arg(range[1], 0, 'g'));
    }
    else
    {
        ui->lineEditVectorMin->setText(QString(""));
        ui->lineEditVectorMax->setText(QString(""));
    }
}

void DataDialog::setPathlineTimeRange(double *range) // std::pair<double> range
{
    if (range)
    {
        ui->lineEditPathlineMin->setText(QString(tr("%1")).arg(range[0], 0, 'g'));
        ui->lineEditPathlineMax->setText(QString(tr("%1")).arg(range[1], 0, 'g'));
    }
    else
    {
        ui->lineEditPathlineMin->setText(QString(""));
        ui->lineEditPathlineMax->setText(QString(""));
    }
}

void DataDialog::activate(bool b)
{
    ui->comboBoxDataType->setEnabled(b);
}

void DataDialog::setCurrentIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void DataDialog::setDataTypeLabels(QStringList labels, int idx)
{
    QSignalBlocker comboBlocker(ui->comboBoxDataType);

    ui->comboBoxDataType->clear();
    ui->comboBoxDataType->insertItems(0, labels);
    ui->comboBoxDataType->setCurrentIndex(idx);
}
