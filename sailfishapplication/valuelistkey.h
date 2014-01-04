#ifndef VALUELISTKEY_H
#define VALUELISTKEY_H

class ValueListKey
{
public:

	//! Constructor.
	ValueListKey( int idIn, int propertyDefIn ) :
		id( idIn ),
		propertyDef( propertyDefIn )
	{}

	//! Copy constructore.
	explicit ValueListKey( const ValueListKey& key ) :
		id( key.id ),
		propertyDef( key.propertyDef )
	{}

	int id;
	int propertyDef;
};

inline bool operator==(const ValueListKey &left, const ValueListKey &right)
{
	return left.id == right.id
		   && left.propertyDef == right.propertyDef;
}

inline uint qHash(const ValueListKey &key)
{
	return qHash( key.id) ^ key.propertyDef;
}



#endif // VALUELISTKEY_H
