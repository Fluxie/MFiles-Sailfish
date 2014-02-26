#ifndef ALLOWEDLOOKUPSRESOLVER_H
#define ALLOWEDLOOKUPSRESOLVER_H

#include <QModelIndex>
#include <QObject>

// Forward declarations.
class AsyncFetch;
class PropertyValueListModel;
class TypedValueFilter;
class VaultFront;

/**
 * @brief The AllowedLookupsResolver class resolves the allowed lookups of property values when the filter of the property value changes.
 */
class AllowedLookupsResolver : public QObject
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new AllowedLookupsResolver object.
	 * @param parent The parent model hosting this resolver.
	 * @param vault Reference to the vault.
	 */
	explicit AllowedLookupsResolver( PropertyValueListModel* parent, VaultFront* vault );

signals:

	/**
	 * @brief This signal is emitted when the allowed value has changed an a new value has been resolved.
	 * @param index The location of the value
	 * @param propertyValue New value
	 */
	void allowedValueResolved( const QModelIndex& owner, const QJsonValue& propertyValue  );

public slots:

	/**
	 * @brief Requess allowed value resolution for the specified indexes.
	 * @param topLeft Top left index.
	 * @param bottomRight Bottom right index.
	 * @remarks {Current implementation only supports topLeft == bottomRight. }
	 */
	void requestValueResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles = QVector<int>() );

// Private interface.
private:

	/**
	 * @brief Gets items that are allowed for the specified index.
	 * @param currentValueIndex The model index of the current value.
	 * @param currentValue  The current value
	 * @return Items that are allowed for the index.
	 */
	AsyncFetch* fetchAllowedItems( const QModelIndex& currentValueIndex, const QJsonValue& currentValue  );

	/**
	 * @brief Resolves the validity/allowed values for the specified index.
	 * @param allowedItems  Items that are allowed.
	 * @param index  The model index of which values validity is resolved.
	 * @param currentValue The current value at the specified model index.
	 */
	void resolveValidity( const QJsonArray& allowedItems, const QModelIndex& index, const QJsonValue& currentValue );

// Private data.
private:

	PropertyValueListModel* m_model;  //!< The model that owns this resolver.
	VaultFront* m_vault;

};

#endif // ALLOWEDLOOKUPSRESOLVER_H
