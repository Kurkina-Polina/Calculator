import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 6.4

ColumnLayout {
    anchors.fill: parent
    spacing: 10

    TextArea {
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: 311
        Layout.preferredHeight: 70
        text: "Скрытое меню"
        readOnly: true
    }

    Button {
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: 321
        Layout.preferredHeight: 26
        text: "назад"
        onClicked: window.currentPage = 0
    }
}
