import QtQuick 2.15
import QtQuick.Controls 6.4

Button {
    property string btnText: ""
    property string btnType: "action"
    property var handler: null // Общий обработчик
    property var onClickAction: null // Конкретная функция для клика
    // icon.source: `qrc:/icons/${btnType}/${btnText}.svg`
    icon.width: 60
    icon.height: 60
    text: btnText
    implicitWidth: 68
    implicitHeight: 68
    background: null

    property bool isPressed: false

    icon.source: isPressed
        ? `qrc:/icons/${btnType}/active_${btnText}.svg`
        : `qrc:/icons/${btnType}/${btnText}.svg`

    onPressedChanged: isPressed = pressed
    onClicked: {
        if (onClickAction) onClickAction()
        else if (handler) handler.defaultAction(btnText)
    }

    Timer {
        id: longPressTimer
        interval: 4000
        onTriggered: if (btnText === "calculate") window.isLongPress = true
    }

    onPressed: if (btnText === "calculate") longPressTimer.start()
    onReleased: {
        longPressTimer.stop()
    }

}

