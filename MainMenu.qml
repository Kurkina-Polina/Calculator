import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 6.4
import Calculator 1.0

Rectangle {
    id: mainDisplay
    width: 360
    height: 640
    color: "#024873"
    property string displayText: "0"
    property string historyText: ""

    property var operationPrecedence: ({
        "+": 1,
        "-": 1,
        "*": 2,
        "/": 2
    })

    CalculatorEngine {
        id: calculator
        onDisplayTextChanged: (text) => {
            mainDisplay.displayText = text
            if (window.isLongPress && text === "123") {
                secretCodeVerified()
            }
        }
        onHistoryTextChanged: (text) => { mainDisplay.historyText = text}
        onSecretCodeVerified: {
            window.currentPage = 1
            window.isLongPress = false
        }
    }
    Image {
        id: statusBar
        anchors.top: parent.top
        source: "qrc:/icons/Status bar.svg"
    }

    Item {
        id: text
        width: 360
        height: 156
        anchors.top: statusBar.bottom

        Image {
           anchors.fill: parent
           source: "qrc:/icons/back_input.svg"
       }

        TextInput {
            id: historyField
            width: 280
            height: 30
            anchors {
                right: parent.right
                rightMargin: 40
                top: parent.top
                topMargin: 44
            }
            horizontalAlignment: Text.AlignRight
            text: mainDisplay.historyText
            readOnly: true
            font.pixelSize: 20
            color: "#FFFFFF"
        }

        TextInput {
            id: displayField
            width: 280
            height: 60
            anchors {
                right: parent.right
                rightMargin: 40
                top: historyField.bottom
                topMargin: 8
            }
            horizontalAlignment: Text.AlignRight
            text: mainDisplay.displayText
            font.pixelSize: 50
            color: "#FFFFFF"
        }
    }
    Item{
        id: menu
        width: 312
        height: 396
        anchors {
            right: parent.right
            rightMargin: 24
            top: text.bottom
            topMargin: 24
        }

        GridLayout {
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 4
            columnSpacing: 15
            rowSpacing: 15

            CalculatorButton { btnText: "parenthese"; btnType: "action";  onClickAction: calculator.handleParentheses}
            CalculatorButton { btnText: "invert"; btnType: "action"; onClickAction: calculator.handleInvertSign}
            CalculatorButton { btnText: "percent"; btnType: "action"; onClickAction: calculator.handlePercent}
            CalculatorButton { btnText: "devide"; btnType: "operation"; handler: calculator; onClickAction: function() { calculator.handleOperation("devide")}}

            CalculatorButton { btnText: "btn_7"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("7")}}
            CalculatorButton { btnText: "btn_8"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("8")}}
            CalculatorButton { btnText: "btn_9"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("9")}}
            CalculatorButton { btnText: "mult";  btnType: "operation"; handler: calculator; onClickAction: function() { calculator.handleOperation("mult")}}

            CalculatorButton { btnText: "btn_4"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("4")}}
            CalculatorButton { btnText: "btn_5"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("5")}}
            CalculatorButton { btnText: "btn_6"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("6") }}
            CalculatorButton { btnText: "minus"; btnType: "operation"; handler: calculator; onClickAction: function() { calculator.handleOperation("minus")} }

            CalculatorButton { btnText: "btn_1"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("1") }}
            CalculatorButton { btnText: "btn_2"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("2")  }}
            CalculatorButton { btnText: "btn_3"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("3") }}
            CalculatorButton { btnText: "plus";  btnType: "operation";  handler: calculator; onClickAction: function() { calculator.handleOperation("plus")} }

            CalculatorButton { btnText: "C"; btnType: "action"; onClickAction: calculator.handleClear }
            CalculatorButton { btnText: "btn_0"; btnType: "number"; handler: calculator; onClickAction: function() { calculator.handleDigit("0") } }
            CalculatorButton { btnText: "point"; btnType: "action";  onClickAction: calculator.handlePoint}
            CalculatorButton { btnText: "calculate"; btnType: "action";  onClickAction: calculator.calculate}
        }
    }
}
