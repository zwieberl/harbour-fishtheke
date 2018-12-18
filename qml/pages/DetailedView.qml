import QtQuick 2.0
import Sailfish.Silica 1.0
import "../utils.js" as Datehelper
import Nemo.DBus 2.0
import Nemo.Notifications 1.0

Page {
    id: detailPage

    Notification {
         id: notification
         urgency: Notification.Critical
         isTransient: true
     }

    DBusInterface {
        id: gallery
        property bool isActive: false

        service: 'com.jolla.gallery'
        iface: 'com.jolla.gallery.ui'
        path: '/com/jolla/gallery/ui'
        Component.onCompleted: {
            isActive = datafetcher.fileExists("/usr/bin/jolla-gallery")
        }

        function sendURL(url) {
            typedCall('playVideoStream',
                          { 'type': 'as', 'value': [url] },
                          function(result) { console.log('Send ' + url + 'to gallery.') },
                          function(error, message) { notification.previewSummary = qsTr('Failed to send to ') + qsTr('Jolla gallery');
                                                     notification.previewBody = message;
                                                     notification.publish()}
                      )
        }
    }

    DBusInterface {
        id: jupii
        property bool isActive: false
        service: 'org.jupii'
        iface: 'org.freedesktop.DBus.Peer'
        path: '/'
        Component.onCompleted: {
            typedCall('Ping',
                          [] ,
                          function(result) { isActive = true
                                             iface = 'org.jupii.Player'},
                          function(error, message) { isActive = false }
                      )
        }

        function sendURL(name, url) {
            typedCall('addUrl',
                          [{ 'type': 's', 'value': url },
                          { 'type': 's', 'value': name }] ,
                          function(result) { console.log('Send ' + url + 'to jupii.') },
                          function(error, message) { notification.previewSummary = qsTr('Failed to send to ') + qsTr('Jupii');
                                                     notification.previewBody = message;
                                                     notification.publish()}
                      )
        }
    }

    property var item
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        Column {
            id: content
            spacing: Theme.paddingMedium

            anchors {
                topMargin: Theme.paddingLarge

                left: parent.left
                leftMargin: Theme.horizontalPageMargin

                right: parent.right
                rightMargin: Theme.horizontalPageMargin
            }

            DetailItem { id: title; label: qsTr("title"); value: item.title }
            DetailItem { label: qsTr("channel"); value: item.channel }
            DetailItem { label: qsTr("duration"); value: Datehelper.seconds_to_DHMS(item.duration) }
            DetailItem { label: qsTr("timestamp"); value: Datehelper.date_from_epoch(item.timestamp) }
            Text {
                color: Theme.primaryColor
                width: parent.width
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.Wrap
                text: item.description
            }

            SilicaListView {
                id: urlView
                // Warning! height: parent.height leads to a weird hanging of the UI for a couple of seconds.
                height: 8 * (Theme.fontSizeLarge + Theme.paddingMedium)
                width: parent.width
                anchors.left: parent.left
                property string type
                property string url
                model: ListModel {
                    Component.onCompleted: {
                        if (detailPage.item.url_video_hd) {
                            append({"type": "Stream-URL HD", "url": detailPage.item.url_video_hd});
                        }
                        if (detailPage.item.url_video_sd) {
                            append({"type": "Stream-URL SD", "url": detailPage.item.url_video_sd});
                        }
                        if (detailPage.item.url_video) {
                            append({"type": "Stream-URL", "url": detailPage.item.url_video});
                        }
                        if (detailPage.item.url_website) {
                            append({"type": "Website", "url": detailPage.item.url_website});
                        }
                    }
                }

                delegate: ListItem {
                    id: streamButtonItem
                    menu: actionsMenu
                    onClicked: {
                        if (menuOpen) {
                            closeMenu()
                        } else {
                            openMenu()
                        }
                    }
                    contentWidth: parent.width
                    width: parent.width;
                    contentHeight: Theme.itemSizeSmall // one line delegate

                    Label {
                        id: streamButtonLabel
                        padding: Theme.paddingMedium
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: type
                        color: streamButtonItem.highlighted ? Theme.highlightColor : Theme.highlightBackgroundColor
                        font.pixelSize: Theme.fontSizeLarge
                    }

                    Component {
                        id: actionsMenu
                        ContextMenu {
                            MenuItem {
                                text: qsTr("Copy to clipboard")
                                onClicked: { Clipboard.text = url }
                            }
                            MenuItem {
                                text: qsTr("Open in browser")
                                onClicked: { Qt.openUrlExternally(url) }
                            }
                            MenuItem {
                                visible: gallery.isActive
                                text: qsTr("Jolla gallery")
                                onClicked: { gallery.sendURL(url) }
                            }
                            MenuItem {
                                visible: jupii.isActive
                                text: qsTr("Jupii")
                                onClicked: { jupii.sendURL(detailPage.item.title, url) }
                            }
                        }
                    }
                }
            }

            VerticalScrollDecorator {}
        }
    }
}
