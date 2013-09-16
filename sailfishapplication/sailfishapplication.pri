QT += quick qml

SOURCES += $$PWD/sailfishapplication.cpp \
    sailfishapplication/objecttypecache.cpp \
    sailfishapplication/hostcore.cpp \
    sailfishapplication/vaultcore.cpp \
    sailfishapplication/classcache.cpp \
    sailfishapplication/mfwsrest.cpp \
    sailfishapplication/vaultcache.cpp
HEADERS += $$PWD/sailfishapplication.h \
    sailfishapplication/objecttypecache.h \
    sailfishapplication/hostcore.h \
    sailfishapplication/vaultcore.h \
    sailfishapplication/classcache.h \
    sailfishapplication/mfwsrest.h \
    sailfishapplication/vaultcache.h
INCLUDEPATH += $$PWD

TARGETPATH = /usr/bin
target.path = $$TARGETPATH

DEPLOYMENT_PATH = /usr/share/$$TARGET
qml.path = $$DEPLOYMENT_PATH
desktop.path = /usr/share/applications

contains(CONFIG, desktop) {
    DEFINES *= DESKTOP
}

INSTALLS += target qml desktop

DEFINES += DEPLOYMENT_PATH=\"\\\"\"$${DEPLOYMENT_PATH}/\"\\\"\"

CONFIG += link_pkgconfig
CONFIG += c++11
packagesExist(qdeclarative-boostable) {
    message("Building with qdeclarative-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative-boostable
} else {
    warning("qdeclarative-boostable not available; startup times will be slower")
}




