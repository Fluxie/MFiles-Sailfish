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

#ifndef TYPEDVALUEFILTER_H
#define TYPEDVALUEFILTER_H

#include <QObject>

#include "../mfiles/objver.h"
#include "../mfiles/typedvalue.h"
#include "lazyownerinfo.h"

/**
 * @brief The TypedValueFilter class specifies filter for value list items when they are fetched from the server.
 */
class TypedValueFilter : public QObject
{
	Q_OBJECT
	Q_PROPERTY( int objectType READ objectType NOTIFY objectTypeChanged )
	Q_PROPERTY( QJsonValue ownerInfo READ ownerInfo NOTIFY ownerInfoChanged )
public:

	/**
	 * @brief The particular filter type is undefined.
	 */
	static const int Undefined = INT_MIN;

	/**
	 * @brief Initializes new TypedValueFilter object.
	 * @param parent Parent for the filter.
	 */
	explicit TypedValueFilter( QObject *parent = nullptr );

	/**
	 * @brief Copy-constructor
	 * @param other Copied filter object.
	 * @param parent Parent for the copy.
	 */
	TypedValueFilter( const TypedValueFilter& other, QObject *parent = 0 ) :
		QObject( parent ),
		m_enabled( other.m_enabled ),
		m_objectType( other.m_objectType ),
		m_propertyDef( other.m_propertyDef ),
		m_ownerInfo( other.m_ownerInfo ? new LazyOwnerInfo( *other.m_ownerInfo, this ) : 0 ),
		m_objver( other.m_objver ),
		m_currentValue( other.m_currentValue )
	 {}

	/**
	 * @brief ~TypedValueFilter
	 */
	virtual ~TypedValueFilter() {}

	/**
	 * @brief Initializes new typed value filter for property definition.
	 * @param propertyDef Property definition.
	 * @return Filter for property definition.
	 */
	static TypedValueFilter* forPropertyDefinition( int propertyDef );

	/**
	 * @brief Initializes new typed value filter for property definition.
	 * @param propertyDef Property definition.	 
	 * @param resolver The owner resolver that will be used to determine the ownership info.
	 * @param objver The current object version
	 * @param currentValue The id of the currently selected item.
	 * @return Filter for property definition with owner filter.
	 */
	static TypedValueFilter* forPropertyDefinition( int propertyDef, LazyOwnerInfo::RESOLVER_T ownerInfoResolver, const QJsonValue& objver, const QJsonValue& currentValue );

	/**
	 * @brief Is this filter enabled?
	 * @return True if the filter is enabled.
	 */
	bool enabled() const { return m_enabled; }

	/**
	 * @brief Gets the object type.
	 * @return The object type.
	 */
	int objectType() const { return m_objectType; }

	/**
	 * @brief Gets the property definition.
	 * @return The property definition.
	 */
	int propertyDef() const { return m_propertyDef; }

	/**
	 * @brief Gets the ownerhip information for filtering.
	 * @return Ownership information for filtering.
	 */
	QJsonValue ownerInfo() const;

	/**
	 * @brief Gets the object version.
	 * @return ObjVer that represents the current object version for which listing is displayed.
	 */
	QJsonValue objver() const { return m_objver; }


	/**
	 * @brief Gets the value that is currently selected for filtering purposes.
	 * @return The currently selected value
	 */
	QJsonValue currentValue() const { return m_currentValue; }

signals:

	/**
	 * @brief This signal is emitted when the object type changes.
	 */
	void objectTypeChanged();

	/**
	 * @brief This signal is emitted when the ownership information in this filter changes.
	 */
	void ownerInfoChanged();

public slots:

	/**
	 * @brief Sets the object type.
	 * @param objectType Object type for filtering.
	 */
	void setObjectType( int objectType );

// Private data.
private:

	bool m_enabled;  //!< Is this filter enabled?
	int m_objectType;  //!< Object type for filtering.
	int m_propertyDef;  //!< Property definition for filtering.
	LazyOwnerInfo* m_ownerInfo;  //!< Owner for filtering.
	QJsonValue m_objver;  //!< The object version of the object.
	QJsonValue m_currentValue;  //!< The item currently selected for the property definition.

};

Q_DECLARE_METATYPE( TypedValueFilter )

/**
 * @brief Compares TypedValueFilter objects for equality.
 * @param left Left operand.
 * @param right Right operand.
 * @return True if the operands are equal.
 */
inline bool operator==( const TypedValueFilter &left, const TypedValueFilter &right )
{
	return left.enabled() == right.enabled()
			&& left.objectType() == right.objectType()
			&& left.propertyDef() == right.propertyDef()
			&& left.ownerInfo() == right.ownerInfo()
			&& left.objver() == right.objver()
			&& left.currentValue() == right.currentValue();
}

/**
 * @brief Hash function for TypedValueFilter object.
 * @param key The TypedValueFilter object for which the hash value is calculated.
 * @return Hash value of the TypedValueFilter object.
 */
inline uint qHash( const TypedValueFilter* key )
{
	if( key == 0 )
		return 0;
	uint hash = key->propertyDef() ^ key->objectType()
				^ qHash( MFiles::TypedValue( key->ownerInfo() ) );
	if( ! key->objver().isNull() )
		hash ^= qHash( MFiles::ObjVer( key->objver() ) );
	if( ! key->currentValue().isNull() )
		hash ^= qHash( MFiles::TypedValue( key->currentValue() ) );
	return key->enabled() ? hash : ~hash;
}

#endif // TYPEDVALUEFILTER_H
