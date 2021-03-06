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

#include "customroleproxymodel.h"

#include <QJsonObject>

#include "../backend/hostcore.h"

CustomRoleProxyModel::CustomRoleProxyModel(QObject *parent) :
	QIdentityProxyModel( parent ),
	m_customRoleId( 0 ),
	m_customRoleName( "custom" )
{
}

QVariant CustomRoleProxyModel::data( const QModelIndex &index, int role ) const
{
	// Is the specified role defined by us?
	if( role != m_customRoleId )
	  return QIdentityProxyModel::data( index, role );

	// Get the data of the role
	QJSValue parameters = HostCore::newJSObject();
	for( QHash< int, QByteArray >::const_iterator itr = m_sourceRoleNames.constBegin();
		 itr != m_sourceRoleNames.constEnd(); itr++ )
	{
		QVariant roleData = QIdentityProxyModel::data( index, itr.key() );
		QMetaType::Type type = ( QMetaType::Type )roleData.type();
		switch( type )
		{
		case QMetaType::QString :
			parameters.setProperty( QString( itr.value() ), roleData.toString() );
			break;

		case QMetaType::Int :
			parameters.setProperty( QString( itr.value() ), roleData.toInt() );
			break;

		// Default to conversion.
		default:
			parameters.setProperty( QString( itr.value() ), HostCore::toJSValue( roleData ) );
			break;
		}
	}
	Q_ASSERT( parameters.isObject() );
	Q_ASSERT( ! parameters.isNull() );
	Q_ASSERT( ! parameters.isUndefined() );

	// Execute the function. Take a copy of the function to prevent it from altering its state.
	QJSValue execute = m_valueFunction;
	QJSValueList args;
	args.push_back( parameters );
	QJSValue result = execute.call( args );
	return result.toVariant();
}

QHash< int, QByteArray > CustomRoleProxyModel::roleNames() const
{
	// Get the role names of the parent class.
	// QHash< int, QByteArray > roles;
	QHash< int, QByteArray > roles = QIdentityProxyModel::roleNames();
	m_sourceRoleNames = roles;

	// Determine the next allowed id.
	int roleIdCandidate = Qt::UserRole;
	while( roles.contains( roleIdCandidate ) )
	{
		// Try next candidate.
		roleIdCandidate++;
	}
	m_customRoleId = roleIdCandidate;
	emit customRoleIdChanged();

	// Add our custom role and return.
	roles.insert( m_customRoleId, m_customRoleName.toLatin1() );
	return roles;
}

void CustomRoleProxyModel::setCustomRoleName( const QString& roleName )
{
	Q_ASSERT( m_customRoleId == 0 );

	m_customRoleName = roleName;
	emit customRoleNameChanged();
}

void CustomRoleProxyModel::setCustomRoleValue( const QJSValue& valueFunction )
{
	Q_ASSERT( valueFunction.isCallable() );
	this->beginResetModel();
	{
		m_valueFunction = valueFunction;
	}
	this->endResetModel();
	emit customRoleValueChanged();
}
