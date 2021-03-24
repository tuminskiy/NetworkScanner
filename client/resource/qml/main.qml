import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ApplicationWindow {
  id: appWin
  visible: false

  width: 500
  height: 500

  ConnectDialog {
    id: connectDialog

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
  }

  Component.onCompleted: connectDialog.show();
}
