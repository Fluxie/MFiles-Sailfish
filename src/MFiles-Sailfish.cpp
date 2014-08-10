/*
 *  Copyright 2013 Mikko Rantanen
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  M-Files for Sailfish is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with M-Files for Sailfish. If not, see
 *  <http://www.gnu.org/licenses/>.
 */

#include <QAbstractItemModel>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml>
#include <QStringListModel>
#include <QSortFilterProxyModel>

#include <sailfishapp.h>

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "errors/appmonitor.h"
#include "backend/asyncfetch.h"
#include "errors/errormodel.h"
#include "errors/errorinfo.h"
#include "backend/hostcore.h"
#include "ui/integervalidator.h"
#include "frontend/objectfront.h"
#include "frontend/objectversionfront.h"
#include "backend/lazyownerinfo.h"
#include "ui/lookuplistmodel.h"
#include "ui/propertyvaluelistmodel.h"
#include "ui/realvalidator.h"
#include "backend/typedvaluefilter.h"
#include "frontend/vaultfront.h"
#include "ui/customroleproxymodel.h"
#include "ui/qmlsortfilterproxymodel.h"
#include "ui/valuelistitemlistmodel.h"
#include "ui/viewlistmodel.h"
#include "frontend/listingfront.h"
#include "frontend/valuelistfront.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{	
	// REgister metatypes.
	qRegisterMetaType< ErrorInfo >();

	// Register C++ classes as QML types.	
	qmlRegisterType< AppMonitor >("mohari.sailfish", 1, 0, "AppMonitor");
	qmlRegisterType< ErrorModel >("mohari.sailfish", 1, 0, "ErrorModel");
	qmlRegisterType< LookupListModel >("mohari.sailfish", 1, 0, "LookupListModel");
	qmlRegisterType< TypedValueFilter >("mohari.sailfish", 1, 0, "TypedValueFilter");
	qmlRegisterType< CustomRoleProxyModel >("mohari.sailfish", 1, 0, "CustomRoleProxyModel");
	qmlRegisterType< PropertyValueListModel >("mohari.sailfish", 1, 0, "PropertyValueListModel");
	qmlRegisterType< QmlSortFilterProxyModel >("mohari.sailfish", 1, 0, "QmlSortFilterProxyModel");
	qmlRegisterType< ValueListItemListModel >("mohari.sailfish", 1, 0, "ValueListItemListModel");
	qmlRegisterType< ViewListModel >("mohari.sailfish", 1, 0, "ViewListModel");
	qmlRegisterType< VaultFront >("mohari.sailfish", 1, 0, "VaultFront");
	qmlRegisterType< IntegerValidator >("mohari.sailfish", 1, 0, "IntegerValidator");
	qmlRegisterType< RealValidator >("mohari.sailfish", 1, 0, "RealValidator");
	qmlRegisterType< AsyncFetch >();
	qmlRegisterType< LazyOwnerInfo >();
	qmlRegisterType< ListingFront >();
	qmlRegisterType< ObjectFront >();
	qmlRegisterType< ObjectVersionFront >();
	qmlRegisterType< ValueListFront >();

	// Register certain QT specific types in order to use them in QML.
	qmlRegisterType< QAbstractItemModel >();
	qmlRegisterType< QStringListModel >();

	// Create and register global monitor.
	QScopedPointer< AppMonitor > monitor( new AppMonitor() );

	// Create host core.
	QScopedPointer< HostCore > host( new HostCore( monitor.data() ) );
	host->start();

	// Create the application and register the GlobalMonitor.
	QScopedPointer<QGuiApplication> app(SailfishApp::application( argc, argv ) );
	app->setQuitOnLastWindowClosed( true );
	QScopedPointer<QQuickView> view( SailfishApp::createView() );
	view->engine()->rootContext()->setContextProperty( "GlobalMonitor", monitor.data() );
	host->setQmlEngine( view->engine() );

	// Open UI.
	view->setSource( SailfishApp::pathTo( "qml/MFiles-Sailfish.qml" ) );
	view->showFullScreen();
    
	int result = app->exec();;	

	// Shutdown the host core thread.
	// Though it is highly likely that Sailfish OS will kill us soon...
	host->quit();
	host->wait();

	return result;
}


