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

#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QtQml>
#include <QStringListModel>

#include <sailfishapp.h>

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "appmonitor.h"
#include "asyncfetch.h"
#include "errormodel.h"
#include "errorinfo.h"
#include "hostcore.h"
#include "integervalidator.h"
#include "objectfront.h"
#include "objectversionfront.h"
#include "lazyownerinfo.h"
#include "lookuplistmodel.h"
#include "propertyvaluelistmodel.h"
#include "realvalidator.h"
#include "typedvaluefilter.h"
#include "vaultfront.h"
#include "valuelistitemlistmodel.h"
#include "valuelistfront.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{	
	// REgister metatypes.
	qRegisterMetaType< ErrorInfo >();

	// Register C++ classes as QML types.	
	qmlRegisterType< AppMonitor >("mohari.sailfish", 1, 0, "AppMonitor");
	qmlRegisterType< ErrorModel >("mohari.sailfish", 1, 0, "ErrorModel");
	qmlRegisterType< LookupListModel >("mohari.sailfish", 1, 0, "LookupListModel");
	qmlRegisterType< TypedValueFilter >("mohari.sailfish", 1, 0, "TypedValueFilter");
	qmlRegisterType< PropertyValueListModel >("mohari.sailfish", 1, 0, "PropertyValueListModel");
	qmlRegisterType< ValueListItemListModel >("mohari.sailfish", 1, 0, "ValueListItemListModel");
	qmlRegisterType< VaultFront >("mohari.sailfish", 1, 0, "VaultFront");
	qmlRegisterType< IntegerValidator >("mohari.sailfish", 1, 0, "IntegerValidator");
	qmlRegisterType< RealValidator >("mohari.sailfish", 1, 0, "RealValidator");
	qmlRegisterType< AsyncFetch >();
	qmlRegisterType< LazyOwnerInfo >();
	qmlRegisterType< ObjectFront >();
	qmlRegisterType< ObjectVersionFront >();
	qmlRegisterType< ValueListFront >();

	// Register certain QT specific types in order to use them in QML.
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
	view->setSource( SailfishApp::pathTo( "qml/MFiles-Sailfish.qml" ) );
	view->showFullScreen();
    
	int result = app->exec();;	

	// Shutdown the host core thread.
	// Though it is highly likely that Sailfish OS will kill us soon...
	host->quit();
	host->wait();

	return result;
}


