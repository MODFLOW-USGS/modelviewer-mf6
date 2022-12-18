#include "vectordialog.h"
#include "ui_vectordialog.h"

#include "mvdoc.h"

#include <QDebug>
#include <QMessageBox>
#include <QDoubleValidator>

#include <vtkColor.h>
#include <vtkNamedColors.h>

#include <cmath>


VectorDialog::VectorDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::VectorDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);
    // resize(minimumSizeHint());

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);

    // Subsample (CVectorControlsPage)
    mColLowerLimit = 0;
    mColUpperLimit = 0;
    mRowLowerLimit = 0;
    mRowUpperLimit = 0;
    mLayLowerLimit = 0;
    mLayUpperLimit = 0;

    ui->spinBoxIRate->setRange(1, VTK_INT_MAX);
    ui->spinBoxJRate->setRange(1, VTK_INT_MAX);
    ui->spinBoxKRate->setRange(1, VTK_INT_MAX);

    //connect(ui->spinBoxIMin, QOverload<int>::of(&QSpinBox::valueChanged), this, &VectorDialog::apply);
    connect(ui->spinBoxIMin, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mColMin = val;
                ui->spinBoxIMax->setMinimum(mColMin);
                applySubsample();
            });
    connect(ui->spinBoxJMin, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mRowMin = val;
                ui->spinBoxJMax->setMinimum(mRowMin);
                applySubsample();
            });
    connect(ui->spinBoxKMin, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mLayMin = val;
                ui->spinBoxKMax->setMinimum(mLayMin);
                applySubsample();
            });
    connect(ui->spinBoxIMax, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mColMax = val;
                ui->spinBoxIMin->setMaximum(mColMax);
                applySubsample();
            });
    connect(ui->spinBoxJMax, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mRowMax = val;
                ui->spinBoxJMin->setMaximum(mRowMax);
                applySubsample();
            });
    connect(ui->spinBoxKMax, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mLayMax = val;
                ui->spinBoxKMin->setMaximum(mLayMax);
                applySubsample();
            });

    connect(ui->spinBoxIRate, QOverload<int>::of(&QSpinBox::valueChanged), this, &VectorDialog::applySubsample);
    connect(ui->spinBoxJRate, QOverload<int>::of(&QSpinBox::valueChanged), this, &VectorDialog::applySubsample);
    connect(ui->spinBoxKRate, QOverload<int>::of(&QSpinBox::valueChanged), this, &VectorDialog::applySubsample);

    // Appearance (CVectorOptionsPage)
    mScaleFactor  = 0.0;
    mShowGlyph    = false;
    mColorOption  = 0;
    mLogTransform = false;
    mLineWidth    = 1.0;

    connect(ui->checkBoxLogTransform, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mLogTransform = (val == Qt::Checked);
                doc->setLogTransformVector(mLogTransform);
                mScaleFactor = doc->vectorScaleFactor();
                updateDataAppearance(false);
            });

    connect(ui->checkBoxShowBase, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mShowGlyph = (val == Qt::Checked);
                doc->activateVectorGlyph(mShowGlyph);
                ui->toolButtonLarger->setEnabled(mShowGlyph);
                ui->toolButtonSmaller->setEnabled(mShowGlyph);
            });

    connect(ui->toolButtonLarger, &QAbstractButton::clicked,
            [=]() {
                doc->enlargeVectorGlyph();
            });
    connect(ui->toolButtonSmaller, &QAbstractButton::clicked,
            [=]() {
                doc->shrinkVectorGlyph();
            });


    // vector color
    vtkNew<vtkNamedColors> colors;
    vtkColor3d             Black = colors->GetColor3d("Black");
    vtkColor3d             Gray  = colors->GetColor3d("Gray");
    vtkColor3d             White = colors->GetColor3d("White");

    connect(ui->radioButtonBlack, &QAbstractButton::clicked, [=]() { doc->setVectorColor(Black); });
    connect(ui->radioButtonGray,  &QAbstractButton::clicked, [=]() { doc->setVectorColor(Gray); });
    connect(ui->radioButtonWhite, &QAbstractButton::clicked, [=]() { doc->setVectorColor(White); });

    // Threshold (CVectorThresholdPage)
    connect(ui->checkBoxApplyThreshold, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mApplyThreshold = (val == Qt::Checked);
                ui->lineEditVectorMin->setEnabled(mApplyThreshold);
                ui->lineEditVectorMax->setEnabled(mApplyThreshold);
                applyThreshold();
            });

    // Crop (CCropVectorsPage)
    defaultCropValues();

    ui->doubleSpinBoxXMin->setRange(0., 1.);
    ui->doubleSpinBoxXMax->setRange(0., 1.);
    ui->doubleSpinBoxYMin->setRange(0., 1.);
    ui->doubleSpinBoxYMax->setRange(0., 1.);
    ui->doubleSpinBoxZMin->setRange(0., 1.);
    ui->doubleSpinBoxZMax->setRange(0., 1.);
    ui->spinBoxCropAngle->setRange(-45, 45);

    ui->lineEditXDelta->setValidator(doubleValidator);
    ui->lineEditYDelta->setValidator(doubleValidator);
    ui->lineEditZDelta->setValidator(doubleValidator);

    connect(ui->doubleSpinBoxXMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mXMin = val;
                ui->doubleSpinBoxXMax->setMinimum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxXMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mXMax = val;
                ui->doubleSpinBoxXMin->setMaximum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });

    connect(ui->doubleSpinBoxYMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mYMin = val;
                ui->doubleSpinBoxYMax->setMinimum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxYMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mYMax = val;
                ui->doubleSpinBoxYMin->setMaximum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });

    connect(ui->doubleSpinBoxZMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mZMin = val;
                ui->doubleSpinBoxZMax->setMinimum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxZMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mZMax = val;
                ui->doubleSpinBoxZMin->setMaximum(val);
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });

    connect(ui->spinBoxCropAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mCropAngle = val;
                doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &VectorDialog::onApply);
    connect(ui->pushButtonDefault, &QAbstractButton::clicked, this, &VectorDialog::onDefault);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, this, &QWidget::hide);
}

VectorDialog::~VectorDialog()
{
    delete ui;
    delete _geometry;
}

void VectorDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void VectorDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void VectorDialog::activate(bool b)
{
    // Subsample (CVectorControlsPage)
    activateSubsample(b);

    // Appearance (CVectorOptionsPage)
    activateAppearance(b);

    // Threshold (CVectorThresholdPage)
    activateThreshold(b);

    // Crop (CCropVectorsPage)
    activateCrop(b);

    ui->pushButtonApply->setEnabled(b);
    ui->pushButtonDefault->setEnabled(b);
}

void VectorDialog::activateSubsample(bool b)
{
    ui->spinBoxIMin->setEnabled(b && mStructuredGrid);
    ui->spinBoxIMax->setEnabled(b && mStructuredGrid);
    ui->spinBoxJMin->setEnabled(b && mStructuredGrid);
    ui->spinBoxJMax->setEnabled(b && mStructuredGrid);
    ui->spinBoxKMin->setEnabled(b && mStructuredGrid);
    ui->spinBoxKMax->setEnabled(b && mStructuredGrid);
}

void VectorDialog::activateAppearance(bool b)
{
    ui->lineEditScaleFactor->setEnabled(b);
    ui->checkBoxShowBase->setEnabled(b);
    bool bb = ui->checkBoxShowBase->isChecked();
    ui->toolButtonLarger->setEnabled(b && bb);
    ui->toolButtonSmaller->setEnabled(b && bb);
    ui->radioButtonBlack->setEnabled(b);
    ui->radioButtonGray->setEnabled(b);
    ui->radioButtonWhite->setEnabled(b);
    ui->checkBoxLogTransform->setEnabled(b);
    ui->lineEditLineWidth->setEnabled(b);
}

void VectorDialog::activateThreshold(bool b)
{
    ui->checkBoxApplyThreshold->setEnabled(b);
    bool bb = ui->checkBoxApplyThreshold->isChecked();
    ui->lineEditVectorMin->setEnabled(b && bb);
    ui->lineEditVectorMax->setEnabled(b && bb);
}

void VectorDialog::activateCrop(bool b)
{
    ui->doubleSpinBoxXMin->setEnabled(b);
    ui->doubleSpinBoxXMax->setEnabled(b);
    ui->doubleSpinBoxYMin->setEnabled(b);
    ui->doubleSpinBoxYMax->setEnabled(b);
    ui->doubleSpinBoxZMin->setEnabled(b);
    ui->doubleSpinBoxZMax->setEnabled(b);
    ui->lineEditXDelta->setEnabled(b);
    ui->lineEditYDelta->setEnabled(b);
    ui->lineEditZDelta->setEnabled(b);
    ui->spinBoxCropAngle->setEnabled(b);
}


void VectorDialog::reinitialize()
{
    // Subsample (CVectorControlsPage)
    reinitializeSubsample();

    // Appearance (CVectorOptionsPage)
    reinitializeAppearance();

    // Threshold (CVectorThresholdPage)
    reinitializeThreshold();

    // Crop (CCropVectorsPage)
    reinitializeCrop();

    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonDefault->setEnabled(false);
}

void VectorDialog::reinitializeSubsample()
{
    activateSubsample(false);
}

void VectorDialog::reinitializeAppearance()
{
    mScaleFactor  = 0.0;
    mShowGlyph    = false;
    mColorOption  = 0;
    mLogTransform = false;
    mLineWidth    = 1.0;

    activateAppearance(false);
}

void VectorDialog::reinitializeThreshold()
{
    ui->lineEditVectorMin->setText("");
    ui->lineEditVectorMax->setText("");
    ui->checkBoxApplyThreshold->setChecked(false);
    mApplyThreshold = false;
    activateThreshold(false);
}

void VectorDialog::reinitializeCrop()
{
    defaultCropValues();
    activateCrop(false);
}

void VectorDialog::setColRange(int min, int max)
{
    QSignalBlocker minIBlock(ui->spinBoxIMin);
    QSignalBlocker maxIBlock(ui->spinBoxIMax);
    mColLowerLimit = min;
    mColUpperLimit = max;
    ui->spinBoxIMin->setRange(min, max);
    ui->spinBoxIMax->setRange(min, max);
    ui->spinBoxIMin->setValue(min);
    ui->spinBoxIMax->setValue(max);
}

void VectorDialog::setRowRange(int min, int max)
{
    QSignalBlocker minJBlock(ui->spinBoxJMin);
    QSignalBlocker maxJBlock(ui->spinBoxJMax);
    mRowLowerLimit = min;
    mRowUpperLimit = max;
    ui->spinBoxJMin->setRange(min, max);
    ui->spinBoxJMax->setRange(min, max);
    ui->spinBoxJMin->setValue(min);
    ui->spinBoxJMax->setValue(max);
}

void VectorDialog::setLayRange(int min, int max)
{
    QSignalBlocker minKBlock(ui->spinBoxKMin);
    QSignalBlocker maxKBlock(ui->spinBoxKMax);
    mLayLowerLimit = min;
    mLayUpperLimit = max;
    ui->spinBoxKMin->setRange(min, max);
    ui->spinBoxKMax->setRange(min, max);
    ui->spinBoxKMin->setValue(min);
    ui->spinBoxKMax->setValue(max);
}

bool VectorDialog::updateDataSubsample(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        mColMin  = ui->spinBoxIMin->value();
        mColMax  = ui->spinBoxIMax->value();
        mColRate = ui->spinBoxIRate->value();

        mRowMin  = ui->spinBoxJMin->value();
        mRowMax  = ui->spinBoxJMax->value();
        mRowRate = ui->spinBoxJRate->value();

        mLayMin  = ui->spinBoxKMin->value();
        mLayMax  = ui->spinBoxKMax->value();
        mLayRate = ui->spinBoxKRate->value();
    }
    else
    {
        QSignalBlocker minIBlock(ui->spinBoxIMin);
        QSignalBlocker maxIBlock(ui->spinBoxIMax);
        QSignalBlocker rateIBlock(ui->spinBoxIRate);
        ui->spinBoxIMin->setValue(mColMin);
        ui->spinBoxIMax->setValue(mColMax);
        ui->spinBoxIRate->setValue(mColRate);

        QSignalBlocker minJBlock(ui->spinBoxJMin);
        QSignalBlocker maxJBlock(ui->spinBoxJMax);
        QSignalBlocker rateJBlock(ui->spinBoxJRate);
        ui->spinBoxJMin->setValue(mRowLowerLimit);
        ui->spinBoxJMax->setValue(mRowUpperLimit);
        ui->spinBoxJRate->setValue(mRowRate);

        QSignalBlocker minKBlock(ui->spinBoxKMin);
        QSignalBlocker maxKBlock(ui->spinBoxKMax);
        QSignalBlocker rateKBlock(ui->spinBoxKRate);
        ui->spinBoxKMin->setValue(mLayLowerLimit);
        ui->spinBoxKMax->setValue(mLayUpperLimit);
        ui->spinBoxKRate->setValue(mLayRate);
    }
    return true;
}

bool VectorDialog::updateDataAppearance(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mScaleFactor
        s   = ui->lineEditScaleFactor->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditScaleFactor->setFocus();
            return false;
        }
        mScaleFactor = val;

        // mShowGlyph
        assert(mShowGlyph == ui->checkBoxShowBase->isChecked());

        // mColorOption
        // no-op

        // mLogTransform
        assert(mLogTransform == ui->checkBoxLogTransform->isChecked());

        // mLineWidth
        s   = ui->lineEditLineWidth->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditLineWidth->setFocus();
            return false;
        }
        mLineWidth = val;
    }
    else
    {
        // mScaleFactor
        ui->lineEditScaleFactor->setText(QString("%1").number(mScaleFactor, 'g'));

        // mLogTransform
        ui->checkBoxLogTransform->setChecked(mLogTransform);

        // mShowGlyph
        ui->checkBoxShowBase->setChecked(mShowGlyph);
        ui->toolButtonLarger->setEnabled(mShowGlyph);
        ui->toolButtonSmaller->setEnabled(mShowGlyph);

        // mColorOption
        switch (mColorOption)
        {
        case 0:
            ui->radioButtonBlack->setChecked(true);
            break;
        case 1:
            ui->radioButtonGray->setChecked(true);
            break;
        case 2:
            ui->radioButtonWhite->setChecked(true);
            break;
        default:
            assert(false);
            break;
        }

        // mLineWidth
        ui->lineEditLineWidth->setText(QString("%1").number(mLineWidth, 'g'));
    }
    return true;
}

bool VectorDialog::updateDataThreshold(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mApplyThreshold
        mApplyThreshold = ui->checkBoxApplyThreshold->isChecked();

        // mVectorMin
        s   = ui->lineEditVectorMin->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditVectorMin->setFocus();
            return false;
        }
        mVectorMin = val;

        // mVectorMax
        s          = ui->lineEditVectorMax->text();
        val        = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditVectorMax->setFocus();
            return false;
        }
        mVectorMax = val;
    }
    else
    {
        // mApplyThreshold
        ui->checkBoxApplyThreshold->setChecked(mApplyThreshold);

        // mVectorMin
        ui->lineEditVectorMin->setText(QString("%1").number(mVectorMin, 'g'));

        // mVectorMax
        ui->lineEditVectorMax->setText(QString("%1").number(mVectorMax, 'g'));
    }
    return true;
}

bool VectorDialog::updateDataCrop(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mXDelta
        s   = ui->lineEditXDelta->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditXDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditXDelta->setFocus();
            return false;
        }
        mXDelta = val;
        ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMin->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
        ui->doubleSpinBoxXMax->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));

        // mYDelta
        s       = ui->lineEditYDelta->text();
        val     = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditYDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditYDelta->setFocus();
            return false;
        }
        mYDelta = val;
        ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMin->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
        ui->doubleSpinBoxYMax->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));

        // mZDelta
        s   = ui->lineEditZDelta->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditZDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditZDelta->setFocus();
            return false;
        }
        mZDelta = val;
        ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMax->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMin->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));
        ui->doubleSpinBoxZMax->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));

    }
    else
    {
        ui->doubleSpinBoxXMin->setValue(mXMin);
        ui->doubleSpinBoxXMax->setValue(mXMax);
        ui->doubleSpinBoxYMin->setValue(mYMin);
        ui->doubleSpinBoxYMax->setValue(mYMax);
        ui->doubleSpinBoxZMin->setValue(mZMin);
        ui->doubleSpinBoxZMax->setValue(mZMax);

        ui->lineEditXDelta->setText(QString("%1").number(mXDelta, 'g'));
        ui->lineEditYDelta->setText(QString("%1").number(mYDelta, 'g'));
        ui->lineEditZDelta->setText(QString("%1").number(mZDelta, 'g'));

        ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMin->setDecimals(abs(log10(mXDelta)));
        ui->doubleSpinBoxXMax->setDecimals(abs(log10(mXDelta)));

        ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMin->setDecimals(abs(log10(mYDelta)));
        ui->doubleSpinBoxYMax->setDecimals(abs(log10(mYDelta)));

        ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMax->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMin->setDecimals(abs(log10(mZDelta)));
        ui->doubleSpinBoxZMax->setDecimals(abs(log10(mZDelta)));

        ui->spinBoxCropAngle->setValue(mCropAngle);
    }
    return true;
}


void VectorDialog::onApply()
{
    int n = this->ui->tabWidget->currentIndex();
    switch (n)
    {
    case 0: // Subsample (CVectorControlsPage)
        applySubsample();
        break;
    case 1: // Appearance (CVectorOptionsPage)
        applyAppearance();
        break;
    case 2: // Threshold (CVectorThresholdPage)
        applyThreshold();
        break;
    case 3: // Crop (CCropVectorsPage)
        applyCrop();
        break;
    default:
        assert(false);
        break;
    }
}

void VectorDialog::applySubsample()
{
    if (updateDataSubsample(true))
    {
        doc->subsampleVectors(mColMin, mColMax, mColRate, mRowMin, mRowMax, mRowRate, mLayMin, mLayMax, mLayRate);
    }
}

void VectorDialog::applyAppearance()
{
    if (updateDataAppearance(true))
    {
        doc->setVectorScaleFactor(mScaleFactor);
        doc->setVectorLineWidth(mLineWidth);
    }
}

void VectorDialog::applyThreshold()
{
    if (updateDataThreshold(true))
    {
        if (mApplyThreshold)
        {
            doc->applyVectorThreshold(mVectorMin, mVectorMax);
        }
        else
        {
            doc->vectorThresholdOff();
        }
    }
}

void VectorDialog::applyCrop()
{
    if (updateDataCrop(true))
    {
        doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
    }
}


void VectorDialog::onDefault()
{
    const QMessageBox::StandardButton ret = QMessageBox::question(this,
                                                                  tr("Model Viewer for Modflow 6"),
                                                                  tr("Do you want to use default settings?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No)
    {
        return;
    }

    switch (ui->tabWidget->currentIndex())
    {
    case 0:
        // Subsample (CVectorControlsPage)
        defaultSubsample();
        break;
    case 1:
        // Appearance (CVectorOptionsPage)
        defaultAppearance();
        break;
    case 2:
        // Threshold (CVectorThresholdPage)
        defaultThreshold();
        break;
    case 3:
        // Crop (CCropVectorsPage)
        defaultCrop();
        break;
    default:
        assert(false);
        break;
    }
}

void VectorDialog::defaultSubsample()
{
    mColMin  = mColLowerLimit;
    mColMax  = mColUpperLimit;
    mRowMin  = mRowLowerLimit;
    mRowMax  = mRowUpperLimit;
    mLayMin  = mLayLowerLimit;
    mLayMax  = mLayUpperLimit;
    mColRate = 1;
    mRowRate = 1;
    mLayRate = 1;
    updateDataSubsample(false);
    applySubsample();
}

void VectorDialog::defaultAppearance()
{
    mScaleFactor  = doc->vectorScaleFactor();
    mShowGlyph    = false;
    mColorOption  = 0;
    mLogTransform = false;
    mLineWidth    = 1;
    updateDataAppearance(false);

    vtkNew<vtkNamedColors> colors;
    doc->setVectorColor(colors->GetColor3d("Black"));
    doc->activateVectorGlyph(false);
}

void VectorDialog::defaultThreshold()
{
    double range[2];
    doc->vectorMagnitudeRange(range);
    mVectorMin = range[0];
    mVectorMax = range[1];
    mApplyThreshold = false;
    updateDataThreshold(false);
    applyThreshold();
}

void VectorDialog::defaultCrop()
{
    defaultCropValues();
    doc->cropVectors(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
}

void VectorDialog::defaultCropValues()
{
    mXMin      = 0.0;
    mXMax      = 1.0;
    mXDelta    = 0.1;
    mYMin      = 0.0;
    mYMax      = 1.0;
    mYDelta    = 0.1;
    mZMin      = 0.0;
    mZMax      = 1.0;
    mZDelta    = 0.1;
    mCropAngle = 0;

    ui->doubleSpinBoxXMin->setValue(mXMin);
    ui->doubleSpinBoxXMax->setValue(mXMax);
    ui->doubleSpinBoxYMin->setValue(mYMin);
    ui->doubleSpinBoxYMax->setValue(mYMax);
    ui->doubleSpinBoxZMin->setValue(mZMin);
    ui->doubleSpinBoxZMax->setValue(mZMax);

    ui->doubleSpinBoxXMin->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
    ui->doubleSpinBoxXMax->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
    ui->doubleSpinBoxYMin->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
    ui->doubleSpinBoxYMax->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
    ui->doubleSpinBoxZMin->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));
    ui->doubleSpinBoxZMax->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));

    ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
    ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
    ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
    ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
    ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
    ui->doubleSpinBoxZMax->setSingleStep(mZDelta);

    ui->lineEditXDelta->setText(QString(tr("%1")).arg(mXDelta, 0, 'g'));
    ui->lineEditYDelta->setText(QString(tr("%1")).arg(mYDelta, 0, 'g'));
    ui->lineEditZDelta->setText(QString(tr("%1")).arg(mZDelta, 0, 'g'));

    ui->spinBoxCropAngle->setValue(mCropAngle);

    updateDataCrop(false);
}

void VectorDialog::setCurrentTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}
