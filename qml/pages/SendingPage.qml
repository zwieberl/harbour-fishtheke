import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.DBus 2.0
import Nemo.Notifications 1.0

Page {
    property var item
    property string url
    property string type

    Notification {
         id: notification
         urgency: Notification.Critical
         isTransient: true
     }

    Item {
        id: browser
        property bool isVisible: true
        property bool isEnabled: true
        function sendURL(name, url) {
           Qt.openUrlExternally(url)
        }
        function init() {}
    }

    Item {
        id: clipboard
        property bool isVisible: true
        property bool isEnabled: true
        function sendURL(name, url) {
           Clipboard.text = url
        }
        function init() {}
    }

    Item {
        id: videoPlayer
        property bool isVisible: false
        property bool isEnabled: true
        function init() {
            console.log("videoplayer: " + datafetcher.fileExists("/usr/bin/harbour-videoPlayer"));
            videoPlayer.isVisible = datafetcher.fileExists("/usr/bin/harbour-videoPlayer")
        }

        function sendURL(name, url) {
           datafetcher.runExternalCommand("/usr/bin/harbour-videoPlayer -p \"" + url + "\"")
        }
    }
    DBusInterface {
        id: gallery
        property bool isVisible: false
        property bool isEnabled: true
        service: 'com.jolla.gallery'
        iface: 'com.jolla.gallery.ui'
        path: '/com/jolla/gallery/ui'
        function init() {
            gallery.isVisible = datafetcher.fileExists("/usr/bin/jolla-gallery")
        }

        function sendURL(name, url) {
            typedCall('playVideoStream',
                          { 'type': 'as', 'value': [url] },
                          function(result) { console.log('Send ' + url + 'to gallery.') },
                          function(error, message) { notification.previewSummary = qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Jolla gallery', 'application name'));
                                                     notification.previewBody = message;
                                                     notification.publish()}
                      )
        }
    }

    DBusInterface {
        id: jupii
        property bool isVisible: false
        property bool isEnabled: false
        service: 'org.jupii'
        iface: 'org.freedesktop.DBus.Peer'
        path: '/'
        function init() {
            isVisible = datafetcher.fileExists("/usr/bin/harbour-jupii")
            if (isVisible) {
                typedCall('Ping',
                              [] ,
                              function(result) { isEnabled = true
                                                 iface = 'org.jupii.Player'},
                              function(error, message) { isEnabled = false }
                          )
            }

        }

        function sendURL(name, url) {
            typedCall('addUrl',
                          [{ 'type': 's', 'value': url },
                          { 'type': 's', 'value': name }] ,
                          function(result) { console.log('Send ' + url + 'to jupii.') },
                          function(error, message) { notification.previewSummary = qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Jupii', 'application name'));
                                                     notification.previewBody = message;
                                                     notification.publish()}
                      )
        }
    }


    DBusInterface {
        id:kodimote
        property bool isVisible: false
        property bool isEnabled: false
        service: 'org.mpris.MediaPlayer2.kodimote'
        iface: 'org.freedesktop.DBus.Peer'
        path: '/org/mpris/MediaPlayer2'

        function init() {
          isVisible = datafetcher.fileExists("/usr/bin/harbour-kodimote");
          typedCall('Ping',
                    [] ,
                    function(result) { isEnabled = true
                                       iface = 'org.mpris.MediaPlayer2.Player'},
                    function(error, message) { isEnabled = false }
                    )
        }
        function sendURL(name, url) {
          typedCall('OpenUri',
                    { 'type': 's', 'value': url } ,
                    function(result) { console.log('Send ' + url + ' to Kodimote.') },
                    function(error, message) { notification.previewSummary = qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Kodimote', 'application name'));
                                               notification.previewBody = message;
                                               notification.publish()}
                    )
        }
    }

    Column {
        id: contentColumn
        spacing: Theme.paddingSmall
        width: parent.width

        PageHeader {
            id: header
            title: qsTr("Sending URL");
            Label {
                id: aboutText
                anchors {
                    left: header.extraContent.left
                    verticalCenter: header.extraContent.verticalCenter
                }
                width: header.extraContent.width
                text: qsTr("Cancel")
                color: Theme.highlightColor
                font {
                    pixelSize: Theme.fontSizeLarge
                    family: Theme.fontFamilyHeading
                }
            }
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
                    var objs = [[browser,     qsTr("Open in browser")],
                                [clipboard,   qsTr("Copy to clipboard")],
                                [gallery,     qsTr("Jolla gallery", "application name")],
                                [jupii,       qsTr("Jupii", "application name")],
                                [kodimote,    qsTr("Kodimote", "application name")],
                                [videoPlayer, qsTr("LLs VideoPlayer", "application name")],
                               ];
                    for(var idx in objs) {
                        var obj = objs[idx][0]
                        var objtext = objs[idx][1]
                        /* Note: Order of Component.onCompleted-calls is undefined! So we need an init()-function */
                        obj.init();
                        if (obj.isVisible) {
                            append({"obj": obj, "objtext": objtext});
                        }
                    }
                }
            }

            delegate: ListItem {
                id: sendButtonItem
                onClicked: {
                    if (obj.isEnabled) {
                        obj.sendURL(item.title, url)
                        pageStack.pop()
                    }
                }

                contentWidth: parent.width
                width: parent.width;
                contentHeight: Theme.itemSizeSmall // one line delegate
                enabled: obj.isEnabled
                Label {
                    id: streamButtonLabel
                    enabled: obj.isEnabled
                    padding: Theme.paddingMedium
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: objtext
                    font.pixelSize: Theme.fontSizeLarge
                    color: obj.isEnabled ? Theme.primaryColor : Theme.darkPrimaryColor;
                }
            }
            VerticalScrollDecorator {}
        }
    }
}

