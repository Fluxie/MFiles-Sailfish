#ifndef VAULTLISTING_H
#define VAULTLISTING_H

#include <QNetworkReply>
#include <QObject>
#include <QString>

// Forward declarations.
class MfwsRest;

class VaultListing : public QObject
{
	Q_OBJECT
	Q_ENUMS( Status )
	Q_PROPERTY( QString host READ host WRITE setHost NOTIFY hostChanged )
	Q_PROPERTY( QString username READ username WRITE setUsername NOTIFY usernameChanged )
	Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )
	Q_PROPERTY( Status status READ status NOTIFY statusChanged )
public:

	enum Status
	{
		Initialized,  //!< The listing has been created but not enough data is available for fetching the listing.
		Waiting,  //!< The listing is waiting for the fetching to start.
		Fetching,  //!< The model is fetching data from the server.
		Completed,  //!< Fetching of the vault list has completed.
		Error  //!< There was an error while fetching the listing.
	};

	/**
	 * @brief Initializes new VaultListing object.
	 * @param parent
	 */
	explicit VaultListing(QObject *parent = 0);

	/**
	 * @brief host set for the model.
	 * @return The host
	 */
	QString host() const;

	/**
	 * @brief Gets the username set for accessing the vault listing.
	 * @return Username set for accessing the vault listing.
	 */
	QString username() const;

	/**
	 * @brief Gets the password set for accessing the vault listing.
	 * @return Password set for accessing the vault listing.
	 */
	QString password() const;

	/**
	 * @brief Gets the status of the model.
	 * @return Status of the listing.
	 */
	Status status() const;

signals:

	/**
	 * @brief hostChanged is signaled when the host of the model changes.
	 */
	void hostChanged();

	/**
	 * @brief usernameChanged is signaled when the username of the model changes.
	 */
	void usernameChanged();

	/**
	 * @brief passwordChanged is signaled when the password of the model changes.
	 */
	void passwordChanged();

	/**
	 * @brief statusChanged is signaled when status of the listing changes.
	 */
	void statusChanged();

public slots:

	/**
	 * @brief Sets the host for the model.
	 * @param New host.
	 */
	void setHost( const QString& host );

	/**
	 * @brief Sets the username for the model.
	 * @param New username.
	 */
	void setUsername( const QString& username );

	/**
	 * @brief Sets the host for the model.
	 * @param New host.
	 */
	void setPassword( const QString& password );

private slots:

	/**
	 * @brief Fetches the listing from the server.
	 */
	void fetchListing();

private slots:

	/**
	 * @brief Reports a network error that occurred within the core.
	 * @param code Error code.
	 * @param description Description of the error.
	 */
	void reportNetworkError( QNetworkReply::NetworkError code, const QString& description );

private:

	/**
	 * @brief Accesses M-Files REST API client for making REST requests.
	 * @return M-Files REST API client
	 */
	MfwsRest* rest() const;

private:

	MfwsRest* m_rest;  //!< REST client for fetching the listing.

	QString m_host;  //!< URL for fetching the data.
	QString m_username;   //!< The username for fetching the vaults.
	QString m_password;  //!< Password for fetching the vaults.

	QJsonArray m_vaults;  //!< List of the fetched vaults.

};

#endif // VAULTLISTING_H
