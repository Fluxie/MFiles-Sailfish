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

.pragma library

/**
 * Synchronizes time from the dialog to the dialog's satellite data.
 *
 * @param {dialog} Dialog
 */
function synchronizeDialogToSatelliteData( dialog ) {

    // Construct the time object.
    var time = new Date();
    time.setHours( dialog.hour );
    time.setMinutes( dialog.minute );
    time.setSeconds( dialog.second );

    // Update text.
    dialog.timeText = time.toLocaleTimeString( Qt.locale(), dialog.timeFormat );
}
