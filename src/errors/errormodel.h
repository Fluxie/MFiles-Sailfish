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

/**
 * @brief The ErrorModel class provides a model for UI for displaying an error.
 */
class ErrorModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY( QString message READ message NOTIFY messageChanged )
	Q_PROPERTY( QStringListModel* stack READ stack NOTIFY stackChanged )
public:

	/**
	 * @brief Initializes empty error model. Required for Qt metatype system.
	 * @param parent Owner object.
	 */
	explicit ErrorModel(QObject *parent = 0);

	/**
	 * @brief Initializes new ErrorModel object based on the given error.
	 * @param error Description of the error.
	 */
	explicit ErrorModel( const ErrorInfo& error );

	/**
	 * @brief Gets the error message.
	 * @return Returns the error message.
	 */
	QString message() const { return m_error.message(); }

	/**
	 * @brief Gets the error stack.
	 * @return Returns the error stack.
	 */
	QStringListModel* stack() const;

signals:

	/**
	 * @brief This signal is emitted when the message changes.
	 */
	void messageChanged();

	/**
	 * @brief This signal is emitted when the stack changes.
	 */
	void stackChanged();

public slots:

private:

	/**
	 * @brief Fills the stack based on the current error object.
	 */
	void fillStack();

// Private data
private:

	ErrorInfo m_error;
	QStringList m_stack;

};

#endif // ERRORMODEL_H
