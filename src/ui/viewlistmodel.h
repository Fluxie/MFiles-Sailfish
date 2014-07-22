#ifndef VIEWLISTMODEL_H
#define VIEWLISTMODEL_H

#include <QAbstractListModel>

/**
 * @brief The ViewListModel class
 */
class ViewListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    /**
     * @brief ViewListModel
     * @param parent
     */
    explicit ViewListModel(QObject *parent = 0);


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

public slots:

};

#endif // VIEWLISTMODEL_H
