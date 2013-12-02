#include "realvalidator.h"

//! Constructor.
RealValidator::RealValidator() :
	QDoubleValidator(0)
{

}

//! Override the validation method.
QValidator::State RealValidator::validate(
	QString& input,
	int& pos
) const
{
	// Empty string is acceptable.
	if( input.isEmpty() || input.isNull() )
		return QValidator::Acceptable;
	else
		return QDoubleValidator::validate( input, pos );
}
