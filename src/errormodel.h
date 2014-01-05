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

#ifndef ERRORMODEL_H
#define ERRORMODEL_H

#include <QObject>
#include <QStringListModel>

#include "errorinfo.h"

class ErrorModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QString message READ message NOTIFY messageChanged )
	Q_PROPERTY( QStringListModel* stack READ stack NOTIFY stackChanged )
public:

	//! Constructrs empty error model.
	explicit ErrorModel(QObject *parent = 0);

	//! Constructrs an error model based on the given error.
	explicit ErrorModel( const ErrorInfo& error );

	//! Gets the error message.
	QString message() const { return m_error.message(); }

	//! Gets the error stack.
	QStringListModel* stack() const;

signals:

	//! Signaled when the message changes.
	void messageChanged();

	//! Signaled when the stack changes.
	void stackChanged();

public slots:

// Private interface.
private:

	//! Fills the stack based on the current error object.
	void fillStack();

// Private data
private:

	ErrorInfo m_error;
	QStringList m_stack;

};

#endif // ERRORMODEL_H
