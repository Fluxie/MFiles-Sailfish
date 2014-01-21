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

#ifndef LAZYOWNERINFO_H
#define LAZYOWNERINFO_H

#include <QJsonValue>
#include <QModelIndex>
#include <QObject>

#include "mfiles/typedvalue.h"

// Forward declarations.
class PropertyValueOwnerResolver;

/**
 * @brief The LazyOwnerInfo class represents all owner values of one property value.
 *
 * The actual ownership information is not resolved until it is required.
 */
class LazyOwnerInfo : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QJsonValue ownerInfo READ ownerInfo NOTIFY ownerInfoChanged )
public:

	/**
	 * @brief LazyOwnerInfo
	 * @param parent
	 */
	explicit LazyOwnerInfo( const QModelIndex& subItem, PropertyValueOwnerResolver* resolver );

	/**
	 * @brief Constructs a copy of LazyOwnerInfo object.
	 * @param source The source of the copy.
	 */
	explicit LazyOwnerInfo( const LazyOwnerInfo& source, QObject* parent );

	/**
	 * @brief owner
	 * @return The owner info.
	 */
	QJsonValue ownerInfo() const;

// Statics.
public:

	static bool Compare( const LazyOwnerInfo* left, const LazyOwnerInfo* right )
	{
		// Compare pointers.
		if( left == 0 && right == 0 )
			return true;
		else if( ( left == 0 && right != 0 ) ||
			( left != 0 && right == 0 ) )
			return false;

		return left->ownerInfo() == right->ownerInfo();
	}

signals:

	/**
	 * @brief ownerInfoChanged is signaled when the ownership information changes.
	 */
	void ownerInfoChanged();

public slots:

private:

	QModelIndex m_subItem;  //!< The item of which ownership information this object represents.
	PropertyValueOwnerResolver* m_resolver;  //!< Owner resolved from which the owner information is actually requested.
	mutable bool m_cached;  //!< Set to true after the ownership information has been cached.
	mutable QJsonValue m_cachedOwnership;  //!< The ownership information is store here for faster access after it is requested the first time.

};

inline bool operator==( const LazyOwnerInfo &left, const LazyOwnerInfo &right )
{
	return left.ownerInfo() == right.ownerInfo();
}

inline uint qHash( const LazyOwnerInfo* ownerInfo )
{
	if( ownerInfo == 0 )
		return 0;
	uint hash = qHash( TypedValue( ownerInfo->ownerInfo() ) );
	return hash;
}

#endif // LAZYOWNERINFO_H
