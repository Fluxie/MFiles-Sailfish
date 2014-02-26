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

#ifndef ERRORLAYER_H
#define ERRORLAYER_H

#include <QString>

/**
 * @brief The ErrorLayer class represents a single error information layer wihtin the error.
 *
 * The error layer captures the location where the error was passed on.
 */
class ErrorLayer
{
public:

	/**
	 * @brief Default constructor, required for QVector.
	 */
	ErrorLayer() {}

	/**
	 * @brief Initializes new layer.
	 * @param file The file name where the layer is created.
	 * @param lineNumber  The line number within the file where the layer is created.
	 * @param className  The name of the class where the layer is created.
	 * @param contextId Additional context information. This information can be used to filter the errors that will be shown to the user.
	 */
	ErrorLayer( const QString& file, int lineNumber, const QString& className, const QString& contextId );

	/**
	 * @brief Copy-constructor.
	 * @param copy Original.
	 */
	ErrorLayer( const ErrorLayer& copy );

	/**
	 * @brief GEts the file name where the layer was created.
	 * @return Returns the file name.
	 */
	const QString& file() const { return m_file; }

	/**
	 * @brief Gets the line number within the file where this layer was created.
	 * @return Returns the line number within the file where this layer was created.
	 */
	int lineNumber() const { return m_lineNumber; }

	/**
	 * @brief Gets the name of the class where the layer is created.
	 * @return Returns the name of the class where this layer was created.
	 */
	const QString& className() const { return m_className; }

	//! The context where this layer was created.
	/**
	 * @brief Gets the context identifier.
	 * @return Returns the context identifier.
	 */
	const QString& contextId() const { return m_contextId; }

private:

	QString m_file;  //!< The file where this layer was added.
	int m_lineNumber;  //!< The line number.
	QString m_className;  //!< The name of the class where the layer was added.
	QString m_contextId;  //!< Identifies the context. This information can be used to filter the errors that will be shown to the user.
};

#endif // ERRORLAYER_H
