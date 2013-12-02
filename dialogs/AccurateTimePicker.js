.pragma library

/**
 * Synchronizes time from the dialog to the dialog's satellite data.
 *
 * @param {dialog} Dialog
 */
function synchronizeDialogToSatelliteData( dialog ) {

	// Update hours.
	if( dialog.hour !== dialog.time.getHours() )
		dialog.time.setHours( dialog.hour );

	// Update minutes.
	if( dialog.minute !== dialog.time.getMinutes() )
		dialog.time.setMinutes( dialog.minute );

	// Update seconds
	if( dialog.second !== dialog.time.getSeconds() )
		dialog.time.setSeconds( dialog.second );

	// Update text.
	dialog.timeText = Qt.formatTime( dialog.time );
}
