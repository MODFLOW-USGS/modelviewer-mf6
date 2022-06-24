#include "ModelViewer.h"
#include "DataFilesDialog.h"
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>

#include "Modflow6DataFilesDlg.h"

#include "Modflow6DataSource.h"

using std::ifstream;
using std::ofstream;

char *DataFilesDialog::GetDataFileList(char *model)
{
    if (stricmp(model, Modflow6DataSource::GetNameStatic()) == 0)
    {
        return GetModflow6DataFiles();
    }

    return NULL;
}

char *DataFilesDialog::GetModflow6DataFiles()
{
    CModflow6DataFilesDlg dlg(::AfxGetMainWnd());
    if (dlg.DoModal() != IDOK)
    {
        return NULL;
    }

    // Store fullpaths and convert to relative when Serializing
    BOOL  setdir = FALSE;
    char *dataFileList;
    if (dlg.m_FileSpecificationMethod == 0)
    {
        CString name = dlg.m_GwfModelNameFile;
        int     pos  = name.ReverseFind('\\');
        if (pos != -1)
        {
            _chdir((LPCTSTR)name.Left(pos));
            setdir = TRUE;
        }
        dataFileList = new char[name.GetLength() + 20];
        strcpy(dataFileList, (LPCTSTR)name);
        strcat(dataFileList, "\n");
    }
    else
    {
        CString gridFile = dlg.m_GridFile;
        int     pos      = gridFile.ReverseFind('\\');
        if (pos != -1)
        {
            _chdir((LPCTSTR)gridFile.Left(pos));
            setdir   = TRUE;
        }
        CString headOrConcFile = dlg.m_HeadOrConcFile;
        CString budgetFile     = dlg.m_BudgetFile;

        dataFileList           = new char[gridFile.GetLength() + headOrConcFile.GetLength() + budgetFile.GetLength() + 20];
        // no name file
        strcpy(dataFileList, "\n");
        strcat(dataFileList, (LPCTSTR)gridFile);
        strcat(dataFileList, "\n");
        strcat(dataFileList, (LPCTSTR)headOrConcFile);
        strcat(dataFileList, "\n");
        strcat(dataFileList, (LPCTSTR)budgetFile);
        strcat(dataFileList, "\n");
    }
    /*
        CString path = dlg.m_PathFile;
        pos = path.ReverseFind('\\');
        if (pos != -1)
        {
            if (!setdir)
            {
                _chdir((LPCTSTR) path.Left(pos));
                setdir = TRUE;
            }
            path = path.Right(path.GetLength() - pos - 1);
        }

        CString external = dlg.m_ExternalFile;
        pos = external.ReverseFind('\\');
        if (pos != -1)
        {
            if (!setdir)
            {
                _chdir((LPCTSTR) external.Left(pos));
                setdir = TRUE;
            }
            external = external.Right(external.GetLength() - pos - 1);
        }
    */

    /*
       char dataCode[10];
        sprintf(dataCode, "%d\n", dlg.m_DataType);
        strcat(dataFileList, dataCode);

        if (dlg.m_PathlineBackwards)
        {
            strcat(dataFileList, "1");
        }
        else
        {
            strcat(dataFileList, "0");
        }
        strcat(dataFileList, "\n");
    */

    return dataFileList;
}
