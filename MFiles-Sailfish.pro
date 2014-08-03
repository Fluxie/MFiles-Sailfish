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
    src/frontend/objectfront.cpp \
    src/backend/objectcore.cpp \
    src/frontend/vaultfront.cpp \
    src/mfiles/objver.cpp \
    src/backend/objectcache.cpp \
    src/mfiles/objid.cpp \
    src/backend/objectversioncore.cpp \
    src/frontend/objectversionfront.cpp \
    src/ui/realvalidator.cpp \
    src/ui/integervalidator.cpp \
    src/backend/valuelistcore.cpp \
    src/frontend/valuelistfront.cpp \
    src/frontend/frontbase.cpp \
    src/backend/corebase.cpp \
    src/errors/appmonitor.cpp \
    src/errors/errorinfo.cpp \
    src/errors/errorlayer.cpp \
    src/errors/errormodel.cpp \
    src/backend/typedvaluefilter.cpp \
    src/frontend/classesfront.cpp \
    src/ui/propertyvalueownerresolver.cpp \
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
    src/ui/allowedlookupsresolver.cpp \
    src/ui/lookuplistmodel.cpp \
    src/ui/valuelistitemlistmodel.cpp \
    src/ui/propertyvaluelistmodel.cpp \
    src/ui/viewlistmodel.cpp \
    src/backend/listingcache.cpp \
    src/backend/cachedlisting.cpp \
    src/backend/listingid.cpp \
    src/backend/listresourcecachebase.cpp \
    src/frontend/listingfront.cpp \
    src/ui/resetmodel.cpp \
    src/mfiles/foldercontentitem.cpp \
    src/mfiles/objectversion.cpp \
    src/mfiles/view.cpp \
    src/mfiles/viewlocation.cpp \
    src/ui/customroleproxymodel.cpp
HEADERS += src/backend/objecttypecache.h \
    src/backend/hostcore.h \
    src/backend/vaultcore.h \
    src/backend/classcache.h \
    src/mfwsrest.h \
    src/backend/structurecachebase.h \
    src/backend/propertydefcache.h \
    src/frontend/objectfront.h \
    src/backend/objectcore.h \
    src/frontend/vaultfront.h \
    src/mfiles/objver.h \
    src/backend/objectcache.h \
    src/mfiles/objid.h \
    src/backend/objectversioncore.h \
    src/frontend/objectversionfront.h \
    src/ui/realvalidator.h \
    src/ui/integervalidator.h \
    src/backend/valuelistcore.h \
    src/frontend/valuelistfront.h \
    src/backend/valuelistkey.h \
    src/frontend/frontbase.h \
    src/backend/corebase.h \
    src/errors/appmonitor.h \
    src/errors/errorinfo.h \
    src/errors/errorlayer.h \
    src/errors/errormodel.h \
    src/backend/typedvaluefilter.h \
    src/frontend/classesfront.h \
    src/ui/propertyvalueownerresolver.h \
    src/mfiles/propertyvalue.h \
    src/mfiles/typedvalue.h \
    src/mfiles/lookup.h \
    src/mfiles/valuelistitem.h \
    src/mfiles/propertydef.h \
    src/mfiles/objtype.h \
    src/backend/asyncfetch.h \
    src/backend/lazyownerinfo.h \
    src/mfiles/ownerproperty.h \
    src/ui/allowedlookupsresolver.h \
    src/ui/lookuplistmodel.h \
    src/ui/valuelistitemlistmodel.h \
    src/ui/propertyvaluelistmodel.h \
    src/mfiles/mfilesconstants.h \
    src/mfiles/mfilestypewrapper.h \
    src/ui/viewlistmodel.h \
    src/backend/listingcache.h \
    src/backend/cachedlisting.h \
    src/backend/listingid.h \
    src/backend/listresourcecachebase.h \
    src/frontend/listingfront.h \
    src/ui/resetmodel.h \
    src/ui/listmodelbase.h \
    src/mfiles/foldercontentitem.h \
    src/mfiles/objectversion.h \
    src/mfiles/view.h \
    src/mfiles/viewlocation.h \
    src/ui/customroleproxymodel.h

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
    MFiles-Sailfish.desktop \
    qml/pages/ViewListing.qml

