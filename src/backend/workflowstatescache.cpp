#include "workflowstatescache.h"

#include <QMutexLocker>

#include "workflowstatescore.h"
#include "vaultcore.h"

WorkflowStatesCache::WorkflowStatesCache( VaultCore* vault ) :
	CoreBase( vault ),
	m_vault( vault )
{	
}

WorkflowStatesCore* WorkflowStatesCache::getStates( int workflow, int currentState )
{
	QMutexLocker lock( &m_mutex );

	// Try locating from the cache.
	WorkflowStatesKey key( workflow, currentState, QJsonValue() );
	STATES::const_iterator found = this->m_states.find( key );
	if( found != this->m_states.constEnd() )
		return found.value();

	// Not found from the cache.
	// Create a new.
	WorkflowStatesCore* states = new WorkflowStatesCore( m_vault, workflow, currentState );
	m_states.insert( key, states );
	return states;
}

void WorkflowStatesCache::clear()
{
	QMutexLocker lock( &m_mutex );

	// Iterate over the cached items and mark them for deletion.
	for( STATES::iterator itr = m_states.begin(); itr != m_states.end(); itr++ )
	{
		itr.value()->deleteLater();
	}
	m_states.clear();
}
