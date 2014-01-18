#ifndef LOOKUPMODEL_H
#define LOOKUPMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QJsonArray>
#include <QJsonValue>

class LookupModel : public QAbstractListModel
{
	//! The role id of the lookup role.
	static const int LookupRole;

	Q_OBJECT
	Q_PROPERTY( int rowLimit READ rowLimit WRITE setRowLimit NOTIFY rowLimitChanged )
	Q_PROPERTY( QJsonValue propertyValue READ propertyValue WRITE setPropertyValue NOTIFY propertyValueChanged )
	Q_PROPERTY( int lookupCount READ lookupCount NOTIFY lookupCountChanged )
public:

	//! Initializes the lookup model.
	explicit LookupModel(QObject *parent = 0);

	//! Maximum number of lookups to display.
	int rowLimit() const { return m_rowLimit; }

	//! Property value.
	QJsonValue propertyValue() const { return m_propertyValue; }

	//! Lookup.
	Q_INVOKABLE QJsonValue lookup( int row ) const { return m_lookups[ row ]; }

	//! The actual number of lookups within this model.
	int lookupCount() const { return m_lookups.size(); }

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

	//! Signaled when the row limit changes.
	void rowLimitChanged();

	//! Signaled when the property value changes.
	void propertyValueChanged();

	//! Signaled when the number of lookups changes.
	void lookupCountChanged();

public slots:

	//! Sets the maximum number of rows to display.
	void setRowLimit( int rowLimit );

	//! Sets property value.
	void setPropertyValue( const QJsonValue propertyValue );

// Private interface.
private:

	//! Updates the property value from the current lookups.
	void updatePropertyValueFromLookups();

	//! Returns data for display.
	void forDisplay( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for decoration.
	void forDecoration( const QModelIndex & index, QVariant& variant ) const;

	//! Returns data for lookup.
	void forLookup( const QModelIndex & index, QVariant& variant ) const;

// Private data.
private:

	int m_rowLimit;  //!< The maximum number of rows to display.
	QJsonValue m_propertyValue;  //!< The property value.
	int m_dataType;  //!< The datatype of the typed value inside the property value.
	QJsonArray m_lookups;  //!< Lookups extrated from the property value for convenience.

};

#endif // LOOKUPMODEL_H
