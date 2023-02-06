import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.DBus 2.0
import Nemo.Notifications 1.0

Page {
    id: sendingPage
    property var item
    property string url
    property string type
    allowedOrientations: Orientation.All

    Notification {
         id: notification
         isTransient: true
    }

    function errorpopup(summary, message) {
        notification.previewSummary = summary;
        notification.previewBody = message;
        notification.urgency = Notification.Critical;
        notification.expireTimeout = -1; // reset because we also use it in popup()
        notification.publish();
    }
    function popup(message) {
        notification.previewSummary = message;
        notification.previewBody = ""; // reset because we may have used it for an error
        notification.urgency = Notification.Normal;
        notification.expireTimeout = 3;
        notification.publish();
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
        property bool sendNameAsWell: false
        function sendURL(name, url) {
           if (sendNameAsWell) {
               Clipboard.text = name + " " + url
           } else {
               Clipboard.text = url
           }

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
                          function(result) {
                                  popup(qsTr('Opening %1', '%1 is application name').arg(qsTr('Jolla gallery', 'application name')))
                                  console.log('Send ' + url + 'to gallery.')
                          },
                          function(error, message) {
                                  errorpopup(
                                          qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Jolla gallery', 'application name')),
                                          message
                                  )
                          }
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
                          function(result) {
                                  popup(qsTr('Opening %1', '%1 is application name').arg(qsTr('Jupii', 'application name')))
                                  console.log('Send ' + url + 'to jupii.')
                          },
                          function(error, message) {
                                  errorpopup(
                                          qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Jupii', 'application name')),
                                          message
                                  )
                          }
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
                    function(result) {
                                  popup(qsTr('Opening %1', '%1 is application name').arg(qsTr('Kodimote', 'application name')))
                                  console.log('Send ' + url + ' to Kodimote.')
                    },
                    function(error, message) {
                            errorpopup(
                                    qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Kodimote', 'application name')),
                                    message
                            )
                    }
          )
        }
    }

    DBusInterface {
        id: vodman
        property bool isVisible: false
        property bool isEnabled: false
        service: 'org.duckdns.jgressmann.vodman.app'
        iface: 'org.freedesktop.DBus.Peer'
        path: '/instance'
        function init() {
            isVisible = datafetcher.fileExists("/usr/bin/harbour-vodman")
            console.log("Is visible: " + isVisible)
            if (isVisible) {
                typedCall('Ping',
                              [] ,
                              function(result) { isEnabled = true
                                                 iface = 'org.duckdns.jgressmann.vodman.app'},
                              function(error, message) { isEnabled = false }
                          )
            }

        }

        function sendURL(name, url) {
            typedCall('download',
                          { 'type': 's', 'value': url },
                          function(result) {
                                  popup(qsTr('Opening %1', '%1 is application name').arg(qsTr('Vodman', 'application name')))
                                  console.log('Send ' + url + 'to vodman.')
                          },
                          function(error, message) {
                                  errorpopup (
                                          qsTr('Failed to send to %1', '%1 is application name').arg(qsTr('Vodman', 'application name')),
                                          message
                                  )
                          }
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
            height: sendingPage.height
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
                                [vodman,      qsTr("Vodman", "application name")],
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
                contentHeight: (obj == clipboard) ? 2 * Theme.itemSizeSmall : Theme.itemSizeSmall // one line delegate
                enabled: obj.isEnabled
                Column {
                    width: parent.width;

                    Label {
                        id: streamButtonLabel
                        enabled: obj.isEnabled
                        padding: Theme.paddingMedium
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: objtext
                        font.pixelSize: Theme.fontSizeLarge
                        color: obj.isEnabled ? Theme.primaryColor : Theme.darkPrimaryColor;
                    }

                    TextSwitch {
                        id: sendNameSwitch
                        visible: (obj == clipboard)
                        checked: false
                        text: qsTr("Copy title as well")
                        leftMargin: Theme.paddingLarge
                        onClicked: { clipboard.sendNameAsWell = checked; }
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}

