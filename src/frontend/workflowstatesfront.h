#ifndef WORKFLOWSTATESFRONT_H
#define WORKFLOWSTATESFRONT_H

#include <QHash>

#include "valuelistfront.h"

// Forward declarations.
class WorkflowStatesCore;
class ValueListcore;
class VaultCore;

/**
 * @brief The WorkflowStatesFront class represent workflow states value list.
 */
class WorkflowStatesFront : public ValueListFront
{	
	/**
	 * @brief Definition for state cache.
	 */
	typedef QHash< int, QJsonValue > STATE_CACHE;

	Q_OBJECT
public:

	/**
	 * @brief Initializes WorkflowStatesFront object that represents workflow states.
	 * @param vault  Parent vault.
	 * @param core Value list
	 * @param states Workflow state.
	 */
	explicit WorkflowStatesFront(
		VaultCore* vault,
		ValueListCore* core,
		WorkflowStatesCore* states
	);

	/**
	 * @brief Gets the workflow states.
	 * @return Workflow states as value list items.
	 */
	Q_INVOKABLE virtual AsyncFetch* items();

	/**
	 * @brief status of the list.
	 * @return Status of the workflow states.
	 */
	virtual ValueListFront::Status status();

private slots:

	/**
	 * @brief Updates the cached transitions.
	 */
	void updateCache();

// Data.
private:

	WorkflowStatesCore* m_workflowStates;

	bool m_cachePopulated;
	STATE_CACHE m_cachedStates;
};

#endif // WORKFLOWSTATESFRONT_H
