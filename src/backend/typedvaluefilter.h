#ifndef TYPEDVALUEFILTER_H
#define TYPEDVALUEFILTER_H

#include <QObject>

#include "../mfiles/typedvalue.h"
#include "lazyownerinfo.h"

/**
 * @brief The TypedValueFilter class
 */
class TypedValueFilter : public QObject
{
	Q_OBJECT
	Q_PROPERTY( int objectType READ objectType NOTIFY objectTypeChanged )
	Q_PROPERTY( QJsonValue ownerInfo READ ownerInfo NOTIFY ownerInfoChanged )
public:

	//! The particular filter type is undefined.
	static const int Undefined = INT_MIN;

	explicit TypedValueFilter(QObject *parent = 0);	

	// Copy constructor.
	TypedValueFilter( const TypedValueFilter& other, QObject *parent = 0 ) :
		QObject( parent ),
		m_enabled( other.m_enabled ),
		m_objectType( other.m_objectType ),
		m_propertyDef( other.m_propertyDef ),
		m_ownerInfo( other.m_ownerInfo ? new LazyOwnerInfo( *other.m_ownerInfo, this ) : 0 )
	 {}

	//! Destructor.
	virtual ~TypedValueFilter() {}

	/**
	 * @brief Initializes new typed value filter for property definition.
	 * @param propertyDef Property definition.
	 * @return
	 */
	static TypedValueFilter* forPropertyDefinition( int propertyDef );

	/**
	 * @brief Initializes new typed value filter for property definition.
	 * @param propertyDef Property definition.
	 * @param index The index of this item.
	 * @param resolver The owner resolver that will be used to determine the ownership info.
	 * @return TypedValueFilter
	 */
	static TypedValueFilter* forPropertyDefinition( int propertyDef, LazyOwnerInfo::RESOLVER_T ownerInfoResolver );

	//! Is this filter enabled?
	bool enabled() const { return m_enabled; }

	//! Gets the object type.
	int objectType() const { return m_objectType; }

	//! Gets the property definition.
	int propertyDef() const { return m_propertyDef; }

	/**
	 * @brief ownerInfo
	 * @return Ownership information for filtering.
	 */
	QJsonValue ownerInfo() const;

signals:

	//! Signaled when the object type changes.
	void objectTypeChanged();

	/**
	 * @brief Signaled when the ownership information in this filter changes.
	 */
	void ownerInfoChanged();

public slots:

	//! Sets the object type.
	void setObjectType( int objectType );

// Private data.
private:

	bool m_enabled;  //!< Is this filter enabled?
	int m_objectType;  //!< Object type for filtering.
	int m_propertyDef;  //!< Property definition for filtering.
	LazyOwnerInfo* m_ownerInfo;  //!< Owner for filtering.

};

Q_DECLARE_METATYPE( TypedValueFilter )

inline bool operator==( const TypedValueFilter &left, const TypedValueFilter &right )
{
	return left.enabled() == right.enabled()
			&& left.objectType() == right.objectType()
			&& left.propertyDef() == right.propertyDef()
			&& left.ownerInfo() == right.ownerInfo();
}

inline uint qHash( const TypedValueFilter* key )
{
	if( key == 0 )
		return 0;
	uint hash = key->propertyDef() ^ key->objectType() ^ qHash( MFiles::TypedValue( key->ownerInfo() ) );
	return key->enabled() ? hash : ~hash;
}

#endif // TYPEDVALUEFILTER_H
