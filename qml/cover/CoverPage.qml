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
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium

            rightMargin: Theme.paddingLarge
            leftMargin: Theme.paddingLarge

            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        fillMode: Image.PreserveAspectFit
        width: parent.width - 2 * Theme.paddingLarge
        opacity: 0.25

    }
}
