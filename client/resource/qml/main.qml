import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3
import swatcher.NscanClient 1.0

ApplicationWindow {
  id: appWin
  visible: false

  width: 500
  height: 500

  Connections {
    target: nscanClient
    function onFailed(message) {
      msgBox.text = message;
      msgBox.open();
    }
  }

  Connections {
    target: nscanClient
    function onConnected() {
      connectDialog.close();
      appWin.show();
    }
  }

  Connections {
    target: connectDialog
    function onConnectClicked() {
      nscanClient.connect(connectDialog.host + ":" + connectDialog.port);
    }
  }

  NscanClient { id: nscanClient }

  MessageDialog {
    id: msgBox
    title: "Error"
  }

  ConnectDialog {
    id: connectDialog

    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
  }

  Component.onCompleted: connectDialog.show();
}
