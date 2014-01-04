.pragma library

//! Sets the value for the control.
function setValue( container, control ) {

    // Update the value.
    var value = container.propertyValue.TypedValue;
    if( ! value.HasValue )
        control.currentIndex = 0;
    else if( value.Value )
        control.currentIndex = 1;
    else
        control.currentIndex = 2;
}
