#include "lightingdialog.h"
#include "ui_lightingdialog.h"

#include <QMessageBox>

#include "mvdoc.h"


LightingDialog::LightingDialog(QWidget *parent, MvDoc *doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::LightingDialog)
    , doc(doc)
    , _geometry(nullptr)
{
    ui->setupUi(this);

    // Lights
    ui->sliderHeadlightIntensity->setRange(0, 100);
    ui->sliderAuxiliaryLightIntensity->setRange(0, 100);

    ui->sliderX->setRange(0, 20);
    ui->sliderY->setRange(0, 20);
    ui->sliderZ->setRange(0, 20);

    reinitializeLights();

    connect(ui->checkBoxHeadlight, QOverload<int>::of(&QCheckBox::stateChanged), this, &LightingDialog::onHeadlight);

    connect(ui->checkBoxAuxiliaryLight, QOverload<int>::of(&QCheckBox::stateChanged), this, &LightingDialog::onAuxiliaryLight);

    connect(ui->sliderHeadlightIntensity, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                headlightIntensity = val;
                doc->setHeadlightIntensity(headlightIntensity * 0.01);
                ui->labelHeadlightIntensity->setText(QString(tr("%1")).arg(headlightIntensity * 0.01, 0, 'f', 2));
            });
    connect(ui->sliderAuxiliaryLightIntensity, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                auxiliaryLightIntensity = val;
                doc->setAuxiliaryLightIntensity(auxiliaryLightIntensity * 0.01);
                ui->labelAuxiliaryLightIntensity->setText(QString(tr("%1")).arg(auxiliaryLightIntensity * 0.01, 0, 'f', 2));
            });
    connect(ui->sliderX, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                dirX = val;
                updateAuxiliaryLightPosition();
                applyAuxiliaryLightPosition();
            });
    connect(ui->sliderY, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                dirY = val;
                updateAuxiliaryLightPosition();
                applyAuxiliaryLightPosition();
            });
    connect(ui->sliderZ, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                dirZ = val;
                updateAuxiliaryLightPosition();
                applyAuxiliaryLightPosition();
            });

    // Surface
    diffuse       = 100;
    ambient       = 0;
    specular      = 0;
    specularPower = 1;

    ui->sliderDiffuse->setRange(0, 100);            // vtkSetClampMacro(Diffuse, double, 0.0, 1.0);
    ui->sliderAmbient->setRange(0, 100);            // vtkSetClampMacro(Ambient, double, 0.0, 1.0);
    ui->sliderSpecular->setRange(0, 100);           // vtkSetClampMacro(Specular, double, 0.0, 1.0);
    ui->sliderSpecularPower->setRange(0, 128);      // vtkSetClampMacro(SpecularPower, double, 0.0, 128.0);

    // in pahsieh@usgs.gov code the diffuse parameter is hard-coded to 1.0
    // and the slider is disabled (see CSurfacePage::OnInitDialog)
    ui->sliderDiffuse->setEnabled(false);

    connect(ui->sliderAmbient, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                ambient = val;
                doc->setAmbientLighting(ambient * 0.01);
                updateLabelsSurface();
            });
    connect(ui->sliderDiffuse, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                diffuse = val;
                doc->setDiffuseLighting(diffuse * 0.01);
                updateLabelsSurface();
            });
    connect(ui->sliderSpecular, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                specular = val;
                doc->setSpecularLighting(specular * 0.01);
                updateLabelsSurface();
            });
    connect(ui->sliderSpecularPower, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                specularPower = val;
                doc->setSpecularPower(specularPower * 1.0);
                updateLabelsSurface();
            });

    // Background
    red   = 100;
    green = 100;
    blue  = 100;

    ui->sliderRed->setRange(0, 100);
    ui->sliderGreen->setRange(0, 100);
    ui->sliderBlue->setRange(0, 100);

    connect(ui->radioButtonWhite, &QAbstractButton::clicked,
            [=]() {
                customBackground = false;
                ui->sliderRed->setEnabled(false);
                ui->sliderGreen->setEnabled(false);
                ui->sliderBlue->setEnabled(false);
                doc->setBackgroundColor(1, 1, 1);
            });
    connect(ui->radioButtonCustom, &QAbstractButton::clicked,
            [=]() {
                customBackground = true;
                ui->sliderRed->setEnabled(true);
                ui->sliderGreen->setEnabled(true);
                ui->sliderBlue->setEnabled(true);
                doc->setBackgroundColor(red * 0.01, blue * 0.01, green * 0.01);
            });
    connect(ui->sliderRed, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                red = val;
                doc->setBackgroundColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsBackground();
            });
    connect(ui->sliderGreen, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                green = val;
                doc->setBackgroundColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsBackground();
            });
    connect(ui->sliderBlue, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                blue = val;
                doc->setBackgroundColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsBackground();
            });

    // buttons
    ui->pushButtonDefault->setEnabled(false);
    connect(ui->pushButtonDefault, &QAbstractButton::clicked, this, &LightingDialog::onDefault);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

LightingDialog::~LightingDialog()
{
    delete ui;
    delete _geometry;
}

void LightingDialog::hideEvent(QHideEvent *e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void LightingDialog::showEvent(QShowEvent *e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void LightingDialog::activate(bool b)
{
    // Lights
    activateLights(b);

    // Surface
    activateSurface(b);

    // Background
    activateBackground(b);
}

void LightingDialog::activateLights(bool b)
{
    ui->checkBoxHeadlight->setEnabled(b);
    ui->checkBoxAuxiliaryLight->setEnabled(b);
    ui->sliderHeadlightIntensity->setEnabled(b && ui->checkBoxHeadlight->isChecked());
    bool bb = ui->checkBoxAuxiliaryLight->isChecked();
    ui->sliderAuxiliaryLightIntensity->setEnabled(b && bb);
    ui->sliderX->setEnabled(b && bb);
    ui->sliderY->setEnabled(b && bb);
    ui->sliderZ->setEnabled(b && bb);
}

void LightingDialog::reinitialize()
{
    reinitializeLights();
    reinitializeSurface();
    reinitializeBackground();
    ui->pushButtonDefault->setEnabled(false);
}


bool LightingDialog::updateDataLights(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(headlightOn == ui->checkBoxHeadlight->isChecked());
        assert(auxiliaryLightOn == ui->checkBoxAuxiliaryLight->isChecked());

        assert(headlightIntensity == ui->sliderHeadlightIntensity->value());
        assert(auxiliaryLightIntensity == ui->sliderAuxiliaryLightIntensity->value());

        assert(dirX == ui->sliderX->value());
        assert(dirY == ui->sliderY->value());
        assert(dirZ == ui->sliderZ->value());
    }
    else
    {
        ui->checkBoxHeadlight->setChecked(headlightOn);
        ui->checkBoxAuxiliaryLight->setChecked(auxiliaryLightOn);

        ui->sliderHeadlightIntensity->setValue(headlightIntensity);
        ui->sliderAuxiliaryLightIntensity->setValue(auxiliaryLightIntensity);
        ui->sliderX->setValue(dirX);
        ui->sliderY->setValue(dirY);
        ui->sliderZ->setValue(dirZ);

        updateHeadlightIntensity();
        updateAuxiliaryLightIntensity();
        updateAuxiliaryLightPosition();
    }
    return true;
}

void LightingDialog::updateIntensity(QLabel *label, int intensity)
{
    label->setText(QString("%1").arg(intensity * 0.01, 0, 'f', 2));
}

void LightingDialog::updateHeadlightIntensity()
{
    updateIntensity(ui->labelHeadlightIntensity, headlightIntensity);
}

void LightingDialog::updateAuxiliaryLightIntensity()
{
    updateIntensity(ui->labelAuxiliaryLightIntensity, auxiliaryLightIntensity);
}

void LightingDialog::updateAuxiliaryLightPosition()
{
    ui->labelAuxiliaryLightPosition->setText(QString(tr("(%1, %2, %3)")).arg((dirX - 10) * 0.1, 0, 'f', 1).arg((dirY - 10) * 0.1, 0, 'f', 1).arg((dirZ - 10) * 0.1, 0, 'f', 1));
}

void LightingDialog::applyAuxiliaryLightPosition()
{
    doc->setAuxiliaryLightPosition((dirX - 10) * 0.1, (dirY - 10) * 0.1, (dirZ - 10) * 0.1);
}

bool LightingDialog::updateDataBackground(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(customBackground == ui->radioButtonCustom->isChecked());
        assert(!customBackground == ui->radioButtonWhite->isChecked());
        assert(red   == ui->sliderRed->value());
        assert(green == ui->sliderGreen->value());
        assert(blue  == ui->sliderBlue->value());
    }
    else
    {
        if (customBackground)
        {
            ui->radioButtonCustom->setChecked(true);
        }
        else
        {
            ui->radioButtonWhite->setChecked(true);
        }
        ui->sliderRed->setValue(red);
        ui->sliderGreen->setValue(green);
        ui->sliderBlue->setValue(blue);
        ui->labelRed->setText(QString(tr("%1")).arg(red / 100.0, 0, 'f', 2));
        ui->labelGreen->setText(QString(tr("%1")).arg(green / 100.0, 0, 'f', 2));
        ui->labelBlue->setText(QString(tr("%1")).arg(blue / 100.0, 0, 'f', 2));
    }
    return true;
}

void LightingDialog::onDefault()
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
    case 0: // Lights
        onDefaultLights();
        break;
    case 1: // Surface
        onDefaultSurface();
        break;
    case 2: // Background
        onDefaultBackground();
        break;
    default:
        assert(false);
        break;
    }
}

void LightingDialog::onDefaultLights()
{
    headlightOn             = true;
    auxiliaryLightOn        = false;
    headlightIntensity      = 100;
    auxiliaryLightIntensity = 0;
    dirX                    = 10;
    dirY                    = 10;
    dirZ                    = 20;
    updateDataLights(false);
    onHeadlight();
    onAuxiliaryLight();
    updateLabelsLights();
    doc->setHeadlightIntensity(headlightIntensity * 0.01);
    doc->setAuxiliaryLightIntensity(auxiliaryLightIntensity * 0.01);
    applyAuxiliaryLightPosition();
}

void LightingDialog::onDefaultSurface()
{
    diffuse       = 100;
    ambient       = 0;
    specular      = 0;
    specularPower = 1;
    updateDataSurface(false);
    updateLabelsSurface();

    doc->setDiffuseLighting(diffuse * 0.01);
    doc->setAmbientLighting(ambient * 0.01);
    doc->setSpecularLighting(specular * 0.01);
    doc->setSpecularPower(specularPower * 1.0);
}

void LightingDialog::onDefaultBackground()
{
    red        = 100;
    green      = 100;
    blue       = 100;
    customBackground = false;
    updateDataBackground(false);
    updateLabelsBackground();
    ui->sliderRed->setEnabled(false);
    ui->sliderGreen->setEnabled(false);
    ui->sliderBlue->setEnabled(false);
    doc->setBackgroundColor(1, 1, 1);
}

void LightingDialog::onHeadlight()
{
    bool b = ui->checkBoxHeadlight->isChecked();
    ui->sliderHeadlightIntensity->setEnabled(b);
    doc->switchOnHeadlight(b);
}

void LightingDialog::onAuxiliaryLight()
{
    bool b = ui->checkBoxAuxiliaryLight->isChecked();
    ui->sliderAuxiliaryLightIntensity->setEnabled(b);
    ui->sliderX->setEnabled(b);
    ui->sliderY->setEnabled(b);
    ui->sliderZ->setEnabled(b);
    doc->switchOnAuxiliaryLight(b);
}

void LightingDialog::reinitializeLights()
{
    auxiliaryLightIntensity = 0;
    auxiliaryLightOn        = false;
    headlightIntensity      = 100;
    headlightOn             = true;
    dirX                    = 10;
    dirY                    = 10;
    dirZ                    = 20;
    updateDataLights(false);
    updateLabelsLights();
    activateLights(false);
}

void LightingDialog::reinitializeSurface()
{
    auxiliaryLightIntensity = 0;
    auxiliaryLightOn        = false;
    headlightIntensity      = 100;
    headlightOn             = true;
    dirX                    = 10;
    dirY                    = 10;
    dirZ                    = 20;
    updateDataLights(false);
    updateLabelsSurface();
    activateLights(false);
}

void LightingDialog::reinitializeBackground()
{
    red              = 100;
    green            = 100;
    blue             = 100;
    customBackground = false;
    updateDataBackground(false);
    updateLabelsBackground();
    activateBackground(false);
}

void LightingDialog::activateBackground(bool b)
{
    ui->radioButtonWhite->setEnabled(b);
    ui->radioButtonCustom->setEnabled(b);
    ui->sliderRed->setEnabled(b && customBackground);
    ui->sliderBlue->setEnabled(b && customBackground);
    ui->sliderGreen->setEnabled(b && customBackground);
}

void LightingDialog::activateSurface(bool b)
{
    // ui->sliderDiffuse->setEnabled(b);  -- see CSurfacePage::Activate
    ui->sliderAmbient->setEnabled(b);
    ui->sliderSpecular->setEnabled(b);
    ui->sliderSpecularPower->setEnabled(b);
}

void LightingDialog::setCurrentIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void LightingDialog::setDefaultEnabled(bool enabled)
{
    ui->pushButtonDefault->setEnabled(enabled);
}

bool LightingDialog::updateDataSurface(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(diffuse       == ui->sliderDiffuse->value());
        assert(ambient       == ui->sliderAmbient->value());
        assert(specular      == ui->sliderSpecular->value());
        assert(specularPower == ui->sliderSpecularPower->value());
    }
    else
    {
        ui->sliderAmbient->setValue(ambient);
        ui->sliderDiffuse->setValue(diffuse);
        ui->sliderSpecular->setValue(specular);
        ui->sliderSpecularPower->setValue(specularPower);
    }
    return true;
}

void LightingDialog::updateLabelsSurface()
{
    ui->labelDiffuse->setText(QString(tr("%1")).arg(diffuse * 0.01, 0, 'f', 2));
    ui->labelAmbient->setText(QString(tr("%1")).arg(ambient * 0.01, 0, 'f', 2));
    ui->labelSpecular->setText(QString(tr("%1")).arg(specular * 0.01, 0, 'f', 2));
    ui->labelSpecularPower->setText(QString(tr("%1")).arg(specularPower * 1.0, 0, 'f', 0));
}

void LightingDialog::updateLabelsBackground()
{
    ui->labelRed->setText(QString(tr("%1")).arg(red / 100.0, 0, 'f', 2));
    ui->labelGreen->setText(QString(tr("%1")).arg(green / 100.0, 0, 'f', 2));
    ui->labelBlue->setText(QString(tr("%1")).arg(blue / 100.0, 0, 'f', 2));
}

void LightingDialog::updateLabelsLights()
{
    updateAuxiliaryLightPosition();
    updateHeadlightIntensity();
    updateAuxiliaryLightIntensity();
}
