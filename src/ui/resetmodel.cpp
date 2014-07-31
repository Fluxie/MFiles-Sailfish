#include "resetmodel.h"

#include "listmodelbase.h"

ResetModel::ResetModel( ListModelBase* model ) :
	m_model( model )
{
	Q_CHECK_PTR( model );

	m_model->beginResetModel();
}

ResetModel::~ResetModel()
{
	m_model->endResetModel();
}
