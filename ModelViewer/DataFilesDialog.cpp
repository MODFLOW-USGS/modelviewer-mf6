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
	CModflow6DataFilesDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return NULL;
	}

   // For now, we just use the file name and not the path.
	// TO DO: Determine the path relative to the document file.
	BOOL setdir = FALSE;
   char *dataFileList;
   if (dlg.m_FileSpecificationMethod == 0)
   {
	   CString name = dlg.m_GwfModelNameFile;
	   int pos = name.ReverseFind('\\');
	   if (pos != -1)
	   {
		   _chdir((LPCTSTR) name.Left(pos));
		   setdir = TRUE;
		   name = name.Right(name.GetLength() - pos - 1);
	   }
	   dataFileList = new char[name.GetLength() + 20];
	   strcpy(dataFileList, (LPCTSTR) name);
	   strcat(dataFileList, "\n");
   }
   else
   {
	   CString gridFile = dlg.m_GridFile;
	   int pos = gridFile.ReverseFind('\\');
	   if (pos != -1)
	   {
		   _chdir((LPCTSTR) gridFile.Left(pos));
		   setdir = TRUE;
		   gridFile = gridFile.Right(gridFile.GetLength() - pos - 1);
	   }
	   CString headFile = dlg.m_HeadFile;
	   pos = headFile.ReverseFind('\\');
	   if (pos != -1)
	   {
		   headFile = headFile.Right(headFile.GetLength() - pos - 1);
	   }
	   CString budgetFile = dlg.m_BudgetFile;
	   pos = budgetFile.ReverseFind('\\');
	   if (pos != -1)
	   {
		   budgetFile = budgetFile.Right(budgetFile.GetLength() - pos - 1);
	   }
	   dataFileList = new char[gridFile.GetLength() + headFile.GetLength() + budgetFile.GetLength() + 20];
      // no name file
	   strcpy(dataFileList, "\n");
	   strcat(dataFileList, (LPCTSTR) gridFile);
	   strcat(dataFileList, "\n");
	   strcat(dataFileList, (LPCTSTR) headFile);
	   strcat(dataFileList, "\n");
	   strcat(dataFileList, (LPCTSTR) budgetFile);
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
