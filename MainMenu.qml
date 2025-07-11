import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 6.4

Rectangle {
    id: background
    width: 360
    height: 640
    color: "#024873"
    property var operationPrecedence: ({
        "+": 1,
        "-": 1,
        "*": 2,
        "/": 2
    })

ColumnLayout {
    Layout.preferredWidth: 360
    Layout.preferredHeight: 640
    anchors.fill: parent
    spacing: 0

    // Главное меню (первая вкладка)
    // Поля ввода
    Image {
        anchors.top: parent.top
        source: "qrc:/icons/Status bar.svg"
    }
    Item {
        width: 360
        height: 156
        anchors.topMargin: 0

        Image {
               anchors.fill: parent
               source: "qrc:/icons/back_input.svg"
           }

        TextInput {
            id: historyDisplay
            Layout.alignment: Qt.AlignHRight
            Layout.preferredWidth: 280
            Layout.preferredHeight: 30
            horizontalAlignment: Text.AlignRight
            readOnly: true

        }

        TextInput {
            id: mainDisplay
            Layout.alignment: Qt.AlignHRight
            Layout.preferredWidth: 280
            Layout.preferredHeight: 30
            text: "0"
            horizontalAlignment: Text.AlignRight
        }
    }

        // Сетка кнопок
    GridLayout {
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 4
        columnSpacing: 5
        rowSpacing: 5

        // "plus", "minus", "mult", "devide" "btn_" "percent" "parenthese" "invert" "C" "point" "calculate"
        // Ряд 0

        CalculatorButton { btnText: "parenthese"; btnType: "action" }
        CalculatorButton { btnText: "invert"; btnType: "action" }
        CalculatorButton { btnText: "percent"; btnType: "action" }
        CalculatorButton { btnText: "devide"; btnType: "operation" }

        // Ряд 1
        CalculatorButton { btnText: "btn_7"; btnType: "number" }
        CalculatorButton { btnText: "btn_8"; btnType: "number" }
        CalculatorButton { btnText: "btn_9"; btnType: "number" }
        CalculatorButton { btnText: "mult"; btnType: "operation" }

        // Ряд 2
        CalculatorButton { btnText: "btn_4"; btnType: "number" }
        CalculatorButton { btnText: "btn_5"; btnType: "number" }
        CalculatorButton { btnText: "btn_6"; btnType: "number" }
        CalculatorButton { btnText: "minus"; btnType: "operation" }

        // Ряд 3
        CalculatorButton { btnText: "btn_1"; btnType: "number" }
        CalculatorButton { btnText: "btn_2"; btnType: "number" }
        CalculatorButton { btnText: "btn_3"; btnType: "number" }
        CalculatorButton { btnText: "plus"; btnType: "operation" }

        // Ряд 4
        CalculatorButton { btnText: "C"; btnType: "action" }
        CalculatorButton { btnText: "btn_0"; btnType: "number" }
        CalculatorButton { btnText: "point"; btnType: "action" }
        CalculatorButton { btnText: "calculate"; btnType: "action" }
    }

}
}
