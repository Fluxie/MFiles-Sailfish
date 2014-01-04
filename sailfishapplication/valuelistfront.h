#ifndef VALUELISTFRONT_H
#define VALUELISTFRONT_H

#include <QJsonArray>
#include <QObject>

#include "frontbase.h"
#include "objecttypecache.h"
#include "valuelistcore.h"

// Forward declarations.
class VaultCore;

class ValueListFront : public FrontBase
{
	Q_OBJECT
	Q_ENUMS( Status )
	Q_PROPERTY( Status status READ status NOTIFY statusChanged )
public:

	enum Status
	{
		Disconnected,  //!< The value list does not have a connection to the core.
		Empty,  //!< The value list is empty.
		Searching,  //!< A new values are being searched for the value list. Old values are still available.
		Ready  //!< The value list is ready and no operations are running.
	};

	//! Initializes new value list front.
	explicit ValueListFront(
		VaultCore* vault,  //!< Vault.
		ValueListCore* core  //!< Core
	);

	//! Value list items.
	Q_INVOKABLE QJsonArray items();

	//! Status.
	Status status();

signals:

	//! Signaled when the status of the value list has changed.
	void statusChanged();

public slots:

// Protected interface.
protected:

	//! Does this front accept the specified core.
	virtual bool accept( QObject* core ) const;

// Private interface.
private:

	//! Value list core.
	ValueListCore* valueList() { return qobject_cast< ValueListCore* >( this->core() ); }

// Private data.
private:

	int m_id;  //!< The id this value list represents.
	int m_propertyDefinition;  //!< The property definition this value list is filtered with.
	VaultCore* m_vault;		

};

#endif // VALUELISTFRONT_H
