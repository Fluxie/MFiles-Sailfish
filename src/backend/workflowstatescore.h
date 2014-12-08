#ifndef WORKFLOWSTATESCORE_H
#define WORKFLOWSTATESCORE_H

#include <QString>

#include "structurecachebase.h"

class WorkflowStatesCore : public StructureCacheBase
{
	Q_OBJECT
public:

	/**
	 * @brief Initializes new WorkflowStatesCore object.
	 * @param vault Parent vault
	 * @param workflow The workflow
	 * @param currentState The current state of the workflow
	 */
	explicit WorkflowStatesCore( VaultCore* vault, int workflow, int currentState );

	/**
	 * @brief Initializes new WorkflowStatesCore object.
	 * @param vault Parent vault
	 * @param workflow The workflow
	 */
	explicit WorkflowStatesCore( VaultCore* vault, int workflow );

signals:

public slots:

private:

	/**
	 * @brief Gets resource for fetching the states.
	 * @param workflow Workflow.
	 * @param currentState Current state
	 * @return Resource for fetching the states
	 */
	static QString getResource( int workflow, int currentState );
};

#endif // WORKFLOWSTATESCORE_H
