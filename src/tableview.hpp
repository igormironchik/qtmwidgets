
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

#ifndef QTMWIDGETS__TABLEVIEW_HPP__INCLUDED
#define QTMWIDGETS__TABLEVIEW_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>

// QtMWidgets include.
#include "scrollarea.hpp"

QT_BEGIN_NAMESPACE
class QLabel;
class QLayout;
QT_END_NAMESPACE


namespace QtMWidgets {

//
// TableViewCell
//

class TableViewCellPrivate;

/*!
	TableViewCell is a row in the TableViewSection.

	TableViewCell contains image label on the left side,
	located one above the other text label and detailed text label
	on the center, and accessory widget on the right side.

	Accessory widget can be any QWidget. For example, you can
	set it to Switch.

	\note If you want to have eccessory widget expanded to whole
	cell then you should change text label's size policy to
	QSizePolicy::Fixed. For example:

	\code
	TableViewCell::textLabel()->setSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed );
	\endcode
*/
class TableViewCell
	:	public QWidget
{
	Q_OBJECT

public:
	TableViewCell( QWidget * parent = 0 );
	virtual ~TableViewCell();

	//! \return Layout of the cell.
	QLayout * layout() const;

	//! \return Label with image.
	QLabel * imageLabel() const;
	//! \return Label with text.
	QLabel * textLabel() const;
	//! \return Label with detailed text.
	QLabel * detailedTextLabel() const;

	//! \return Accessory widget.
	QWidget * accessoryWidget() const;
	//! Set accessory widget.
	void setAccessoryWidget( QWidget * accessory );

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

protected:
	explicit TableViewCell( TableViewCellPrivate * dd, QWidget * parent = 0 );

	friend class TableViewCellPrivate;

	QScopedPointer< TableViewCellPrivate > d;

private:
	Q_DISABLE_COPY( TableViewCell )
}; // class TableViewCell


//
// TableViewSection
//

class TableViewSectionPrivate;

/*!
	TableViewSection is a section in TableView with
	TableViewCell as rows, header and footer.
*/
class TableViewSection
	:	public QWidget
{
	Q_OBJECT

public:
	TableViewSection( QWidget * parent = 0 );
	~TableViewSection();

	//! \return Header.
	QLabel * header() const;
	//! \return Footer.
	QLabel * footer() const;

	/*!
		Add new cell to the bottom.
		Parent of the cell will be set to this section.
	*/
	void addCell( TableViewCell * cell );
	/*!
		Insert new cell at the given position.
		Parent of the cell will be set to this section.
	*/
	void insertCell( int index, TableViewCell * cell );

	/*!
		Remove cell from this section.

		\note Cell won't be deleted. Parent of the cell
		will be set to 0.
	*/
	void removeCell( TableViewCell * cell );
	/*!
		Remove cell with position \a index from this section.

		\note Returned cell won't be deleted. Parent of the cell
		will be set to 0.
	*/
	TableViewCell * removeCell( int index );

	//! \return Count of cells in this section.
	int cellsCount() const;

	//! \return Cell for the given \a index position.
	TableViewCell * cellAt( int index );

private:
	friend class TableViewSectionPrivate;

	Q_DISABLE_COPY( TableViewSection )

	QScopedPointer< TableViewSectionPrivate > d;
}; // class TableViewSection


//
// TableView
//

class TableViewPrivate;

/*!
	TableView is a view with sections with rows (cells).
	It's very similar to UITableView from iOS. TableView is
	widget based view that allows to display sections with rows
	(cells). Cell can have accessory widget, for example, Switch.
	This class is very usefull for settings of the application.
*/
class TableView
	:	public ScrollArea
{
	Q_OBJECT

public:
	TableView( QWidget * parent = 0 );
	~TableView();

	/*!
		Add new section to the bottom.
		Parent of the section will be set to this view.
	*/
	void addSection( TableViewSection * section );
	/*!
		Insert new section at the given position.
		Parent of the section will be set to this view.
	*/
	void insertSection( int index, TableViewSection * section );

	/*!
		Remove Section from this view.

		\note Section won't be deleted. Parent of the section
		will be set to 0.
	*/
	void removeSection( TableViewSection * section );
	/*!
		Remove section with position \a index from this view.

		\note Returned section won't be deleted. Parent of the section
		will be set to 0.
	*/
	TableViewSection * removeSection( int index );

	//! \return Count of sections in this view.
	int sectionsCount() const;

	//! \return Section for the given \a index position.
	TableViewSection * sectionAt( int index );

private:
	Q_DISABLE_COPY( TableView )

	friend class TableViewPrivate;

	inline TableViewPrivate * d_func()
		{ return reinterpret_cast< TableViewPrivate* > ( d.data() ); }
	inline const TableViewPrivate * d_func() const
		{ return reinterpret_cast< const TableViewPrivate* >( d.data() ); }
}; // class TableView

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__TABLEVIEW_HPP__INCLUDED
