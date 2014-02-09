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
#include <QVector>

class AsyncFetch : public QObject
{
	Q_OBJECT
public:

	typedef std::function< bool( const QJsonValue& ) > FILTER_T;

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
	QJsonArray values() const;

	/**
	 * @brief state
	 * @return State of the fetching.
	 */
	State state() const { return m_state; }

	/**
	 * @brief appendFilter Appends new filter to the filters.
	 * @param filter Filter to append to the list of existing filters.
	 */	
	void appendFilter( FILTER_T filter ) { m_filters.push_back( FILTER_T( filter ) ); m_filteringUpToDate = false; }

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
	 * @brief itemsFetched is signaled when fetching items for the whole list completes.
	 * @param cookie The cookie that identifies the fetch request
	 * @param values The fetched items.
	 */
	void itemsFetched( int cookie, const QJsonArray& values );

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

// Private interface.
private:

	/**
	 * @brief applyFilter
	 * @param values Values to be filtered.
	 * @return Filtered values
	 */
	QJsonArray applyFilter( const QJsonArray& values ) const;

// Private data.
private:

	State m_state;  //!< The state of the fetch operation.
	int m_cookie;  //!< The cookie that identifies the fetch request.
	QJsonArray m_values;  //!< The fetched value.

	mutable bool m_filteringUpToDate;  //!< True when filtering of the values is up-to-date.
	mutable QJsonArray m_filteredValues;  //!< Filtered values.

	QVector< FILTER_T > m_filters;  //!< Filter function for filttering the accepted values.

};


#endif // ASYNCFETCH_H
