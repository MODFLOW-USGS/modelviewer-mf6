#include <cstdint>

#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QIcon>
#include <QSurfaceFormat>

#include <QVTKOpenGLNativeWidget.h>

#include "mainwindow.h"

int main(int argc, char** argv)
{
#if defined(_DEBUG)
  int* leak1968 = (int*)malloc(1968);
#endif

  // needed to ensure appropriate OpenGL context is created for VTK rendering.
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);
  app.setOrganizationName("USGS");
  app.setApplicationName("Model Viewer for Modflow 6");
  app.setOrganizationDomain("gov.usgs");
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

  return ret;
}
