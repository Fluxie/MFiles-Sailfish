#ifndef LISTMODELBASE_H
#define LISTMODELBASE_H

#include <QAbstractListModel>

// Forward declarations.
class ResetModel;

/**
 * @brief The ListModelBase class injects friend declarations to the models.
 */
class ListModelBase : public QAbstractListModel
{
public:

	/**
	 * @brief ListModelBase Initilizes the model.
	 * @param parent Parent of the model.
	 */
	explicit ListModelBase( QObject *parent ) : QAbstractListModel( parent ) {}

	virtual ~ListModelBase() {}

// Friend declarations.
private:

	friend class ResetModel;

};

#endif // LISTMODELBASE_H
