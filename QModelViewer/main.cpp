#include <cstdint>

#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QSurfaceFormat>

#include <QVTKOpenGLNativeWidget.h>

#include "mainwindow.h"

#if defined(Q_OS_LINUX)

#include <link.h>

static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
    // char *type;
    const char *type;
    int p_type, j;

    printf("Name: \"%s\" (%d segments)\n", info->dlpi_name,
              info->dlpi_phnum);

    for (j = 0; j < info->dlpi_phnum; j++) {
        p_type = info->dlpi_phdr[j].p_type;
        type =  (p_type == PT_LOAD) ? "PT_LOAD" :
                (p_type == PT_DYNAMIC) ? "PT_DYNAMIC" :
                (p_type == PT_INTERP) ? "PT_INTERP" :
                (p_type == PT_NOTE) ? "PT_NOTE" :
                (p_type == PT_INTERP) ? "PT_INTERP" :
                (p_type == PT_PHDR) ? "PT_PHDR" :
                (p_type == PT_TLS) ? "PT_TLS" :
                (p_type == PT_GNU_EH_FRAME) ? "PT_GNU_EH_FRAME" :
                (p_type == PT_GNU_STACK) ? "PT_GNU_STACK" :
                (p_type == PT_GNU_RELRO) ? "PT_GNU_RELRO" : NULL;

        printf("    %2d: [%14p; memsz:%7lx] flags: 0x%x; ", j,
                (void *) (info->dlpi_addr + info->dlpi_phdr[j].p_vaddr),
                info->dlpi_phdr[j].p_memsz,
                info->dlpi_phdr[j].p_flags);
        if (type != NULL)
            printf("%s\n", type);
        else
            printf("[other (0x%x)]\n", p_type);
    }

    return 0;
}

#endif // defined(Q_OS_LINUX)


int main(int argc, char** argv)
{
#if defined(_DEBUG)
  int* leak1968 = (int*)malloc(1968);
#endif

  qDebug() << "sizeof(unsigned char)"  << sizeof(unsigned char);
  qDebug() << "sizeof(unsigned short)" << sizeof(unsigned short);
  qDebug() << "sizeof(unsigned int)"   << sizeof(unsigned int);
  qDebug() << "sizeof(unsigned long)"  << sizeof(unsigned long);
  qDebug() << "sizeof(std::uint32_t)"  << sizeof(std::uint32_t);
  qDebug() << "sizeof(std::uint64_t)"  << sizeof(std::uint64_t);

  // needed to ensure appropriate OpenGL context is created for VTK rendering.
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);
  app.setOrganizationName("USGS");
  app.setApplicationName("Model Viewer for Modflow 6");
  app.setOrganizationDomain("gov.usgs.ModelViewer");
  QGuiApplication::setApplicationDisplayName(QObject::tr("Model Viewer for Modflow 6"));

  QIcon icon(":/images/ModelViewer-32x32.png");
  icon.addFile(":/images/ModelViewer-16x16.png");
  QGuiApplication::setWindowIcon(icon);

  MainWindow window;
  window.show();

  int ret = app.exec();

#if defined(_DEBUG)
  int* leak2022 = (int*)malloc(2022);
#endif

#if !defined(NDEBUG) && defined(Q_OS_LINUX)
  dl_iterate_phdr(callback, NULL);
#endif

  return ret;
}
