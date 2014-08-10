/*
 *  Copyright 2014 Juha Lepola
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

#include "qmlsortfilterproxymodel.h"

#include <QJSValue>
#include <QJSValueList>

#include "../backend/hostcore.h"
#include "dataaccessorsource.h"

QmlSortFilterProxyModel::QmlSortFilterProxyModel( QObject* parent ) :
	QSortFilterProxyModel( parent ),
	m_dataAccessor( nullptr )
{
	QObject::connect( this, &QmlSortFilterProxyModel::sourceModelChanged, this, &QmlSortFilterProxyModel::updateDataAccessorSource );
}

void QmlSortFilterProxyModel::sortByDefaults()
{
	// Delegate.
	QSortFilterProxyModel::sort( 0, Qt::AscendingOrder );
}

void QmlSortFilterProxyModel::setLessThanJS( const QJSValue& lessThanFunction )
{
	Q_ASSERT( lessThanFunction.isUndefined() || lessThanFunction.isCallable() );
	m_lessThanJS = lessThanFunction;
	emit lessThanJSChanged();
}

void QmlSortFilterProxyModel::componentComplete()
{
	QMetaObject::invokeMethod( this, "sortByDefaults", Qt::QueuedConnection );
}

bool QmlSortFilterProxyModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{
	// Delegate to base class if our comparison function has not been defined.
	if( ! m_lessThanJS.isCallable() || m_dataAccessor == nullptr )
		return QSortFilterProxyModel::lessThan( left, right );

	// Prepare parameters and execute the lessThan.
	QJSValue execute = m_lessThanJS;
	QJSValueList args;
	args.push_back( HostCore::toJSValue( m_dataAccessor->createAccessor( left ) ) );
	args.push_back( HostCore::toJSValue( m_dataAccessor->createAccessor( right ) ) );
	QJSValue resultValue = execute.call( args );
	bool result = resultValue.toBool();
	return result;
}

void QmlSortFilterProxyModel::updateDataAccessorSource()
{
	m_dataAccessor = nullptr;
	if( this->sourceModel() != nullptr )
		m_dataAccessor = dynamic_cast< DataAccessorSource* >( this->sourceModel() );
}
