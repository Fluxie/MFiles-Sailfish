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
    src/backend/objecttypecache.cpp \
    src/backend/hostcore.cpp \
    src/backend/vaultcore.cpp \
    src/backend/classcache.cpp \
    src/mfwsrest.cpp \
    src/backend/structurecachebase.cpp \
    src/backend/propertydefcache.cpp \
    src/objectfront.cpp \
    src/backend/objectcore.cpp \
    src/vaultfront.cpp \
    src/mfiles/objver.cpp \
    src/backend/objectcache.cpp \
    src/mfiles/objid.cpp \
    src/backend/objectversioncore.cpp \
    src/objectversionfront.cpp \
    src/realvalidator.cpp \
    src/integervalidator.cpp \
    src/backend/valuelistcore.cpp \
    src/valuelistfront.cpp \
    src/frontbase.cpp \
    src/backend/corebase.cpp \
    src/errors/appmonitor.cpp \
    src/errors/errorinfo.cpp \
    src/errors/errorlayer.cpp \
    src/errors/errormodel.cpp \
    src/backend/typedvaluefilter.cpp \
    src/classesfront.cpp \
    src/propertyvalueownerresolver.cpp \
    src/mfiles/propertyvalue.cpp \
    src/mfiles/typedvalue.cpp \
    src/mfiles/lookup.cpp \
    src/mfiles/valuelistitem.cpp \
    src/mfiles/propertydef.cpp \
    src/mfiles/mfilestypewrapper.cpp \
    src/mfiles/objtype.cpp \
    src/backend/asyncfetch.cpp \
    src/backend/lazyownerinfo.cpp \
    src/mfiles/ownerproperty.cpp \
    src/allowedlookupsresolver.cpp \
    src/lookuplistmodel.cpp \
    src/valuelistitemlistmodel.cpp \
    src/propertyvaluelistmodel.cpp
HEADERS += src/backend/objecttypecache.h \
    src/backend/hostcore.h \
    src/backend/vaultcore.h \
    src/backend/classcache.h \
    src/mfwsrest.h \
    src/backend/structurecachebase.h \
    src/backend/propertydefcache.h \
    src/objectfront.h \
    src/backend/objectcore.h \
    src/vaultfront.h \
    src/mfiles/objver.h \
    src/backend/objectcache.h \
    src/mfiles/objid.h \
    src/backend/objectversioncore.h \
    src/objectversionfront.h \
    src/realvalidator.h \
    src/integervalidator.h \
    src/backend/valuelistcore.h \
    src/valuelistfront.h \
    src/backend/valuelistkey.h \
    src/frontbase.h \
    src/backend/corebase.h \
    src/errors/appmonitor.h \
    src/errors/errorinfo.h \
    src/errors/errorlayer.h \
    src/errors/errormodel.h \
    src/backend/typedvaluefilter.h \
    src/classesfront.h \
    src/propertyvalueownerresolver.h \
    src/mfiles/propertyvalue.h \
    src/mfiles/typedvalue.h \
    src/mfiles/lookup.h \
    src/mfiles/valuelistitem.h \
    src/mfiles/propertydef.h \
    src/mfiles/objtype.h \
    src/backend/asyncfetch.h \
    src/backend/lazyownerinfo.h \
    src/mfiles/ownerproperty.h \
    src/allowedlookupsresolver.h \
    src/lookuplistmodel.h \
    src/valuelistitemlistmodel.h \
    src/propertyvaluelistmodel.h \
    src/mfiles/mfilesconstants.h \
    src/mfiles/mfilestypewrapper.h

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

