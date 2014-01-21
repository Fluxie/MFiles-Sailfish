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

#ifndef ASYNCFETCH_H
#define ASYNCFETCH_H

#include <QHash>
#include <QJsonArray>
#include <QJsonValue>
#include <QMetaMethod>
#include <QObject>

class AsyncFetch : public QObject
{	
	Q_OBJECT
public:

	/**
	 * @brief The State enum identifying the current state of the fetch operation.
	 */
	enum State
	{
		Fetching,
		Finished,
		Error
	};

	/**
	 * @brief AsyncFetch
	 * @param cookie The cookie that identifies the fetch request.
	 */
	explicit AsyncFetch( int cookie );

	/**
	 * @brief value
	 * @return Retrieves the value.
	 */
	QJsonValue value() const;

	/**
	 * @brief value
	 * @return Retrieves the values.
	 */
	QJsonArray values() const { return m_values; }

	/**
	 * @brief state
	 * @return State of the fetching.
	 */
	State state() const { return m_state; }

signals:

	/**
	 * @brief finished
	 */
	void finished();

	/**
	 * @brief Signaled when error has occurred during fetching.
	 */
	void error();

public slots:

	/**
	 * @brief itemFetched is called when the value becomes available.
	 * @param cookie The cookie that identifies the fetch request.
	 * @param value  The fetched value.
	 */
	void itemFetched( int cookie, const QJsonValue& value );

	/**
	 * @brief error
	 * @param cookie The cookie that identifies the fetch request.
	 */
	void reportError( int cookie );

// Protected interface.
protected:

	/**
	 * @brief This virtual function is called when something has been connected to signal in this object.
	 * @param signal
	 */
	virtual void connectNotify( const QMetaMethod& signal );

// Private data.
private:

	State m_state;  //!< The state of the fetch operation.
	int m_cookie;  //!< The cookie that identifies the fetch request.
	QJsonArray m_values;  //!< The fetched value.

};


#endif // ASYNCFETCH_H
