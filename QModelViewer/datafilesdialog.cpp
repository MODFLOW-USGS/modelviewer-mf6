#include "datafilesdialog.h"

#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
///#include <direct.h>
#include "mvUtil.h"


#include "modflow6datafilesdialog.h"

#include "Modflow6DataSource.h"

//using std::ifstream;
//using std::ofstream;

#include <QDir>
//#include <QWidget.h>


char *DataFilesDialog::GetDataFileList(char *model, QWidget *parent /* = nullptr */)
{
    if (mvUtil::stricmp(model, Modflow6DataSource::GetNameStatic()) == 0)
    {
        return GetModflow6DataFiles(parent);
    }

    return nullptr;
}

char *DataFilesDialog::GetModflow6DataFiles(QWidget *parent /* = nullptr */)
{
    //CModflow6DataFilesDlg dlg(::AfxGetMainWnd());
    //if (dlg.DoModal() != IDOK)
    //{
    //    return NULL;
    //}

    //// Store fullpaths and convert to relative when Serializing
    //BOOL  setdir = FALSE;
    //char *dataFileList;
    //if (dlg.m_FileSpecificationMethod == 0)
    //{
    //    CString name = dlg.m_GwfModelNameFile;
    //    int     pos  = name.ReverseFind('\\');
    //    if (pos != -1)
    //    {
    //        _chdir((LPCTSTR)name.Left(pos));
    //        setdir = TRUE;
    //    }
    //    dataFileList = new char[name.GetLength() + 20];
    //    strcpy(dataFileList, (LPCTSTR)name);
    //    strcat(dataFileList, "\n");
    //}
    //else
    //{
    //    CString gridFile = dlg.m_GridFile;
    //    int     pos      = gridFile.ReverseFind('\\');
    //    if (pos != -1)
    //    {
    //        _chdir((LPCTSTR)gridFile.Left(pos));
    //        setdir = TRUE;
    //    }
    //    CString headOrConcFile = dlg.m_HeadOrConcFile;
    //    CString budgetFile     = dlg.m_BudgetFile;

    //    dataFileList           = new char[gridFile.GetLength() + headOrConcFile.GetLength() + budgetFile.GetLength() + 20];
    //    // no name file
    //    strcpy(dataFileList, "\n");
    //    strcat(dataFileList, (LPCTSTR)gridFile);
    //    strcat(dataFileList, "\n");
    //    strcat(dataFileList, (LPCTSTR)headOrConcFile);
    //    strcat(dataFileList, "\n");
    //    strcat(dataFileList, (LPCTSTR)budgetFile);
    //    strcat(dataFileList, "\n");
    //}

    //return dataFileList;



    Modflow6DataFilesDialog dlg(parent);

    if (dlg.exec() != QDialog::Accepted)
    {
        return nullptr;
    }

    // Store fullpaths and convert to relative when Serializing
    char *dataFileList = nullptr;
    if (dlg.specifyNameFile)
    {
        QString name = dlg.gwfModelNameFile;
        // int     pos  = name.ReverseFind('\\');
        int     pos  = name.lastIndexOf(QDir::separator());
        if (pos != -1)
        {
            //_chdir(name.left(pos).toLocal8Bit().data());
            QDir::setCurrent(name.left(pos));
        }
        dataFileList = new char[name.size() + 20];
        strcpy(dataFileList, name.toLocal8Bit().data());
        strcat(dataFileList, "\n");
    }
    else
    {
        QString gridFile = dlg.gridFile;
        int     pos      = gridFile.lastIndexOf(QDir::separator());
        if (pos != -1)
        {
            QDir::setCurrent(gridFile.left(pos));
        }
        QString headOrConcFile = dlg.headOrConcFile;
        QString budgetFile     = dlg.budgetFile;

        dataFileList           = new char[gridFile.size() + headOrConcFile.size() + budgetFile.size() + 20];
        // no name file
        strcpy(dataFileList, "\n");
        strcat(dataFileList, gridFile.toLocal8Bit().data());
        strcat(dataFileList, "\n");
        strcat(dataFileList, headOrConcFile.toLocal8Bit().data());
        strcat(dataFileList, "\n");
        strcat(dataFileList, budgetFile.toLocal8Bit().data());
        strcat(dataFileList, "\n");
    }
    return dataFileList;
}
