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
	 * @brief AllowedLookupsResolver
	 * @param parent The parent model hosting this resolver.
	 * @param vault Reference to the vault.
	 */
	explicit AllowedLookupsResolver( PropertyValueListModel* parent, VaultFront* vault );

signals:

	/**
	 * @brief allowedValueResolved Signaled when the allowed value has changed an a new value has been resolved.
	 * @param index The location of the value
	 * @param propertyValue New value
	 */
	void allowedValueResolved( const QModelIndex& owner, const QJsonValue& propertyValue  );

public slots:

	/**
	 * @brief requestValueResolution
	 * @param topLeft
	 * @param bottomRight
	 */
	void requestValueResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles = QVector<int>() );

// Private interface.
private:

	AsyncFetch* fetchAllowedItems( const QModelIndex& currentValueInded, const QJsonValue& currentValue  );

	void resolveValidity( const QJsonArray& allowedItems, const QModelIndex& index, const QJsonValue& currentValue );

// Private data.
private:

	PropertyValueListModel* m_model;  //!< The model that owns this resolver.
	VaultFront* m_vault;

};

#endif // ALLOWEDLOOKUPSRESOLVER_H
