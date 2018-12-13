import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        id: label
        topPadding: Theme.paddingLarge
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Fishtheke")
    }

    Image {
        source: "/usr/share/icons/hicolor/172x172/apps/harbour-fishtheke.png"
        anchors {
            verticalCenter: parent.verticalCenter

            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium

            right: parent.right
            rightMargin: Theme.paddingMedium
        }

        fillMode: Image.PreserveAspectFit
        width: parent.width - Theme.paddingMedium
        opacity: 0.25

    }
}
