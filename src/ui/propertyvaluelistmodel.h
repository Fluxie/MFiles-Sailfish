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

#ifndef PROPERTYVALUEListMODEL_H
#define PROPERTYVALUEListMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QJsonArray>

// Forward declarations.
class AllowedLookupsResolver;
class ObjectVersionFront;
class PropertyValueOwnerResolver;
class VaultFront;

//! Model that displays the property values of an object.
class PropertyValueListModel : public QAbstractListModel
{
	Q_OBJECT
	Q_ENUMS( DataFilter )
	Q_PROPERTY( DataFilter dataFilter READ dataFilter WRITE setDataFilter NOTIFY dataFilterChanged )
	Q_PROPERTY( ObjectVersionFront* objectVersion READ objectVersion WRITE setObjectVersion NOTIFY objectVersionChanged )
	Q_PROPERTY( VaultFront* vault READ vault WRITE setVault NOTIFY vaultChanged )
public:

	enum DataFilter
	{
		Undefined,  //!< No data has been specified to be shown in the model.
		PropertyValuesForDisplay,  //!< Property values for display are to be shown.
		Workflow,  //!< Properties related to the workflow are shown.
		AllPropertyValues  //!< All property values are shown.
	};

	//! The role id of the property definition id role.
	static const int PropertyDefinitionIdRole;

	//! The role id the property value role.
	static const int PropertyValueRole;

	//! The role id the filter role.
	static const int FilterRole;

	explicit PropertyValueListModel(QObject *parent = 0);

	//! The data that is modelled by this model.
	DataFilter dataFilter() const { return m_filter; }

	//! Object version tied to this model.
	ObjectVersionFront* objectVersion() const { return m_objectVersion; }

	/**
	 * @brief vault
	 * @return Vault associated with this model.
	 */
	VaultFront* vault() const { return m_vault; }

// Interface implementing the model.
public:

	//! Returns the number of rows under the given parent.
	virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

	//! Returns the data stored under the given role for the item referred to by the index.
	virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	//! Role names. Note: The documentation claims that we should call setRoleNames to specify the roles. However, this function no longer exists and roleNAmes has been made virtula.
	virtual QHash< int,QByteArray > roleNames() const;

	//! Flags.
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	//! Sets the data.
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

signals:

	//! The modelled data has changed.
	void dataFilterChanged();

	//! The object version of the model has changed.
	void objectVersionChanged();

	/**
	 * @brief vaultChanged is signaled when the vault associated with the property value model changes.
	 */
	void vaultChanged();

public slots:

	//! Sets the modelled data.
	void setDataFilter( DataFilter filter );

	//! Sets the object version for the model.
	void setObjectVersion( ObjectVersionFront* objectVersion );

	/**
	 * @brief setVault
	 * @param vault associated with the model.
	 */
	void setVault( VaultFront* vault );

	/**
	 * @brief suggestData
	 * @param index The location of the new data.
	 * @param propertyValue The new value for the location.
	 */
	void suggestData( const QModelIndex& index, const QJsonValue& propertyValue );

	//! Refreshes the property values based on the current filter and object version.
	void refreshPropertyValues();

	//! Submit the changes made to the model.
	virtual bool submit();

	//! Revert the changes made to the model.
	virtual void revert();

// Private interface.
private:

	//! Refreshes the property values based on the current filter and object version.
	void refreshPropertyValuesImpl();

	//! Returns data for display.
	void forDisplay( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for decoration.
	void forDecoration( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for property definition id role.
	void forPropertyDefinitionId( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for property value role.
	void forPropertyValue( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for filter role.
	void forFilter( const QModelIndex & index, QVariant& variant ) const;

// Private data:
private:

	DataFilter m_filter;  //!< The type of the data modelled in this model.
	ObjectVersionFront* m_objectVersion;  //!< The object version this model represents.
	QJsonArray m_propertyValues;  //!< The property values shown in this model.

	// Auxilary functions.
	PropertyValueOwnerResolver* m_ownerResolver;  //!< Functionality for resolving the owner.
	AllowedLookupsResolver* m_allowedLookupsResolvers;  //!< Functionality for resolving the allowed lookup values based on the current filter.
	VaultFront* m_vault;

};

#endif // PROPERTYVALUEListMODEL_H
