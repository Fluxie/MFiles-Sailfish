#ifndef WORKFLOWSTATESKEY_H
#define WORKFLOWSTATESKEY_H

#include <QJsonValue>

#include "../mfiles/objver.h"

/**
 * @brief The ValueListKey class specified a lookup key for cached workflow states value lists.
 */
class WorkflowStatesKey
{
public:

	/**
	 * @brief Initializes new WorkflowStatesKey object.
	 * @param idIn The id of the value list.
	 * @param filterIn  //!< The filter used to filter the value list.
	 */
	WorkflowStatesKey( int workflowIn, int currentStateIn, QJsonValue objverIn ) :
		workflow( workflowIn ),
		currentState( currentStateIn ),
		objver( objverIn )
	{}

	/**
	 * @brief Copy-contstructor.
	 * @param key Original.
	 */
	explicit WorkflowStatesKey( const WorkflowStatesKey& key ) :
		workflow( key.workflow ),
		currentState( key.currentState ),
		objver( key.objver )
	{}

	int workflow;
	int currentState;
	QJsonValue objver;
};

/**
 * @brief Comparison operator for WorkflowStatesKey objects.
 * @param left  Left operand.
 * @param right Right operand.
 * @return Returns true if the compared objects are equal.
 */
inline bool operator==( const WorkflowStatesKey &left, const WorkflowStatesKey &right )
{
	// Make the equality comparison.
	return left.workflow == right.workflow
			&& left.currentState == right.currentState
			&& left.objver == right.objver;
}

/**
 * @brief Hash function for WorkflowStatesKey object.
 * @param key The WorkflowStatesKey object for which the hash value is calculated.
 * @return Hash value of the WorkflowStatesKey object.
 */
inline uint qHash(const WorkflowStatesKey &key )
{
	if( key.objver.isNull() || key.objver.isUndefined() )
		return qHash( key.workflow << 16 ) ^ qHash( key.currentState );
	else
		return qHash( key.workflow << 16 ) ^ qHash( key.currentState ) ^ qHash( MFiles::ObjVer( key.objver ) );
}


#endif // WORKFLOWSTATESKEY_H
