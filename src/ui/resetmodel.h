#ifndef RESETMODEL_H
#define RESETMODEL_H

// Forward declarations.
class ListModelBase;

/**
 * @brief The ResetModel class resets the model.
 */
class ResetModel
{
public:

	/**
	 * @brief Initializes the ResetModel objects and begins the reset.
	 * @param model The model that is reset.
	 */
	ResetModel( ListModelBase* model );

	/**
	  * Uninitializes the ResetModel object and ends the reset.
	  */
	~ResetModel();

private:

	ListModelBase* m_model;
};

#endif // RESETMODEL_H
