#ifndef CORECONSTRUCTOR_H
#define CORECONSTRUCTOR_H

#include <QObject>

//! A helper class for managing the construction of core objects.
class FrontBase : public QObject
{
Q_OBJECT

// Public interface.
public:

	//! Initializes new core object.
	explicit FrontBase() {}

	//! Initializes new core object.
	explicit FrontBase( QObject* core ) :
		m_core( core )
	{ emit refreshed(); }

signals:

	//! Signaled when the data has been refresheded. Core becomes available or otherwise.
	void refreshed();

public slots:

	//! A core of the given type has become available.
	void coreAvailable( QObject* core, QObject* source );	

// Protected interface.
protected:

	//! Does this front accept the specified core.
	virtual bool accept( QObject* core ) const = 0;

	//! Accesses the core object.
	QObject* core() { return m_core; }

// Pritvate interface.
private:

	//! Sets the core.
	void setCore( QObject* core, QObject* source );

// Private data:
private:

	QObject* m_core;
};


#endif // CORECONSTRUCTOR_H
