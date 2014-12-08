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

/**
 * @brief AsyncFetch objects represent an on-going asynchronous retrieval operations from M-Files REST API.
 *
 * @todo Improve the explanation.
 */
class AsyncFetch : public QObject
{
	Q_OBJECT
public:

	/**
	 * @brief Type definition for a filter.
	 */
	typedef std::function< bool( const QJsonValue& ) > FILTER_T;

	/**
	  * @brief Type definition for a result converter function.
	  */
	typedef std::function< QJsonArray( const QJsonArray& ) > RESULTCONVERTER_T;

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
	 * @brief Initializes new AsyncFetch object.
	 * @param cookie The cookie that identifies the fetch request.
	 */
	explicit AsyncFetch( int cookie );

	/**
	 * @brief Gets the fetched value.
	 * @remarks { Call only when the fetch operations returns at maximum one value. }
	 * @return Retrieves the value.
	 */
	QJsonValue value() const;

	/**
	 * @brief Gets all the fetched values.
	 * @return Retrieves the values.
	 */
	QJsonArray values() const;

	/**
	 * @brief Gets the state of the fetch operation.
	 * @return State of the fetching.
	 */
	State state() const { return m_state; }

	/**
	 * @brief Appends new filter to the filters.
	 * @param filter Filter to append to the list of existing filters.
	 */	
	void appendFilter( FILTER_T filter ) { m_filters.push_back( FILTER_T( filter ) ); m_processedValuesUpToDate = false; }

	/**
	 * @brief Sets new converter function.
	 * @param converter The new converter function.
	 */
	void setResultConverter( RESULTCONVERTER_T converter ) { m_resultConverter = converter; m_processedValuesUpToDate = false; }

signals:

	/**
	 * @brief Signaled when the fecth operation completes.
	 */
	void finished();

	/**
	 * @brief Signaled when error has occurred during fetching.
	 */
	void error();

public slots:

	/**
	 * @brief Notifies the AsyncFetch that fetching a value has completed.
	 * @param cookie The cookie that identifies the fetch request.
	 * @param value  The fetched value.
	 */
	void itemFetched( int cookie, const QJsonValue& value );

	/**
	 * @brief Notifies the AsyncFetch that fetching of items is complete.
	 * @param cookie The cookie that identifies the fetch request
	 * @param values The fetched items.
	 */
	void itemsFetched( int cookie, const QJsonArray& values );

	/**
	 * @brief Rerpots an error during the fetch operation.
	 * @param cookie The cookie that identifies the fetch request.
	 */
	void reportError( int cookie );

// Protected interface.
protected:

	/**
	 * @brief This virtual function is called when something has been connected to signal in this object.
	 * @param signal An slot was connected to this signal of this object.
	 */
	virtual void connectNotify( const QMetaMethod& signal );

// Private interface.
private:

	/**
	 * @brief Applies filter for the given values.
	 * @param values Values to be filtered.
	 * @return Filtered values
	 */
	QJsonArray applyFilter( const QJsonArray& values ) const;

	/**
	 * @brief Applies the filter and performs the possible result conversion.
	 */
	void processValues() const;

// Private data.
private:

	State m_state;  //!< The state of the fetch operation.
	int m_cookie;  //!< The cookie that identifies the fetch request.
	QJsonArray m_values;  //!< The fetched value.

	mutable bool m_processedValuesUpToDate;  //!< True when filtering and processing of the values is up-to-date.
	mutable QJsonArray m_processedValues;  //!< Filtered and processed values.

	QVector< FILTER_T > m_filters;  //!< Filter function for filttering the accepted values.
	RESULTCONVERTER_T m_resultConverter;  //!< Converter function for processing the accepted values.

};


#endif // ASYNCFETCH_H
