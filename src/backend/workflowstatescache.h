#ifndef WORKFLOWSTATESCACHE_H
#define WORKFLOWSTATESCACHE_H

#include <QHash>
#include <QMutex>
#include <QObject>

#include "workflowstateskey.h"
#include "corebase.h"

// Forward declarations.
class VaultCore;
class WorkflowStatesCore;

class WorkflowStatesCache : public CoreBase
{
	/**
	 * @brief Definition for cached states
	 */
	typedef QHash< WorkflowStatesKey, WorkflowStatesCore* >  STATES;

	Q_OBJECT
public:

	/**
	 * @brief Initializes new WorkflowStatesCache.
	 * @param vault Vault.
	 */
	explicit WorkflowStatesCache( VaultCore* vault );


	/**
	 * @brief getTransitions
	 * @param workflow The workflow of which states are requested.
	 * @param currentState  The current state in the workflow.
	 * @return States of the specified workflow
	 */
	WorkflowStatesCore* getStates( int workflow, int currentState );

signals:

public slots:

	/**
	 * @brief Clears the cache.
	 */
	void clear();

// Data.
private:

	mutable QMutex m_mutex;

	STATES m_states;
	VaultCore* m_vault;

};

#endif // WORKFLOWSTATESCACHE_H
