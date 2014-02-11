# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = MFiles-Sailfish

CONFIG += sailfishapp
CONFIG += c++11

SOURCES += src/MFiles-Sailfish.cpp \
    src/objecttypecache.cpp \
    src/hostcore.cpp \
    src/vaultcore.cpp \
    src/classcache.cpp \
    src/mfwsrest.cpp \
    src/structurecachebase.cpp \
    src/propertydefcache.cpp \
    src/objectfront.cpp \
    src/objectcore.cpp \
    src/vaultfront.cpp \
    src/objver.cpp \
    src/objectcache.cpp \
    src/objid.cpp \
    src/objectversioncore.cpp \
    src/objectversionfront.cpp \
    src/realvalidator.cpp \
    src/integervalidator.cpp \
    src/valuelistcore.cpp \
    src/valuelistfront.cpp \
    src/frontbase.cpp \
    src/corebase.cpp \
    src/appmonitor.cpp \
    src/errorinfo.cpp \
    src/errorlayer.cpp \
    src/errormodel.cpp \
    src/typedvaluefilter.cpp \
    src/classesfront.cpp \
    src/propertyvalueownerresolver.cpp \
    src/mfiles/propertyvalue.cpp \
    src/mfiles/typedvalue.cpp \
    src/mfiles/lookup.cpp \
    src/mfiles/valuelistitem.cpp \
    src/mfiles/propertydef.cpp \
    src/mfiles/mfilestypecapsule.cpp \
    src/mfiles/objtype.cpp \
    src/asyncfetch.cpp \
    src/lazyownerinfo.cpp \
    src/mfiles/ownerproperty.cpp \
    src/allowedlookupsresolver.cpp \
    src/lookuplistmodel.cpp \
    src/valuelistitemlistmodel.cpp \
    src/propertyvaluelistmodel.cpp
HEADERS += src/objecttypecache.h \
    src/hostcore.h \
    src/vaultcore.h \
    src/classcache.h \
    src/mfwsrest.h \
    src/structurecachebase.h \
    src/propertydefcache.h \
    src/objectfront.h \
    src/objectcore.h \
    src/vaultfront.h \
    src/objver.h \
    src/objectcache.h \
    src/objid.h \
    src/objectversioncore.h \
    src/objectversionfront.h \
    src/realvalidator.h \
    src/integervalidator.h \
    src/valuelistcore.h \
    src/valuelistfront.h \
    src/valuelistkey.h \
    src/frontbase.h \
    src/corebase.h \
    src/appmonitor.h \
    src/errorinfo.h \
    src/errorlayer.h \
    src/errormodel.h \
    src/typedvaluefilter.h \
    src/classesfront.h \
    src/mfilesconstants.h \
    src/propertyvalueownerresolver.h \
    src/mfiles/propertyvalue.h \
    src/mfiles/typedvalue.h \
    src/mfiles/lookup.h \
    src/mfiles/valuelistitem.h \
    src/mfiles/propertydef.h \
    src/mfiles/mfilestypecapsule.h \
    src/mfiles/objtype.h \
    src/asyncfetch.h \
    src/lazyownerinfo.h \
    src/mfiles/ownerproperty.h \
    src/allowedlookupsresolver.h \
    src/lookuplistmodel.h \
    src/valuelistitemlistmodel.h \
    src/propertyvaluelistmodel.h

# QML files and folders
qml.files = *.qml common controls dialogs pages cover

OTHER_FILES += qml/MFiles-Sailfish.qml \
    qml/cover/*.qml \
    qml/pages/*.qml \
    qml/pages/*.js \
    qml/dialogs/*.qml \
    qml/dialogs/*.js \
    qml/controls/*.qml \
    qml/controls/*.js \
    qml/common/*.js \
    qml/visuals/*.qml \
    qml/visuals/*.js \
    rpm/MFiles-Sailfish.spec \
    rpm/MFiles-Sailfish.yaml \
    MFiles-Sailfish.desktop

