#include "dataselectiondialog.h"
#include "ui_dataselectiondialog.h"

DataSelectionDialog::DataSelectionDialog(const QStringList &timePointLabels, const QStringList &dataTypeLabels, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataSelectionDialog)
    , selectedTimePoint{0}
    , selectedDataType{0}
{
    ui->setupUi(this);
    ui->comboBoxData->addItems(dataTypeLabels);
    ui->comboBoxTime->addItems(timePointLabels);

    connect(ui->comboBoxData, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int idx) { selectedDataType = idx; });
    connect(ui->comboBoxTime, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int idx) { selectedTimePoint = idx; });

    connect(ui->pushButton, &QAbstractButton::clicked, [=]() { accept(); });
}

DataSelectionDialog::~DataSelectionDialog()
{
    delete ui;
}
