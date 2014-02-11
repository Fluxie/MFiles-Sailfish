#ifndef MFILESCONSTANTS_H
#define MFILESCONSTANTS_H

/** Various constants related to M-Files.
 *
 */
namespace MFilesConstants
{
	//! A special value indicating all object types.
	static const int AllObjectTypes = -2;

	/**
	 * @brief AutomaticFiltering is a special value for OwnerProperty that specifies that automatic filtering is used.
	 */
	static const int AutomaticFiltering = -106;

	/**
	 * @brief The DataType enum
	 */
	enum DataType
	{
		SingleSelectLookup = 9,
		MultiSelectLookup = 10,
	};

}

#endif // MFILESCONSTANTS_H
