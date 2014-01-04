#include "frontbase.h"

//! A core of the given type has become available.
void FrontBase::coreAvailable( QObject* core, QObject* source )
{
	// Store the core if it is accepted to this object.
	if( this->accept( core ) )
		this->setCore( core, source );
}

//! Sets the core.
void FrontBase::setCore( QObject* core, QObject* source )
{
	// Store the core, disconnect from the core source and notify listeners.
	m_core = core;
	source->disconnect( this, "coreAvailable" );
	emit refreshed();
}
