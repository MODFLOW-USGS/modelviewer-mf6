#include "animationdialog.h"
#include "ui_animationdialog.h"

#include "mvdoc.h"

#include <QDebug>
#include <QMessageBox>
#include <QDoubleValidator>

AnimationDialog::AnimationDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::AnimationDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);

    // Controls
    animationType = AnimationType::atTime;
    numberOfSteps = 1;

    // continuously fire clicked events when mouse is held down
    ui->pushButtonAdvance->setAutoRepeat(true);

    ui->spinBoxNumberOfSteps->setRange(1, 10000000);

    connect(ui->spinBoxNumberOfSteps, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                numberOfSteps = val;
                doc->setAnimationSteps(numberOfSteps);
            });
    connect(ui->pushButtonRun, &QAbstractButton::clicked, this, &AnimationDialog::onRun);
    connect(ui->comboBoxAnimationType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, QOverload<int>::of(&AnimationDialog::onChangeComboBoxAnimationType));
    connect(ui->pushButtonAdvance, &QAbstractButton::clicked,
            [=]() {
                AnimationType at = doc->animationType();
                switch (at)
                {
                case AnimationType::atTime:
                    doc->advanceOneTimePoint();
                    break;
                case AnimationType::atSpace:
                    doc->updateAnimationWithSameTime();
                    break;
                }
            });
    connect(ui->pushButtonStop, &QAbstractButton::clicked,
            [=]() {
                ui->pushButtonStop->setEnabled(false);
                doc->stopAnimation();
            });
    connect(ui->pushButtonSet, &QAbstractButton::clicked,
            [=]() {
                assert(ui->comboBoxTimePoints->currentIndex() >= 0);
                doc->setTimePointTo(ui->comboBoxTimePoints->currentIndex());
            });

    // Options
    delay   = 0;
    rotate  = 0;
    elevate = 0;

    ui->lineEditDelay->setValidator(doubleValidator);
    ui->lineEditRotate->setValidator(doubleValidator);
    ui->lineEditElevate->setValidator(doubleValidator);

    // tab
    connect(ui->tabWidget, QOverload<int>::of(&QTabWidget::currentChanged), this, &AnimationDialog::onTabChanged);

    // buttons
    connect(ui->pushButtonDone, &QAbstractButton::clicked, this, &QWidget::hide);
}

AnimationDialog::~AnimationDialog()
{
    delete ui;
    delete _geometry;
}

void AnimationDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void AnimationDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void AnimationDialog::activate(bool b)
{
    // Controls
    activateControls(b);

    // Options
    activateOptions(b);
}

void AnimationDialog::activateControls(bool b)
{
    // no-op
}

void AnimationDialog::activateOptions(bool b)
{
#if !defined(DONT_DISABLE_LABELS)
    ui->labelDelay->setEnabled(b);
    ui->labelDelayUnits->setEnabled(b);
    ui->groupBoxViewpoint->setEnabled(b);

    static QPalette original = ui->groupBoxViewpoint->palette();
    if (b)
    {
        ui->groupBoxViewpoint->setPalette(original);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Disabled, QPalette::WindowText,
                         QApplication::palette().color(QPalette::Disabled, QPalette::WindowText));
        ui->groupBoxViewpoint->setPalette(palette);
    }
#endif

    ui->lineEditDelay->setEnabled(b);
    ui->lineEditRotate->setEnabled(b);
    ui->lineEditElevate->setEnabled(b);
}

void AnimationDialog::enableRun(bool b)
{
    ui->pushButtonRun->setEnabled(b);
}

void AnimationDialog::enableStop(bool b)
{
    ui->pushButtonStop->setEnabled(b);
}

void AnimationDialog::enableAdvance(bool b)
{
    ui->pushButtonAdvance->setEnabled(b);
}

void AnimationDialog::enableSet(bool b)
{
    ui->pushButtonSet->setEnabled(b);
}

void AnimationDialog::enableTimePointsCombo(bool b)
{
    ui->comboBoxTimePoints->setEnabled(b);
}

void AnimationDialog::reinitialize()
{
    // Controls
    reinitializeControls();

    // Options
    reinitializeOptions();
}

void AnimationDialog::reinitializeControls()
{
    timePointLabels.clear();
    ui->labelCurrentTime->clear();
    reset();
}

void AnimationDialog::reinitializeOptions()
{
    delay   = 0;
    rotate  = 0;
    elevate = 0;
    updateDataOptions(false);
    activateOptions(false);
}

void AnimationDialog::setCurrentTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void AnimationDialog::reset()
{
    ui->comboBoxTimePoints->clear();
    ui->comboBoxTimePoints->addItems(timePointLabels);

    if (timePointLabels.size() <= 1)
    {
        ui->pushButtonRun->setEnabled(false);
        ui->pushButtonAdvance->setEnabled(false);
        ui->pushButtonStop->setEnabled(false);
        ui->pushButtonSet->setEnabled(false);
        ui->comboBoxAnimationType->setEnabled(false);
        ui->spinBoxNumberOfSteps->setEnabled(false);
    }
    else
    {
        ui->pushButtonStop->setEnabled(false);
        ui->pushButtonSet->setEnabled(true);
        bool b = (ui->comboBoxTimePoints->currentIndex() < timePointLabels.size() - 1);
        ui->pushButtonRun->setEnabled(b);
        ui->pushButtonAdvance->setEnabled(b);
        ui->pushButtonAdvance->setEnabled(b);
        ui->comboBoxAnimationType->setEnabled(true);
    }

#if !defined(DONT_DISABLE_LABELS)
    //ui->labelTimePoints->setEnabled(false);
    //ui->labelCurrent->setEnabled(false);

    ui->labelAnimationType->setEnabled(ui->comboBoxAnimationType->isEnabled());
    ui->labelNumberOfSteps->setEnabled(ui->spinBoxNumberOfSteps->isEnabled());
#endif


    switch (static_cast<AnimationType>(ui->comboBoxAnimationType->currentIndex()))
    {
    case AnimationType::atTime:
        ui->spinBoxNumberOfSteps->setEnabled(false);
        break;
    case AnimationType::atSpace:
        ui->spinBoxNumberOfSteps->setEnabled(true);
        break;
    default:
        assert(false);
        ui->spinBoxNumberOfSteps->setEnabled(false);
        break;
    }
}

void AnimationDialog::setAndDisplayCurrentTime(int timePointIndex)
{
    ui->labelCurrentTime->setText(timePointLabels[timePointIndex]);
}

void AnimationDialog::setTimePointsLabel(QString label)
{
    ui->labelTimePoints->setText(label);
}

void AnimationDialog::setCurrentTimeLabel(QString label)
{
    ui->labelCurrent->setText(label);
}

void AnimationDialog::setAnimationType(AnimationType animationType)
{
    ui->comboBoxAnimationType->setCurrentIndex(static_cast<int>(animationType));
}

bool AnimationDialog::updateDataControls(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // numberOfSteps
        numberOfSteps = ui->spinBoxNumberOfSteps->value();
    }
    else
    {
        // numberOfSteps
        ui->spinBoxNumberOfSteps->setValue(numberOfSteps);

        // animationType
        doAnimationTypeChange();
    }
    return true;
}

bool AnimationDialog::updateDataOptions(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // delay
        s   = ui->lineEditDelay->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditDelay->setFocus();
            return false;
        }
        delay = val;

        // elevate
        s     = ui->lineEditElevate->text();
        val   = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditElevate->setFocus();
            return false;
        }
        elevate = val;

        // rotate
        s       = ui->lineEditRotate->text();
        val     = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditRotate->setFocus();
            return false;
        }
        rotate = val;
    }
    else
    {
        // delay
        ui->lineEditDelay->setText(QString("%1").number(delay, 'g'));

        // elevate
        ui->lineEditElevate->setText(QString("%1").number(elevate, 'g'));

        // rotate
        ui->lineEditRotate->setText(QString("%1").number(rotate, 'g'));
    }
    return true;
}

void AnimationDialog::onChangeComboBoxAnimationType(int index)
{
    animationType = static_cast<AnimationType>(index);
    //doAnimationTypeChange();

    bool canRun   = ui->comboBoxTimePoints->currentIndex() < ui->comboBoxTimePoints->count() - 1;
    switch (animationType)
    {
    case AnimationType::atTime:
        ui->labelNumberOfSteps->setEnabled(false);
        ui->spinBoxNumberOfSteps->setEnabled(false);
        doc->setAnimationType(AnimationType::atTime);
        break;
    case AnimationType::atSpace:
        canRun = true;
        ui->labelNumberOfSteps->setEnabled(true);
        ui->spinBoxNumberOfSteps->setEnabled(true);
        doc->setAnimationType(AnimationType::atSpace);
        break;
    default:
        assert(false);
        break;
    }
    ui->pushButtonRun->setEnabled(canRun);
    ui->pushButtonAdvance->setEnabled(canRun);
}

void AnimationDialog::doAnimationTypeChange()
{
    switch (animationType)
    {
    case AnimationType::atTime:
        ui->labelNumberOfSteps->setEnabled(false);
        ui->spinBoxNumberOfSteps->setEnabled(false);
        break;
    case AnimationType::atSpace:
        ui->labelNumberOfSteps->setEnabled(true);
        ui->spinBoxNumberOfSteps->setEnabled(true);
        break;
    default:
        assert(false);
        break;
    }
}

void AnimationDialog::onRun()
{
    ui->pushButtonRun->setEnabled(false);
    ui->pushButtonAdvance->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonSet->setEnabled(false);
    ui->comboBoxTimePoints->setEnabled(false);

    activateOptions(false);
    doc->startAnimation();
}

void AnimationDialog::onTabChanged(int idx)
{
    switch (idx)
    {
    case 0:
        updateDataOptions(true);
        break;
    case 1:
        break;
    default:
        break;
    }
}
