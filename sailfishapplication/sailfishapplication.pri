QT += quick qml

SOURCES += $$PWD/sailfishapplication.cpp \
    sailfishapplication/objecttypecache.cpp \
    sailfishapplication/hostcore.cpp \
    sailfishapplication/vaultcore.cpp \
    sailfishapplication/classcache.cpp \
    sailfishapplication/mfwsrest.cpp \
    sailfishapplication/structurecachebase.cpp \
    sailfishapplication/propertydefcache.cpp \
    sailfishapplication/objectfront.cpp \
    sailfishapplication/objectcore.cpp \
    sailfishapplication/vaultfront.cpp \
    sailfishapplication/objver.cpp \
    sailfishapplication/objectcache.cpp \
    sailfishapplication/objid.cpp \
    sailfishapplication/objectversioncore.cpp \
    sailfishapplication/objectversionfront.cpp \
    sailfishapplication/realvalidator.cpp \
    sailfishapplication/integervalidator.cpp \
    sailfishapplication/valuelistcache.cpp \
    sailfishapplication/valuelistcore.cpp \
    sailfishapplication/valuelistfront.cpp \
    sailfishapplication/valuelistmodel.cpp \
    sailfishapplication/frontbase.cpp \
    sailfishapplication/corebase.cpp \
    sailfishapplication/appmonitor.cpp \
    sailfishapplication/errorinfo.cpp
HEADERS += $$PWD/sailfishapplication.h \
    sailfishapplication/objecttypecache.h \
    sailfishapplication/hostcore.h \
    sailfishapplication/vaultcore.h \
    sailfishapplication/classcache.h \
    sailfishapplication/mfwsrest.h \
    sailfishapplication/structurecachebase.h \
    sailfishapplication/propertydefcache.h \
    sailfishapplication/objectfront.h \
    sailfishapplication/objectcore.h \
    sailfishapplication/vaultfront.h \
    sailfishapplication/objver.h \
    sailfishapplication/objectcache.h \
    sailfishapplication/objid.h \
    sailfishapplication/objectversioncore.h \
    sailfishapplication/objectversionfront.h \
    sailfishapplication/realvalidator.h \
    sailfishapplication/integervalidator.h \
    sailfishapplication/valuelistcache.h \
    sailfishapplication/valuelistcore.h \
    sailfishapplication/valuelistfront.h \
    sailfishapplication/valuelistmodel.h \
    sailfishapplication/valuelistkey.h \
    sailfishapplication/frontbase.h \
    sailfishapplication/corebase.h \
    sailfishapplication/appmonitor.h \
    sailfishapplication/errorinfo.h
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




