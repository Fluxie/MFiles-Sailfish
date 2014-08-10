.pragma library

/**
  * Sorting order for the different types of rh
  */
var folderTypeSortOrder = [ 1, 2, 4, 3 ];

/**
 * Less-than comparsion function for view/folder items
 *
 * @param {string} left Left operand of the less than comparion
 * @param {string} right Right operand of the lest than comparsion
 */
function lessThanFolderItem( left, right ) {

    // Compare types?
    var leftType = left.type;
    var rightType = right.type;
    var folderTypeResult = lessThanFolderType( leftType, rightType );
    if( folderTypeResult < 0 )
        return true;
    else if( folderTypeResult > 0 )
        return false;

    // Compare based on the type.
    var comparisonResult = 0;
    var leftData = left.data;
    var rightData = right.data;
    switch( leftType )
    {
    // ViewFolder
    case 1 :
        comparisonResult = lessThanViewFolder( leftData, rightData );
        break;

    // PropertyFolder
    case 2 :
        comparisonResult = lessThanPropertyFolder( leftData, rightData );
        break;

    // ObjectVersion
    case 4 :
        comparisonResult = lessThanObjectVersion( leftData, rightData );
        break;
    }
    if( comparisonResult < 0 )
        return true;

    // The items are equal or the right operand is smaller.
    return false;
}

/**
 * Less-than comparsion function for view/folder item types
 *
 * @param {string} left Left operand of the less than comparion
 * @param {string} right Right operand of the lest than comparsion
 */
function lessThanFolderType( left, right ) {

    // Equal?
    if( left === right )
        return 0;

    // Sort using the defined sorting order.
    var indexOfLeft = folderTypeSortOrder.indexOf( left );
    var indexOfRight = folderTypeSortOrder.indexOf( right );
    if( indexOfLeft < indexOfRight )
        return -1;
    else
        return 1;
}

/**
  * Less-than comparison function for views
  * @param {string} left Left operand of the less than comparion
  * @param {string} right Right operand of the lest than comparsion
  */
function lessThanViewFolder( left, right ) {
    return left.Name.localeCompare( right.Name );
}

/**
 * Less-than comparsion function for property folders.
 *
 * @param {string} left Left operand of the less than comparion
 * @param {string} right Right operand of the lest than comparsion
 */
function lessThanPropertyFolder( left, right ) {
    return left.SortingKey.localeCompare( right.SortingKey );
}

/**
 * Less-than comparsion function for object versions.
 *
 * @param {string} left Left operand of the less than comparion
 * @param {string} right Right operand of the lest than comparsion
 */
function lessThanObjectVersion( left, right ) {
    return left.Title.localeCompare( right.Title );
}
