import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: "Calculator"

    property string inputText: "0"
    property string historyText: ""
    property bool isLongPress: false
    property int currentPage: 0


    // Текущая страница (0 - основное, 1 - скрытое)


    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainMenu {}
    }

    Timer {
        id: comboResetTimer
        interval: 3000
        onTriggered: inputText = ""
    }

    onInputTextChanged: {
        // comboResetTimer.restart()
        //
        if (isLongPress && inputText === "123") {
            stackView.push("HiddenMenu.qml")
            inputText = ""
        }
    }
}
