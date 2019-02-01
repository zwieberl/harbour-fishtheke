import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutPage
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(searchPage)
        }
    }

    SilicaFlickable {
        id: aboutContainer
        contentHeight: column.height
        anchors.fill: parent

        Column {
            id: column
            width: aboutPage.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Filter")
            }

            Image {
                source: "/usr/share/icons/hicolor/172x172/apps/harbour-fishtheke.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }

                fillMode: Image.PreserveAspectFit
                width: 1/2 * parent.width

            }

            Label {
                text: "Fishtheke 0.6"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraLarge
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            Label {
                width: parent.width
                padding: Theme.paddingLarge
                text: qsTr("Licensed under MIT License")
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Label {
                padding: Theme.paddingLarge
                width: parent.width
                text: qsTr("Icons and banner licensed under CC-BY-3.0 (for details, see source code)")
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: "<a href=\"https://github.com/zwieberl/harbour-fishtheke\">" + qsTr("Sources on GitHub") + "</a>"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.highlightColor

                onLinkActivated: Qt.openUrlExternally("https://github.com/zwieberl/harbour-fishtheke")
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            SectionHeader {
                text: qsTr("Credits")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                text: qsTr("Special thanks to mediathekviewweb.de for providing their service and allowing me to utilize it with fishtheke!")
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: "<a href=\"https://mediathekviewweb.de/\">mediathekviewweb.de</a>"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
                font.pixelSize: Theme.fontSizeSmall
                linkColor: Theme.highlightColor

                onLinkActivated: Qt.openUrlExternally("https://mediathekviewweb.de/")
            }

            Separator {
                width: parent.width
                color: Theme.primaryColor
                horizontalAlignment: Qt.AlignHCenter
            }

            SectionHeader {
                text: qsTr("Contributions")
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                text: qsTr("Another very special thanks to the following contributors") + ":"
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }
            Repeater {
                model: [["eLtMosen", qsTr("Design of icons and store-banner")],
                        ["jgibbon", qsTr("Help with various topics")]]
                Label {
                    x: Theme.horizontalPageMargin
                    width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                    text: modelData[0] + ": " + modelData[1]
                    wrapMode: Text.Wrap
                    leftPadding: Theme.paddingLarge
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                    }
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            Label {
                id: separatorLabel
                x: Theme.horizontalPageMargin
                width: parent.width  - ( 2 * Theme.horizontalPageMargin )
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.Wrap
                anchors {
                    horizontalCenter: parent.horizontalCenter
                }
            }

            VerticalScrollDecorator {}
        }

    }
}
