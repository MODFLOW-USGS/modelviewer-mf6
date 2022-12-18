#ifndef BITMAPRESOLUTIONDIALOG_H
#define BITMAPRESOLUTIONDIALOG_H

#include <QDialog>

namespace Ui {
class BitmapResolutionDialog;
}

enum class ResolutionType {
    rtScreen,
    rt150ppi,
    rt300ppi
};

enum class SideType {
    stWidth,
    stHeight
};


class BitmapResolutionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BitmapResolutionDialog(QWidget *parent = nullptr);
    ~BitmapResolutionDialog();

public:
    ResolutionType resolutionOption = ResolutionType::rtScreen;
    SideType       sideOption       = SideType::stWidth;

    int            widthInInches;
    int            heightInInches;

private:
    Ui::BitmapResolutionDialog *ui;

    void                        enableSize(bool enable);
    void                        initDialog();

private slots:
    void onResolutionScreen();
    void onResolution150();
    void onResolution300();
    void onWidthOption();
    void onHeightOption();
};

#endif // BITMAPRESOLUTIONDIALOG_H
