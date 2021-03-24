import QtQuick 2.11
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

Window {
  id: root

  signal connectClicked

  width: 400
  height: 170

  maximumWidth: 400
  maximumHeight: 170

  ColumnLayout {
    anchors.fill: parent

    RowLayout {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

      Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        text: "Host:"
        font.pointSize: 16
      }

      TextField {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        placeholderText: "192.168.111.111"
        font.pointSize: 14

        Keys.onReturnPressed: root.connectClicked();
      }
    }

    RowLayout {
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

      Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        text: "Port:"
        font.pointSize: 16
      }

      TextField {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        placeholderText: "25015"
        font.pointSize: 16

        Keys.onReturnPressed: root.connectClicked();
      }
    }

    Button {
      Layout.rightMargin: 5
      Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

      text: "Connect"
      font.pointSize: 14

      onClicked: root.connectClicked();
    }
  }
}
