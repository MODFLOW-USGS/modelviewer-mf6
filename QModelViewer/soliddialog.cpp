#include "soliddialog.h"
#include "ui_soliddialog.h"

#include "mvDefine.h"
#include "mvdoc.h"

#include <QMessageBox>

SolidDialog::SolidDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::SolidDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);
    resize(minimumSizeHint());

    // the following slots don't get called the QCheckBox::setChecked(bool) is called
    connect(ui->radioButtonSmooth, &QAbstractButton::clicked, this, &SolidDialog::onSmooth);
    connect(ui->radioButtonBlocky, &QAbstractButton::clicked, this, &SolidDialog::onBlocky);
    connect(ui->radioButtonBanded, &QAbstractButton::clicked, this, &SolidDialog::onBanded);

    ui->spinBoxColorBands->setRange(2, 50);
    connect(ui->spinBoxColorBands, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mNumberOfColorBands = val;
                doc->applySolidControl(mApplyThreshold, mSolidThresholdMin, mSolidThresholdMax, mNumberOfColorBands);
            });

    // the following slots don't get called the QCheckBox::setChecked(bool) is called
    connect(ui->checkBoxApplyThreshold, QOverload<bool>::of(&QCheckBox::clicked), this, &SolidDialog::onApplyThreshold);

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &SolidDialog::onApply);
    connect(ui->pushButtonFull, &QAbstractButton::clicked, this, &SolidDialog::onFull);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

SolidDialog::~SolidDialog()
{
    delete ui;
    delete _geometry;
}

void SolidDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void SolidDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void SolidDialog::reinitialize()
{
    mApplyThreshold = false;
    mSolidDisplayMode = SolidDisplayType::MV_SOLID_SMOOTH;
    updateData(false);
    ui->lineEditThresholdMin->setText("");
    ui->lineEditThresholdMax->setText("");
    activate(false);
}

void SolidDialog::activate(bool b)
{
    ui->radioButtonSmooth->setEnabled(b && doc->gridType() != GridType::MV_UNSTRUCTURED_GRID);
    ui->radioButtonBlocky->setEnabled(b && doc->gridType() != GridType::MV_UNSTRUCTURED_GRID && mPrimaryScalarMode == ScalarMode::MV_CELL_SCALARS);
    ui->radioButtonBanded->setEnabled(b && doc->gridType() != GridType::MV_UNSTRUCTURED_GRID);
    ui->checkBoxApplyThreshold->setEnabled(b);
    ui->spinBoxColorBands->setEnabled(b && ui->radioButtonBanded->isChecked());
    ui->pushButtonApply->setEnabled(b);

    bool bb = ui->checkBoxApplyThreshold->isChecked();
    ui->lineEditThresholdMin->setEnabled(b && bb);
    ui->lineEditThresholdMax->setEnabled(b && bb);
    ui->pushButtonFull->setEnabled(b && bb);
}

bool SolidDialog::updateData(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mSolidThresholdMin
        s   = ui->lineEditThresholdMin->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditThresholdMin->setFocus();
            return false;
        }
        mSolidThresholdMin = val;

        // mSolidThresholdMax
        s                  = ui->lineEditThresholdMax->text();
        val                = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditThresholdMax->setFocus();
            return false;
        }
        mSolidThresholdMax = val;
    }
    else
    {
        switch (mSolidDisplayMode)
        {
        case SolidDisplayType::MV_SOLID_NOT_DEFINED:
            // fall-through
        case SolidDisplayType::MV_SOLID_SMOOTH:
            ui->radioButtonSmooth->setChecked(true);
            break;
        case SolidDisplayType::MV_SOLID_BLOCKY:
            ui->radioButtonBlocky->setChecked(true);
            break;
        case SolidDisplayType::MV_SOLID_BANDED:
            ui->radioButtonBanded->setChecked(true);
            break;
        default:
            assert(false);
            break;
        }

        ui->spinBoxColorBands->setValue(mNumberOfColorBands);

        ui->lineEditThresholdMin->setText(QString("%1").number(mSolidThresholdMin, 'g', std::numeric_limits<double>::digits10));
        ui->lineEditThresholdMax->setText(QString("%1").number(mSolidThresholdMax, 'g', std::numeric_limits<double>::digits10));
    }
    return true;
}

void SolidDialog::setRadioButtonBlockyVisible(bool visible)
{
    ui->radioButtonBlocky->setVisible(visible);
}

void SolidDialog::onFull()
{
    double range[2];
    doc->getScalarDataRange(range);
    if (range[0] == range[1])
    {
        mSolidThresholdMin = range[0] - 1;
        mSolidThresholdMax = range[1] + 1;
    }
    else
    {
        mSolidThresholdMin = range[0];
        mSolidThresholdMax = range[1];
    }
    updateData(false);
    doc->applySolidControl(mApplyThreshold, mSolidThresholdMin, mSolidThresholdMax, mNumberOfColorBands);
}

void SolidDialog::onSmooth()
{
    mSolidDisplayMode = SolidDisplayType::MV_SOLID_SMOOTH;
    ui->spinBoxColorBands->setEnabled(false);
    doc->setSolidDisplayToSmooth();
}

void SolidDialog::onBlocky()
{
    mSolidDisplayMode = SolidDisplayType::MV_SOLID_BLOCKY;
    ui->spinBoxColorBands->setEnabled(false);
    doc->setSolidDisplayToBlocky();
}

void SolidDialog::onBanded()
{
    mSolidDisplayMode = SolidDisplayType::MV_SOLID_BANDED;
    ui->spinBoxColorBands->setEnabled(true);
    doc->setSolidDisplayToBanded();
}

void SolidDialog::onApplyThreshold(bool checked)
{
    mApplyThreshold = checked;
    ui->lineEditThresholdMin->setEnabled(mApplyThreshold);
    ui->lineEditThresholdMax->setEnabled(mApplyThreshold);
    ui->pushButtonFull->setEnabled(mApplyThreshold);
    onApply();
}

void SolidDialog::onApply()
{
    if (updateData(true))
    {
        doc->applySolidControl(mApplyThreshold, mSolidThresholdMin, mSolidThresholdMax, mNumberOfColorBands);
    }
}
