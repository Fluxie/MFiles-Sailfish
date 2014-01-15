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
import mohari.sailfish 1.0
import "../controls"
import "VaultHome.js" as Logic

Page {

	id: propertyValues

	// Define properties
	property variant objectVersion
	property VaultFront vault
	property variant objectData: vault.object( objectVersion )
	property int latestVersion: objectData.latestVersion

	// Declare list view that displays the property values.
	SilicaListView {

		id: propertyValueList

		model: PropertyValueModel {

			dataFilter: PropertyValueModel.PropertyValuesForDisplay
			objectVersion: objectData.version( propertyValues.latestVersion )
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

			property variant propertyDefinition: vault.propertyDefinitionsReady ? vault.get( VaultFront.PropertyDefinition, model.propertyDefinitionId ) : null

			// Position.
			anchors.left: parent.left
			anchors.right: parent.right
			height: label.height + Math.max( typedValue.minimumHeight, typedValue.height ) + Theme.paddingMedium

			// Label.
			Label {

				id: label

				// Position
				anchors.top: parent.top
				anchors.left: parent.left
				anchors.right: parent.right

				// Content
                text: propertyDefinition ? propertyDefinition.Name : ""
				color: Theme.secondaryColor
			}

			TypedValue {

				id: typedValue

				// Position
				anchors.top: label.bottom
				//anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.right: parent.right

				// Content
				propertyDefinitionName: propertyDefinition ? propertyDefinition.Name : ""
				propertyValue: model.propertyValue
				vault: propertyValues.vault

				// Update the model if necessary.
				onAccepted: model.propertyValue = typedValue.propertyValue

			}  // end TypedValue
		}
	}
}
