
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

#ifndef QTMWIDGETS__LISTMODEL_HPP__INCLUDED
#define QTMWIDGETS__LISTMODEL_HPP__INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>

// QtMWidgets include.
#include "abstractlistmodel.hpp"
#include "private/listmodel_p.hpp"


namespace QtMWidgets {

//
// ListModel
//

/*!
	ListModel is a model used with ListView.
	Template parameter is type of the data in the model.
*/
template< typename T >
class ListModel
	:	public AbstractListModel
{
public:
	ListModel( QObject * parent = 0 )
		:	AbstractListModel( parent )
		,	d( new ListModelPrivate< T > ( this ) )
	{
	}

	virtual ~ListModel()
	{
	}

	//! \return Data in the given \a row row.
	virtual const T & data( int row ) const
	{
		return d->data.at( row );
	}

	//! Insert new row at the given \a row position with \a value value.
	bool insertRow( int row, const T & value )
	{
		if( row > d->data.count() )
			return false;

		d->data.insert( row, value );

		emit rowsInserted( row, row );

		return true;
	}

	//! Append new row with the given \a value.
	bool appendRow( const T & value )
	{
		return insertRow( d->data.count(), value );
	}

	//! Insert new row at the given \a row position.
	bool insertRow( int row )
	{
		return insertRows( row, 1 );
	}

	//! Insert \a count count of rows at the given \a row prosition.
	virtual bool insertRows( int row, int count )
	{
		if( row > d->data.count() )
			return false;

		for( int i = 0; i < count; ++i )
			d->data.insert( row + i, T() );

		emit rowsInserted( row, row + count - 1 );

		return true;
	}

	//! Move \a sourceRow row to the new \a destinationRow destination.
	bool moveRow( int sourceRow, int destinationRow )
	{
		return moveRows( sourceRow, 1, destinationRow );
	}

	/*!
		Move \a count count of rows from \a sourceRow position to the
		\a destinationRow row.
	*/
	virtual bool moveRows( int sourceRow, int count, int destinationRow )
	{
		if( sourceRow == destinationRow )
			return false;

		for( int i = 0; i < count; ++i )
			d->data.move( sourceRow + ( sourceRow > destinationRow ? i : 0 ),
				( destinationRow + i < d->data.size() ? destinationRow + i : d->data.size() - 1 ) );

		emit rowsMoved( sourceRow, sourceRow + count - 1, destinationRow );

		return true;
	}

	//! Remove \a row row from the model.
	bool removeRow( int row )
	{
		return removeRows( row, 1 );
	}

	//! Remove \a count count of rows from \a row position.
	virtual bool removeRows( int row, int count )
	{
		if( row + count > d->data.count() )
			return false;

		for( int i = 0; i < count; ++i )
			d->data.removeAt( row );

		emit rowsRemoved( row, row + count - 1 );

		return true;
	}

	//! \return Count of rows.
	virtual int rowCount() const
	{
		return d->data.count();
	}

	//! Set data in \a row position to the \a value value.
	virtual bool setData( int row, const T & value )
	{
		if( row >= d->data.count() )
			return false;

		d->data[ row ] = value;

		emit dataChanged( row, row );

		return true;
	}

	/*!
		Sort data in the model in \a order order.

		If you want sort capability you need to implement this method.
	*/
	virtual void sort( Qt::SortOrder order = Qt::AscendingOrder )
	{
		Q_UNUSED( order )
	}

	//! Reset model.
	virtual void reset()
	{
		d->data.clear();

		emit modelReset();
	}

protected:
	explicit ListModel( ListModelPrivate< T > * dd, QObject * parent = 0 )
		:	AbstractListModel( parent )
		,	d( dd )
	{

	}

protected:
	QScopedPointer< ListModelPrivate< T > > d;

private:
	Q_DISABLE_COPY( ListModel )
}; // class ListModel

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__LISTMODEL_HPP__INCLUDED
