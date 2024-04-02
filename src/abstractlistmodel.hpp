
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__ABSTRACTLISTMODEL_HPP__INCLUDED
#define QTMWIDGETS__ABSTRACTLISTMODEL_HPP__INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>


namespace QtMWidgets {

//
// AbstractListModel
//

//!	AbstractListModel is a base class for the model used with ListView.
class AbstractListModel
	:	public QObject
{
	Q_OBJECT

signals:
	//! Emits when data changed in the range from \a first to the \a last.
	void dataChanged( int first, int last );
	//! Emits when model resets.
	void modelReset();
	//! Emits when new rows inserted.
	void rowsInserted( int first, int last );
	//! Emits when rows removed.
	void rowsRemoved( int first, int last );
	//! Emits when rows moved.
	void rowsMoved( int sourceStart, int sourceEnd, int destinationRow );

public:
	virtual ~AbstractListModel()
	{
	}

protected:
	AbstractListModel( QObject * parent = 0 )
		:	QObject( parent )
	{
	}

private:
	Q_DISABLE_COPY( AbstractListModel )
}; // class ListModel

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__ABSTRACTLISTMODEL_HPP__INCLUDED
