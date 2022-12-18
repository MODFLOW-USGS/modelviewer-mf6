#ifndef LIGHTINGDIALOG_H
#define LIGHTINGDIALOG_H

#include <QDialog>

namespace Ui {
class LightingDialog;
}

class MvDoc;
class QLabel;

class LightingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LightingDialog(QWidget *parent, MvDoc *doc);
    ~LightingDialog();

    void hideEvent(QHideEvent *event) override;
    void showEvent(QShowEvent *e) override;

    void activate(bool b);
    void onDefault();
    void reinitialize();
    void setCurrentIndex(int index);
    void setDefaultEnabled(bool enabled);


    // Lights
    void activateLights(bool b);
    void applyAuxiliaryLightPosition();
    void onDefaultLights();
    void reinitializeLights();
    bool updateDataLights(bool saveAndValidate);
    void updateAuxiliaryLightIntensity();
    void updateLabelsLights();
    void updateAuxiliaryLightPosition();
    void updateHeadlightIntensity();

    bool headlightOn;
    bool auxiliaryLightOn;
    int  headlightIntensity;
    int  auxiliaryLightIntensity;
    int  dirX;
    int  dirY;
    int  dirZ;


    // Surface
    void activateSurface(bool b);
    void onDefaultSurface();
    void reinitializeSurface();
    bool updateDataSurface(bool saveAndValidate);
    void updateLabelsSurface();

    int  diffuse;
    int  ambient;
    int  specular;
    int  specularPower;


    // Background
    void activateBackground(bool b);
    void onDefaultBackground();
    void reinitializeBackground();
    bool updateDataBackground(bool saveAndValidate);
    void updateLabelsBackground();

    bool customBackground;
    int  red;
    int  green;
    int  blue;

public Q_SLOTS:
    void onHeadlight();
    void onAuxiliaryLight();

private:
    void updateIntensity(QLabel *label, int intensity);

private:
    Ui::LightingDialog *ui;
    MvDoc *             doc;
    QRect *             _geometry;
};

#endif // LIGHTINGDIALOG_H
