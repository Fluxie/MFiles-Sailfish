/*
 *  Copyright 2013 Juha Lepola
 *
 *  This file is part of M-Files for Sailfish.
 *
 *  M-Files for Sailfish is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  M-Files for Sailfish is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with M-Files for Sailfish. If not, see
 *  <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Silica.theme 1.0
import mohari.sailfish 1.0
import "../controls"
import "VaultHome.js" as Logic

Page {

	id: propertyValues

	// Define properties
	property variant objectVersion
	property VaultFront vault
	property variant objectData
	property variant latestVersion
	property variant propertiesForDisplay

	// Store the property values for display of the latest version if they are available.
	onLatestVersionChanged: {

		// Set the listener target if the latest version is available.
		if( latestVersion )
		{
			// Connect the latest version to listeners and store property values for display if available.
			latestVersionConnections.target = latestVersion;
			if( latestVersion.propertiesForDisplay )
				propertiesForDisplay = latestVersion.propertiesForDisplay;
			else
				propertiesForDisplay = null;
		}
		else
		{
			// Clear references.
			latestVersionConnections.target = null;
			propertiesForDisplay = null;

		}  // end if
	}

	// Update the number of property values when the property values for display change.
	onPropertiesForDisplayChanged: {
		if( propertiesForDisplay )
			propertyValueList.propertyValueCount = propertiesForDisplay.length;
		else
			propertyValueList.propertyValueCount = 0;
	}

	// Read object data.
	objectData: vault.object( objectVersion );
	latestVersion: objectData.latest();

	// Listen for new versions.
	Connections {
		target: objectData
		onLatestVersionChanged : {
			latestVersion = objectData.latest();
		}
	}

	// List for changes to property values in the latest version
	Connections {
		id: latestVersionConnections
		target: null

		// Update the list model when the property values for display have changed.
		onPropertiesForDisplayChanged : {
			if( latestVersionConnections.target.propertiesForDisplay )
				propertiesForDisplay = latestVersionConnections.target.propertiesForDisplay;
			else
				propertiesForDisplay = null;
		}
	}

	// Define a component that represents individual property value.


	// Declare list view that displays the property values.
	SilicaListView {

		id: propertyValueList

		// The number of property values.
		// We use the number of property values as the model and update the model here.
		property int propertyValueCount : 0
		onPropertyValueCountChanged: {
			propertyValueList.model = propertyValueCount;
		}

		// Position
		anchors.fill: parent
		anchors.leftMargin: Theme.paddingLarge

		// Header for the page.
		header: PageHeader {

			 id: header
			 title: 'Properties'
		 }

		// Placeholder for empty list view.
		ViewPlaceholder {
			enabled: propertyValueList.count === 0
			text: propertyValues.objectVersion.Title
		}

		// Delegate function that generates the list items.
		delegate: Item	{

			id: propertyValueDelegate

			property variant propertyValue: propertiesForDisplay[ index ]
			property variant propertyDefinition: vault.get( VaultFront.PropertyDefinition, propertyValue.PropertyDef )

			// Position.
			anchors.left: parent.left
			anchors.right: parent.right
			height: label.height + ( typedValue.dynamicHeight ? Math.max( typedValue.minimumHeight, typedValue.height ) : typedValue.minimumHeight )
			// contentHeight: label.height + typedValue.minimumHeight


			// Label.
			Label {

				id: label

				// Position
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right

				// Content
				text: propertyDefinition.Name
				color: Theme.secondaryColor
			}

			TypedValue {

				id: typedValue

				// Position
				anchors.top: label.bottom
				//anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.leftMargin: Theme.paddingMedium
				anchors.right: parent.right

				// Content
				propertyDefinitionName: propertyDefinition.Name
				propertyValue: propertyValueDelegate.propertyValue
			}
		}
	}
}
