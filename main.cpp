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

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include "sailfishapplication.h"
#include "appmonitor.h"
#include "errorinfo.h"
#include "hostcore.h"
#include "integervalidator.h"
#include "objectfront.h"
#include "objectversionfront.h"
#include "realvalidator.h"
#include "vaultfront.h"
#include "valuelistmodel.h"
#include "valuelistfront.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{	
	// REgister metatypes.
	qRegisterMetaType< ErrorInfo >();

	// Register C++ classes as QML types.	
	qmlRegisterType< ValueListModel >("mohari.sailfish", 1, 0, "ValueListModel");
	qmlRegisterType< VaultFront >("mohari.sailfish", 1, 0, "VaultFront");
	qmlRegisterType< IntegerValidator >("mohari.sailfish", 1, 0, "IntegerValidator");
	qmlRegisterType< RealValidator >("mohari.sailfish", 1, 0, "RealValidator");
	qmlRegisterType< ObjectFront >();
	qmlRegisterType< ObjectVersionFront >();
	qmlRegisterType< ValueListFront >();
	qmlRegisterType< AppMonitor >();

    QScopedPointer<QGuiApplication> app(Sailfish::createApplication(argc, argv));
	QScopedPointer<QQuickView> view( Sailfish::createView("main.qml") );

    // Create host core.
	QScopedPointer< HostCore > host( new HostCore() );
	host->start();
    
    Sailfish::showView(view.data());
    
	int result = app->exec();;	

	// Shutdown the host core thread.
	// Though it is highly likely that Sailfish OS will kill us soon...
	host->quit();
	host->wait();

	return result;
}


