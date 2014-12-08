#include "workflowstatesfront.h"

#include <QtAlgorithms>
#include <QJsonValue>

#include "../backend/workflowstatescore.h"
#include "../backend/asyncfetch.h"
#include "../mfiles/valuelistitem.h"

WorkflowStatesFront::WorkflowStatesFront(
	VaultCore* vault,
	ValueListCore* core,
	WorkflowStatesCore* states
) :
	ValueListFront( vault, core ),
	m_workflowStates( states ),
	m_cachePopulated( states == nullptr )
{
	// Connect signals to allowed transitions.
	if( m_workflowStates != nullptr )
	{
		QObject::connect( m_workflowStates, &WorkflowStatesCore::refreshed, this, &WorkflowStatesFront::updateCache );
		updateCache();
	}
}

ValueListFront::Status WorkflowStatesFront::status()
{
	// Disconnected?
	ValueListFront::Status coreStatus =  ValueListFront::status();
	if( coreStatus == ValueListFront::Disconnected )
		return ValueListFront::Disconnected;

	// Is the value list empty.	
	if( coreStatus == ValueListFront::Empty || ! m_cachePopulated )
		return ValueListFront::Empty;

	// We are in a ready state.
	return ValueListFront::Ready;
}

//! Value list items.
AsyncFetch* WorkflowStatesFront::items()
{
	// Fetch items from the base class.
	AsyncFetch* items = ValueListFront::items();

	// Set result converter if specific workflow states were given to us.
	if( m_workflowStates != nullptr	)
	{
		// Set result conversion.
		QPointer< WorkflowStatesFront > me = this;
		items->setResultConverter( [ me ]( const QJsonArray& input ) -> QJsonArray {

			// Return the list asis if the owner has died for some reason.
			if( me.isNull() )
				return input;

			// Update the selectable flag of each item.
			QVector< QJsonValue > selectedStates;
			selectedStates.reserve( input.size() );
			foreach( MFiles::ValueListItem item, input )
			{
				// Is the user allowed to select this state.
				STATE_CACHE::iterator foundState = me->m_cachedStates.find( item.id() );
				if( foundState == me->m_cachedStates.end() )
					continue;
				QJsonObject state = foundState.value().toObject();
				bool canSelect = state[ "Selectable" ].toBool();

				// Update the selectability.
				item.setCanSelect( canSelect );
				item[ "__sf__Order" ] = state[ "__sf__Order" ];
				selectedStates.push_back( item.toJsonValue() );
			}

			// Sort the output based on the order.
			qSort( selectedStates.begin(), selectedStates.end(), []( const MFiles::ValueListItem& left, const MFiles::ValueListItem& right ) -> bool {

				// Compare the order.
				int leftOrder = left[ "__sf__Order" ].toDouble();
				int rightOrder = right[ "__sf__Order" ].toDouble();
				return leftOrder < rightOrder;
			});

			// Create output.
			QJsonArray output;
			foreach( const QJsonValue& item, selectedStates )
			{
				output.append( item );
			}
			return output;
		});
	}
	else
	{
		// No need to apply a separate filter.

	}  // end if.

	// Return the items.
	return items;
}

void WorkflowStatesFront::updateCache()
{
	// Store current status.
	ValueListFront::Status previousStatus = status();

	// Fetch and cache the workflow states
	AsyncFetch* fetchStates = m_workflowStates->list();
	if( fetchStates->state() != AsyncFetch::Finished )
		return;  // A new fetch has started.
	m_cachedStates.clear();
	int order = 0;
	foreach( QJsonValue workflowState, fetchStates->values() )
	{
		QJsonObject asObject = workflowState.toObject();
		asObject[ "__sf__Order" ] = order++;
		int id = asObject[ "ID" ].toDouble();
		m_cachedStates.insert( id, asObject );
	}
	m_cachePopulated = true;

	// Status changed?
	if( previousStatus != status() )
		emit statusChanged();
	emit refreshed();
}
