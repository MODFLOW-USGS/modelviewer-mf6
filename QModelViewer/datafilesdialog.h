#ifndef DATAFILESDIALOG_H
#define DATAFILESDIALOG_H

class QWidget;

class DataFilesDialog
{
public:
    static char *GetDataFileList(char *model, QWidget *parent = nullptr);

private:
    static char *GetModflow6DataFiles(QWidget *parent = nullptr);
};

#endif // DATAFILESDIALOG_H
