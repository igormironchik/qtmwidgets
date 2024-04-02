
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__PRIVATE__LISTMODEL_P_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__LISTMODEL_P_HPP__INCLUDED

// Qt include.
#include <QList>


namespace QtMWidgets {

template< typename T >
class ListModel;

//
// ListModelPrivate
//

template< typename T >
class ListModelPrivate {
public:
	ListModelPrivate( ListModel< T > * parent )
		:	q( parent )
	{
	}

	virtual ~ListModelPrivate()
	{
	}

	//! Parent.
	ListModel< T > * q;
	//! Data.
	QList< T > data;
}; // class ListModelPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__LISTMODEL_P_HPP__INCLUDED
