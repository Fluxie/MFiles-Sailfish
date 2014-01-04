.pragma library

function setValue( container, control ) {

    // TODO: Set value list.
    var typedValue = getTypedValue( container );
    control.text = typedValue.Lookup.DisplayValue;
}

//! Gets the value list id this lookup represents.
function getValueListId( container ) {
    var vault = container.vault;
    container.propertyValue.PropertyDef
}

//! Gets the lookup.
function getTypedValue( container ) {
    return container.propertyValue.TypedValue;
}
