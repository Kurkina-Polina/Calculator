import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Calculator 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 360
    height: 640
    title: "Calculator"
    property int currentPage: 0 // Текущая страница (0 - основное, 1 - скрытое)
    property bool isLongPress: false

    onCurrentPageChanged: {
        if (currentPage === 0) {
            stackView.replace(null, mainMenuComponent)
        } else {
            stackView.replace(null, secretPageComponent)
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainMenu {}

        Component {
            id: mainMenuComponent
            MainMenu {}
        }

        Component {
            id: secretPageComponent
            HiddenMenu {}
        }
    }

}
