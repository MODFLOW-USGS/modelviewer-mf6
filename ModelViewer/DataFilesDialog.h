#ifndef __DataFilesDialog_h
#define __DataFilesDialog_h

class DataFilesDialog
{
public:
    static char *GetDataFileList(char *model);

private:
    static char *GetModflow6DataFiles();
};

#endif
