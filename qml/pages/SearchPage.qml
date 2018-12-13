import QtQuick 2.0
import Sailfish.Silica 1.0
import fishtheke.filterelement 1.0
import fishtheke.sortorder 1.0
import fishtheke.sortkey 1.0

Page {
    id: searchPage
    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    onStatusChanged: {
        if (status === PageStatus.Active && pageStack.nextPage() === null) {
            pageStack.pushAttached(Qt.resolvedUrl("ResultsPage.qml"))
        }
    }

    SilicaFlickable {
        id: searchFlick
        anchors.fill: parent
        // Tell SilicaFlickable the height of its content.
        contentHeight: searchPage.height
        contentWidth: searchPage.width
        PullDownMenu {
            quickSelect: true
            MenuItem {
                _duration: 25
                text: qsTr("Add filter")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("QueryDialog.qml"))
                    dialog.accepted.connect(function() {
                        queryFilters.addFilter(dialog.elem)
                    })
                }
            }
        }

        Column {
            id: contentColumn
            spacing: Theme.paddingSmall
            width: parent.width

            PageHeader {
                id: header
                title: qsTr("Search");
                Label {
                    id: aboutText
                    anchors {
                        left: header.extraContent.left
                        verticalCenter: header.extraContent.verticalCenter
                    }
                    width: header.extraContent.width
                    text: qsTr("About")
                    color: Theme.highlightColor
                    font {
                        pixelSize: Theme.fontSizeLarge
                        family: Theme.fontFamilyHeading
                    }
                }
            }

            ComboBox {
                width: parent.width
                label: qsTr("Sorted by") + ":"

                menu: ContextMenu {
                    MenuItem { text: qsTr("timestamp")}
                    MenuItem { text: qsTr("duration") }
                    MenuItem { text: qsTr("channel")  }
                }
                onValueChanged: {
                    switch (currentIndex) {
                    case 2:
                        datafetcher.sortedBy = SortKey.CHANNEL;
                        break;
                    case 1:
                        datafetcher.sortedBy = SortKey.DURATION;
                        break;
                    case 0:
                    default:
                        datafetcher.sortedBy = SortKey.TIMESTAMP;
                        break;
                    };
                }
            }

            ComboBox {
                width: parent.width
                label: qsTr("Sort order") + ":"

                menu: ContextMenu {
                    MenuItem { text: qsTr("desc")}
                    MenuItem { text: qsTr("asc") }
                }
                onValueChanged: {
                    switch (currentIndex) {
                    case 1:
                        datafetcher.sortOrder = SortOrder.ASC;
                        break;
                    case 0:
                    default:
                        datafetcher.sortOrder = SortOrder.DESC;
                        break;
                    };
                }
            }

            Slider {
                width: parent.width
                minimumValue: 1
                maximumValue: 100
                stepSize: 1
                value: 25
//                valueText: value + " " + qsTr("Items per search")
                valueText: value
                label: qsTr("Items per search")
                onValueChanged: {
                    datafetcher.searchBlockSize = value;
                }
            }

            SilicaListView {
                height: parent.height
                width: parent.width
                anchors.left: parent.left

                id: filters
                model: queryFilters
                delegate: ListItem {
                    contentWidth: parent.width
                    width: parent.width;
                    id: listItem
                    menu: contextMenu

                    contentHeight: Theme.itemSizeMedium // two line delegate
                    ListView.onRemove: animateRemoval(listItem)
                    onClicked: {
                        if (menuOpen) {
                            closeMenu()
                        } else {
                            openMenu()
                        }
                    }

                    function getOptionsAsString(display) {
                        var text = ""
                        if (display.title) {
                            text += qsTr("titles") + " ";
                        }
                        if (display.channel) {
                            text += qsTr("channel") + " ";
                        }
                        if (display.description) {
                            text += qsTr("description") + " ";
                        }
                        if (display.topic) {
                            text += qsTr("topic") + " ";
                        }
                        return text;
                    }

                    Label {
                        id: label
                        rightPadding: Theme.paddingLarge
                        leftPadding: Theme.paddingLarge
                        text: display.query
                        color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                        font.pixelSize: Theme.fontSizeLarge
                    }
                    Label {
                        rightPadding: Theme.paddingLarge
                        leftPadding: Theme.paddingLarge
                        anchors.top: label.bottom
                        text: getOptionsAsString(display)

                        font.pixelSize: Theme.fontSizeMedium
                        color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
                    }

                    Component {
                        id: contextMenu
                        ContextMenu {
                            MenuItem {
                                text: qsTr("Remove")
                                onClicked: { queryFilters.removeRow(index, index) }
                            }
                            MenuItem {
                                text: qsTr("Remove all")
                                onClicked: { queryFilters.clearAllFilters() }
                            }
                        }
                    }
                }

                ViewPlaceholder {
                    anchors.fill: parent
                    anchors.left: filters.left
                    width: filters.width
                    id: emptyText
                    text: qsTr("Add a filter to your search")
                    hintText: qsTr("Pull down to add a filter")
                    enabled: filters.count == 0
                }
            }
        }
    }
}
