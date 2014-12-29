
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
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
