#ifndef CUSTOMROLEPROXYMODLE_H
#define CUSTOMROLEPROXYMODLE_H

#include <QByteArray>
#include <QHash>
#include <QIdentityProxyModel>
#include <QJSValue>
#include <QModelIndex>
#include <QString>
#include <QVariant>

/**
 * @brief The CustomRoleProxyModle class allows the user
 * to specify custom function which will create a new model role based.
 * With the QSortFilterProxyModel this custom role can be used to create
 * complex sorting and filtering scenarios.
 *
 */
class CustomRoleProxyModel : public QIdentityProxyModel
{
	Q_OBJECT
	Q_PROPERTY( int customRoleId READ customRoleId NOTIFY customRoleIdChanged )
	Q_PROPERTY( QString customRoleName READ customRoleName WRITE setCustomRoleName NOTIFY customRoleNameChanged )
	Q_PROPERTY( QJSValue customRoleValue READ customRoleValue WRITE setCustomRoleValue NOTIFY customRoleValueChanged )
public:

	/**
	 * @brief Initializes new custom role proyx mo
	 * @param parent Parent object.
	 */
	explicit CustomRoleProxyModel( QObject *parent = 0 );

	/**
	 * @brief Gets the id of the custom role.
	 * @return The custom role id.
	 */
	int customRoleId() const { return m_customRoleId; }

	/**
	 * @brief Gets the name of the custom role.
	 * @return The name of the custom role.
	 */
	QString customRoleName() const { return m_customRoleName; }

	/**
	 * @brief Gets the function that determines the value of the custom role.
	 * @return The function that determines the value of the custom role.
	 */
	QJSValue customRoleValue() const { return m_valueFunction; }

// QIdentityProxyModel.
public:

	virtual QVariant data( const QModelIndex &index, int role ) const;

	virtual QHash< int, QByteArray > roleNames() const;

signals:

	/**
	 * @brief customRoleIdChanged is signaled when the custom role id has changed.
	 */
	void customRoleIdChanged();

	/**
	 * @brief customRoleNameChanged is signaled when the custom role name has changed.
	 */
	void customRoleNameChanged();

	/**
	 * @brief customRoleValueChanged is signaled when the function for determining the value of the custom role changes.
	 */
	void customRoleValueChanged();

public slots:

	/**
	 * @brief Sets the custom role name.
	 * @param The new custom role name.
	 */
	void setCustomRoleName( const QString& roleName );

	/**
	 * @brief Sets the function for determining the value of the custom function.
	 * @param The function for determining the value of the custom role.
	 */
	void setCustomRoleValue( const QJSValue& valueFunction );

private:

	mutable int m_customRoleId;
	QString m_customRoleName;
	QJSValue m_valueFunction;

	mutable QHash< int, QByteArray > m_sourceRoleNames;

};

#endif // CUSTOMROLEPROXYMODLE_H
