#ifndef ALLOWEDLOOKUPSRESOLVER_H
#define ALLOWEDLOOKUPSRESOLVER_H

#include <QModelIndex>
#include <QObject>

// Forward declarations.
class PropertyValueModel;
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
	explicit AllowedLookupsResolver( PropertyValueModel* parent, VaultFront* vault );

signals:

public slots:

	/**
	 * @brief requestValueResolution
	 * @param topLeft
	 * @param bottomRight
	 */
	void requestValueResolution( const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int> &roles = QVector<int>() );

// Private data.
private:

	PropertyValueModel* m_model;  //!< The model that owns this resolver.
	VaultFront* m_vault;

};

#endif // ALLOWEDLOOKUPSRESOLVER_H
