#ifndef MFILESCONSTANTS_H
#define MFILESCONSTANTS_H

/**
 * Namespace for M-Files types.
 */
namespace MFiles
{

/**
 * Various constants related to M-Files.
 */
namespace Constants
{

	/**
	 * @brief AllObjectTypes specified "All object types" in the context where it is used.
	 */
	static const int AllObjectTypes = -2;

	/**
	 * @brief AutomaticFiltering is a special value for OwnerProperty that specifies when automatic filtering is used.
	 */
	static const int AutomaticFiltering = -106;

	/**
	 * @brief Defines the different data types available for TypedValues.
	 */
	enum DataType
	{
		SingleSelectLookup = 9,
		MultiSelectLookup = 10,
	};

}

}

#endif // MFILESCONSTANTS_H
