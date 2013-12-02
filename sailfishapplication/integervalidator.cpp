#include "integervalidator.h"

//! Constructor
IntegerValidator::IntegerValidator() :
	QIntValidator()
{
}

//! Override the validation method.
QValidator::State IntegerValidator::validate(
	QString& input,
	int& pos
) const
{
	// Empty string is acceptable.
	if( input.isEmpty() || input.isNull() )
		return QValidator::Acceptable;
	else
		return QIntValidator::validate( input, pos );
}
