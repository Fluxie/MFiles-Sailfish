#ifndef INTEGERVALIDATOR_H
#define INTEGERVALIDATOR_H

#include <QIntValidator>

class IntegerValidator : public QIntValidator
{
	Q_OBJECT
public:

	//! Constructor.
	explicit IntegerValidator();

	//! Override the validation method.
	Q_INVOKABLE virtual QValidator::State validate(
			QString& input,
			int& pos
	) const;

signals:

public slots:

};

#endif // INTEGERVALIDATOR_H
