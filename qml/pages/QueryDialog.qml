import QtQuick 2.0
import Sailfish.Silica 1.0
import fishtheke.filterelement 1.0

Dialog {
    id: diag
    canNavigateForward: false
    allowedOrientations: Orientation.All
    property FilterElement elem

    function checkForwardPossible() {
        if ((channelSwitch.checked || titleSwitch.checked || descriptionSwitch.checked) && searchField.text.length > 0) {
            diag.canNavigateForward = true
        } else {
            diag.canNavigateForward = false
        }
    }

    Column {
        width: parent.width
        DialogHeader {
            id: header
            title: qsTr("Search")
        }

        TextSwitch {
            id: channelSwitch
            checked: false
            text: qsTr("channel")
            onClicked: checkForwardPossible()
        }
        TextSwitch {
            id: titleSwitch
            checked: false
            text: qsTr("title")
            onClicked: checkForwardPossible()
        }
        TextSwitch {
            id: descriptionSwitch
            checked: false
            text: qsTr("description")
            onClicked: checkForwardPossible()
        }
        SearchField {
            id: searchField
            width: parent.width
            placeholderText: qsTr("Search in Mediathek...")
            focus: true
            // By default, text prediction is turned off for SearchFields,
            // so we have to reactivate it by hand
            inputMethodHints: Qt.ImhNone
            onTextChanged: checkForwardPossible()

            EnterKey.onClicked: {
                                  focus = false;
                                  if (canNavigateForward) {
                                      accept();
                                  }
                                }
        }
    }
    onOpened: {
        if (elem) {
            searchField.text = elem.query;
            titleSwitch.checked = elem.title;
            descriptionSwitch.checked = elem.description;
            channelSwitch.checked = elem.channel;
            checkForwardPossible()
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            if (!elem) {
                elem = queryFilters.createNewFilter()
            }
            elem.query = searchField.text
            elem.channel = channelSwitch.checked
            elem.title = titleSwitch.checked
            elem.description = descriptionSwitch.checked
        }
    }
}
