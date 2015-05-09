/*
 *  Copyright 2015 Juha Lepola
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

#ifndef VAULTLISTMODEL_H
#define VAULTLISTMODEL_H

#include <QJsonArray>
#include <QObject>
#include <QString>

#include "dataaccessorsource.h"
#include "listmodelbase.h"
#include "../frontend/listingstatus.h"

// Forward declarations.
class VaultListing;


/**
 * @brief The VaultListModel class
 */
class VaultListModel : public ListModelBase
{
	Q_OBJECT	
public:	

	/**
	 * @brief Initializes new VaultListModel object
	 * @param parent
	 */
	explicit VaultListModel(QObject *parent = 0);



signals:



public slots:


private:

	VaultListing* m_listing;


};

#endif // VAULTLISTMODEL_H
