
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
		,	highlightOnClick( false )
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
	bool highlightOnClick;
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
		,	highlightCellOnClick( false )
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
	bool highlightCellOnClick;
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
		,	highlightCellOnClick( false )
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
	bool highlightCellOnClick;
}; // class TableViewPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__TABLEVIEW_P_HPP__INCLUDED
