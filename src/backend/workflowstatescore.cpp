#include "workflowstatescore.h"

WorkflowStatesCore::WorkflowStatesCore( VaultCore* vault, int workflow, int currentState ) :
	StructureCacheBase( WorkflowStatesCore::getResource( workflow, currentState ), vault )
{
}

WorkflowStatesCore::WorkflowStatesCore( VaultCore* vault, int workflow ) :
	StructureCacheBase( WorkflowStatesCore::getResource( workflow, -1 ), vault )
{
}


QString WorkflowStatesCore::getResource( int workflow, int currentState )
{
	QString resource;
	if( currentState > 0)
		resource = QString( "/structure/workflows/%1/states?currentstate=%2" ).arg( workflow ).arg( currentState );
	else
		resource = QString( "/structure/workflows/%1/states" ).arg( workflow );
	return resource;
}

