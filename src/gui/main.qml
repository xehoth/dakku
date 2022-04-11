import QtQuick

Window {
    id: window
    objectName: "window"
    width: 1920
    height: 1080
    visible: true
    title: qsTr("dakku renderer gui")

    Image {
        id: film
        x: 0
        y: 0
        objectName: "film"
        cache: false
        source: "image://film/"
        fillMode: Image.PreserveAspectFit
        asynchronous: true
        scale: 1.0
        width: window.width
        height: window.height
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
