import QtQuick 2.15
import QtQuick.Controls 6.4

Button {
    property string btnText: ""
    property string btnType: "number" // "number", "operation", "action"
    icon.source: `qrc:/icons/${btnType}/${btnText}.svg`
    icon.width: 60
    icon.height: 60
    text: btnText
    implicitWidth: 60
    implicitHeight: 60
    background: null

    onClicked: {
        if (btnType === "action") {
            console.log("action:", btnText)
        }
        // Для нетипизированных кнопок сработает ветка 'number'
    }

    // signal clicked(string text, string type)
    // onClicked: clicked(btnText, btnType)


}

// Button{
// width: 60
// height: 60
// implicitWidth: 60  // Добавьте это
// implicitHeight: 60 // И это
// icon.source: "qrc:/icons/action/active.svg" // Путь к иконке
// icon.width: 60  // Размер иконки
// icon.height: 60
// background: null
// font.pixelSize: 24
// contentItem: Text {
//     text: root.text
//     font: root.font
//     color: root.textColor
//     horizontalAlignment: Text.AlignHCenter
//     verticalAlignment: Text.AlignVCenter
// }
// }

// Button {
//     id: root
//     property alias iconSource: icon.source
//     property string buttonText: "" // Текст для вывода при нажатии
//     property string buttonType: "" // Тип кнопки (например, "digit", "operation")

//     width: 60
//     height: 60

//     // signal clickedWithText(string text, string type) // Сигнал с параметрами
//     signal clickedWithText(string text, property color buttonTextColor) // Сигнал с параметрами

//     background: Rectangle {
//         color: "transparent"
//         radius: 5
//     }

//     Image {
//         id: icon
//         anchors.centerIn: parent
//         sourceSize: Qt.size(60, 60)
//     }

//     onClicked: {
//         clickedWithText(buttonText, buttonType) // Генерируем сигнал
//     }
// }
