#ifndef REALVALIDATOR_H
#define REALVALIDATOR_H

#include <QValidator>
#include <QDoubleValidator>

/**
 * @brief Validator for M-Files Real data type.
 *
 * This a basically standard DoubleValidator with the addition that empty value is considered valid.
 */
class RealValidator : public QDoubleValidator
{
	Q_OBJECT
public:

	//! Constructor
	explicit RealValidator();

	//! Override the validation method.
	Q_INVOKABLE virtual QValidator::State validate(
			QString& input,
			int& pos
	) const;

signals:

public slots:

};

#endif // REALVALIDATOR_H
