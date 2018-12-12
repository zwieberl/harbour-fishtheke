import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{
    id: main
    Component.onCompleted: {
        pageStack.push([aboutPage, searchPage], PageStackAction.Immediate)
    }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations
    AboutPage { id: aboutPage }
    SearchPage { id: searchPage}
}
