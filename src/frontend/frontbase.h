/*
 *  Copyright 2013 Juha Lepola
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

#ifndef CORECONSTRUCTOR_H
#define CORECONSTRUCTOR_H

#include <QObject>

class VaultFront;

//! A helper class for managing the construction of core objects.
class FrontBase : public QObject
{
Q_OBJECT

// Public interface.
public:

	//! Initializes new FrontBase object.
	explicit FrontBase() : m_core( nullptr ) {}

	//! Initializes new FrontBase object.
	explicit FrontBase( QObject* core ) : m_core( core ) { }

	/**
	 * @brief Initializes new FrontBase object.
	 * @param core Core object
	 * @param parent Parent.
	 */
	explicit FrontBase( QObject* core, QObject* parent  );

	/**
	 * @brief Checks if this object is part of the given vault.
	 * @param vault The vault
	 * @return True if this front belongs to the given vault
	 */
	bool isPartOf( const VaultFront* vault ) const;

	/**
	 * @brief Sets new core.
	 * @param core The new core.
	 */
	void setCore( QObject* core );

signals:

	/**
	 * @brief coreChanged is signaled when the core has changed.
	 */
	void coreChanged();

public slots:

	//! A core of the given type has become available.
	void coreAvailable( QObject* core, QObject* source );	

// Protected interface.
protected:

	//! Does this front accept the specified core.
	virtual bool accept( QObject* core ) const = 0;

	//! Accesses the core object.
	QObject* core() { return m_core; }

	//! Accesses the core object.
	const QObject* coreConst() const { return m_core; }

// Pritvate interface.
private:

	//! Sets the core.
	void setCore( QObject* core, QObject* source );

// Private data:
private:

	QObject* m_core;
};


#endif // CORECONSTRUCTOR_H
