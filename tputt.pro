QT += quick core network networkauth
CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


INCLUDEPATH += \
        include \
        include/Entities \
        include/Services

HEADERS += \
        $$files(include/*.h) \
        $$files(include/Entities/*.h) \
        $$files(include/Services/*.h)

SOURCES += \
        $$files(src/*.cpp) \
        $$files(src/Entities/*.cpp) \
        $$files(src/Services/*.cpp) 
RESOURCES += ui/qml.qrc

TRANSLATIONS += ui/locales/ru_RU.ts \
                ui/locales/en_GB.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

