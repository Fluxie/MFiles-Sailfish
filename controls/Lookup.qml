import QtQuick 2.0
import Sailfish.Silica 1.0
import mohari.sailfish 1.0

import "Lookup.js" as Logic

Button {

    id: lookup

	// Properties
    property Loader container
    property variant value    
    property int propertyDefinitionId: container ? container.propertyValue.PropertyDef : -1
    property VaultFront vault: container ? container.vault : null

    // Do not enable before the property definition is available
    text: container ? container.propertyValue.TypedValue.DisplayValue : ""
    enabled: vault ? vault.propertyDefinitionsReady : false

	// Action
	onClicked: {        
        console.log( "onClicked" );
        console.log( "Property definition:" + propertyDefinitionId );
        var propertyDefinition = vault.get( VaultFront.PropertyDefinition, propertyDefinitionId );
        var valueListId = propertyDefinition.ValueList;
        var dialog = pageStack.push("../dialogs/SelectLookups.qml",
                                    { valueList: valueListId, propertyDefinition: propertyDefinitionId, vault: lookup.vault } );
		dialog.accepted.connect(function() {})
	}
}
