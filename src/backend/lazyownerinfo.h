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
#include <QObject>

#include "../mfiles/typedvalue.h"

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
	 * @brief Definition for function for resolving JSON values.
	 */
	typedef std::function< QJsonValue() > RESOLVER_T;

	/**
	 * @brief LazyOwnerInfo
	 * @param parent
	 */
	explicit LazyOwnerInfo( RESOLVER_T ownerInfoResolver );

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

	/**
	 * @brief Compares two LazyOwnerInfo objects.
	 * @param left Left operand.
	 * @param right Right operand.
	 * @return True if the operands are equal.
	 */
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
	 * @brief This signal is emitted when the ownership information changes.
	 */
	void ownerInfoChanged();

public slots:

private:

	RESOLVER_T m_ownerInfoResolver;  //!< Function for resolving the owner info.
	mutable bool m_cached;  //!< Set to true after the ownership information has been cached.
	mutable QJsonValue m_cachedOwnership;  //!< The ownership information is store here for faster access after it is requested the first time.

};

/**
 * @brief Comparison operator for LazyOwnerInfo objects.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if the operands are equal.
 */
inline bool operator==( const LazyOwnerInfo &left, const LazyOwnerInfo &right )
{
	return left.ownerInfo() == right.ownerInfo();
}

/**
 * @brief Hash function for LazyOwnerInfo object.
 * @param ownerInfo The LazyOwnerInfo object for which the hash value is calculated.
 * @return Hash value of the LazyOwnerInfo object
 */
inline uint qHash( const LazyOwnerInfo* ownerInfo )
{
	if( ownerInfo == 0 )
		return 0;
	uint hash = qHash( MFiles::TypedValue( ownerInfo->ownerInfo() ) );
	return hash;
}

#endif // LAZYOWNERINFO_H
