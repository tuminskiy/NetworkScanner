#include <QApplication>

#include "nscanclient/nscanclient.hpp"
#include "gui/window/connectdialog.hpp"
#include "gui/window/mainwindow.hpp"

#include <memory>

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  auto client = std::make_shared<swatcher::NscanClient>();

  gui::ConnectDialog cdialog(client);
  gui::MainWindow mwindow(client);

  QObject::connect(&cdialog, &gui::ConnectDialog::connected,
                   &mwindow, &gui::MainWindow::connected);

  cdialog.open();

  return app.exec();
}