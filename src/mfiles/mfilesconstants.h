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
		Text = 1,
		Integer = 2,
		Floating = 3,
		Date = 5,
		Time = 6,
		Timestamp = 7,
		Boolean = 8,
		Lookup = 9,
		SingleSelectLookup = 9,
		MultiSelectLookup = 10,
		// Integer64 = 11,
		// FILETIME = 12,
		MultiLineText = 13,
		// ACL = 14,
	};

	/**
	 * @brief Defines the different folder items that can be visible in views.
	 */
	enum FolderContentItemType
	{
		Unknown	= 0,
		ViewFolder = 1,
		PropertyFolder = 2,
		TraditionalFolder = 3,
		ObjectVersion = 4,
	};

}

}

#endif // MFILESCONSTANTS_H
