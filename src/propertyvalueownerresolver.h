#ifndef PROPERTYVALUEOWNERRESOLVE_H
#define PROPERTYVALUEOWNERRESOLVE_H

#include <QJsonValue>
#include <QModelIndex>
#include <QMultiHash>
#include <QObject>

// Forward declarations.
class PropertyValueModel;
class VaultFront;

/**
 * @brief The PropertyValueOwnerResolve class TODO
 */
class PropertyValueOwnerResolver : public QObject
{
	typedef QMultiHash< int, int > MAPPER;

	Q_OBJECT
public:

	//! Initializes new Property value owner resolver.
	explicit PropertyValueOwnerResolver(
			PropertyValueModel* parent,
			VaultFront* vault
	);

	/**
	 * @brief ownershipInfo
	 * @param subItem The index of the sub-item of which ownership information is requested.
	 * @return Information about the owners of the specified item
	 */
	QJsonValue ownershipInfo( const QModelIndex& subItem ) const;

	/**
	 * @brief mayHaveOwner checks if the given sub-item may have an owner value.
	 * @param subItem
	 * @return True if the specified subItem *may* have an owner.
	 */
	bool mayHaveOwner( const QModelIndex& subItem ) const;

signals:

	/**
	 * @brief Signaled when owner of an has been determined
	 * @param index
	 * @param propertyValue New owner.
	 */
	void ownerDetermined( const QModelIndex& owner, QJsonValue propertyValue );

	/**
	 * @brief Signaled when the ownership information of the item is changed.
	 * @param topLeft The item which ownership information is changed.
	 * @param bottomRight The item which ownership information is changed.
	 */
	void ownershipInfoChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector< int >& roles = QVector<int>() );

public slots:

	//! Data in the model has changed and a resolution of ownership is required.
	void requestOwnerResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles = QVector<int>() );

	/**
	 * @brief requestOwnershipInfoResolution
	 * @param topLeft
	 * @param bottomRight
	 */
	void requestOwnershipInfoResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles = QVector<int>() );

	/**
	 * @brief refreshOwnershipInfo
	 */
	void refreshOwnershipInfo();

// Private interfae.
private:

	void tryAutoFillOwnerLocation( int ownerLocationVersion, int ownerPropertyLocation, int ownerId );

// Private data.
private:

	PropertyValueModel* m_model;  //!< The model that owns this resolver.
	VaultFront* m_vault;

	int m_ownerLocationVersion;  //!< The version of owner location mapping. This version is incremented when the owner location map is refreshed.
	MAPPER m_ownerLocationsBySubListLocation;  //!< Property definitions that can filter the 'key' property definition.
	MAPPER m_subItemLocationByOwnerLocation;  //!< Maps the locations of possible subitems by their owners.
};

#endif // PROPERTYVALUEOWNERRESOLVE_H
