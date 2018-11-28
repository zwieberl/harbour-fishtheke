# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-fishtheke

CONFIG += sailfishapp

SOURCES += \
    src/datafetcher.cpp \
    src/queryfilters.cpp \
    src/filterelement.cpp \
    src/harbour-fishtheke.cpp

DISTFILES += \
    qml/cover/CoverPage.qml \
    rpm/harbour-fishtheke.changes.in \
    rpm/harbour-fishtheke.changes.run.in \
    rpm/harbour-fishtheke.spec \
    rpm/harbour-fishtheke.yaml \
    translations/*.ts \
    harbour-fishtheke.desktop \
    qml/pages/SearchPage.qml \
    qml/pages/ResultsPage.qml \
    qml/pages/DetailedView.qml \
    qml/pages/QueryDialog.qml \
    qml/harbour-fishtheke.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-fishtheke-de.ts \
                translations/harbour-fishtheke.ts

HEADERS += \
    src/datafetcher.h \
    src/queryfilters.h \
    src/filterelement.h
