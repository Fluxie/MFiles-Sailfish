#ifndef CLASSESFRONT_H
#define CLASSESFRONT_H

#include <QJsonArray>
#include <QSet>

#include "valuelistfront.h"

// Forward declarations.
class ValueListCore;
class VaultCore;

/**
 * @brief The ClassesFront class
 */
class ClassesFront : public ValueListFront
{
Q_OBJECT
public:

	//! Initializes new value list front.
	explicit ClassesFront(
		VaultCore* vault,  //!< Vault.
		ValueListCore* core,  //!< Core
		int objectType  //!< The object type used to filter classes.
	);

	//! Value list items.
	Q_INVOKABLE virtual AsyncFetch* items();

signals:

	//! Signaled when the object type has changed.
	void objectTypeChanged();

public slots:

	//! Refreshes the available classes from the vault.
	void refreshClasses();

// Private data.
private:

	int m_objectType;  //!< The object the classes in this list are intended for.
	QSet< int > m_classes;  //!< The classes of the current object type.
};

#endif // CLASSESFRONT_H
