#ifndef LISTINGSTATUS_H
#define LISTINGSTATUS_H

#include <QObject>

/**
 * @brief The ListingStatus class is a wrapper for enumerations related to view listing or listing.
 */
class ListingStatus : public QObject
{
	Q_OBJECT	
	Q_ENUMS( Status )
public:

	/**
	 * @brief Initializes new ListingStatus object
	 * @param parent
	 */
	explicit ListingStatus( QObject *parent = 0 );

	enum Status
	{
		Disconnected,  //!< The value list does not have a connection to the core.
		Refreshing,  //!< A new values are being searched for the listing. Old values are still available.
		Ready  //!< The listing is ready and no operations are running.
	};

signals:

public slots:

};

#endif // LISTINGSTATUS_H
