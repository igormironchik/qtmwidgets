
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

#ifndef QTMWIDGETS__PRIVATE__TABLEVIEW_P_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__TABLEVIEW_P_HPP__INCLUDED

// QtMWidgets include.
#include "scrollarea_p.hpp"
#include "../tableview.hpp"

// Qt include.
#include <QLabel>
#include <QList>
#include <QVBoxLayout>


namespace QtMWidgets {

class MinimumSizeLabel;
class TextLabel;
class TableViewCellLayout;


//
// TableViewCellPrivate
//

class TableViewCellPrivate {
public:
	TableViewCellPrivate( TableViewCell * parent )
		:	q( parent )
		,	imageLabel( 0 )
		,	textLabel( 0 )
		,	detailedTextLabel( 0 )
		,	accessoryWidget( 0 )
		,	layout( 0 )
		,	clicked( false )
	{
	}

	virtual ~TableViewCellPrivate()
	{
	}

	void init();

	TableViewCell * q;
	MinimumSizeLabel * imageLabel;
	TextLabel * textLabel;
	TextLabel * detailedTextLabel;
	QWidget * accessoryWidget;
	TableViewCellLayout * layout;
	QColor highlightColor;
	bool clicked;
}; // class TableViewCellPrivate

class RowsSeparator;

//
// TableViewSectionPrivate
//

class TableViewSectionPrivate {
public:
	TableViewSectionPrivate( TableViewSection * parent )
		:	q( parent )
		,	header( 0 )
		,	footer( 0 )
		,	layout( 0 )
	{
	}

	virtual ~TableViewSectionPrivate()
	{
	}

	void init();

	TableViewSection * q;
	TextLabel * header;
	TextLabel * footer;
	QList< TableViewCell* > cells;
	QVBoxLayout * layout;
	QList< RowsSeparator* > separators;
}; // class TableViewSectionPrivate


//
// TableViewPrivate
//

class TableViewPrivate
	:	public ScrollAreaPrivate
{
public:
	TableViewPrivate( TableView * parent )
		:	ScrollAreaPrivate( parent )
		,	layout( 0 )
		,	widget( 0 )
	{
	}

	virtual ~TableViewPrivate()
	{
	}

	void init();

	inline TableView * q_func() { return static_cast< TableView* >( q ); }
	inline const TableView * q_func() const { return static_cast< const TableView* >( q ); }

	QVBoxLayout * layout;
	QList< TableViewSection* > sections;
	QWidget * widget;
}; // class TableViewPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__TABLEVIEW_P_HPP__INCLUDED
