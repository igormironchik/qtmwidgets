
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
class QHBoxLayout;
QT_END_NAMESPACE


namespace QtMWidgets {

class TextLabel;

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
*/
class TableViewCell
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property highlightOnClick

		\brief Enables/disables highlighting of the cell on click.

		By default, this property is false.
	*/
	Q_PROPERTY( bool highlightOnClick READ highlightOnClick
		WRITE setHighlightOnClick )

public:
	TableViewCell( QWidget * parent = 0 );
	virtual ~TableViewCell();

	//! \return Label with image.
	QLabel * imageLabel() const;
	//! \return Label with text.
	TextLabel * textLabel() const;
	//! \return Label with detailed text.
	TextLabel * detailedTextLabel() const;

	//! \return Accessory widget.
	QWidget * accessoryWidget() const;
	//! Set accessory widget.
	void setAccessoryWidget( QWidget * accessory );

	//! \return Is highlighting of the cell on click enabled?
	bool highlightOnClick() const;
	//! Enable/disable highlighting of the cell on click.
	void setHighlightOnClick( bool on );

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * e ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;

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

	/*!
		\property highlightCellOnClick

		\brief Enables/disables highlighting of the cell on click.

		By default, this property is false.
	*/
	Q_PROPERTY( bool highlightCellOnClick READ highlightCellOnClick
		WRITE setHighlightCellOnClick )

public:
	TableViewSection( QWidget * parent = 0 );
	virtual ~TableViewSection();

	//! \return Header.
	TextLabel * header() const;
	//! \return Footer.
	TextLabel * footer() const;

	/*!
		Add new cell to the bottom.
		Parent of the cell will be set to this section.

		\note highlightOnClick property of the \a cell
		will be overriden.
	*/
	void addCell( TableViewCell * cell );
	/*!
		Insert new cell at the given position.
		Parent of the cell will be set to this section.

		\note highlightOnClick property of the \a cell
		will be overriden.
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

	//! \return Is highlighting of the cell on click enabled?
	bool highlightCellOnClick() const;
	//! Enable/disable highlighting of the cell on click.
	void setHighlightCellOnClick( bool on );

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

	/*!
		\property highlightCellOnClick

		\brief Enables/disables highlighting of the cell on click.

		By default, this property is false.
	*/
	Q_PROPERTY( bool highlightCellOnClick READ highlightCellOnClick
		WRITE setHighlightCellOnClick )

public:
	TableView( QWidget * parent = 0 );
	virtual ~TableView();

	/*!
		Add new section to the bottom.
		Parent of the section will be set to this view.

		\note highlightCellOnClick property of the \a section
		will be overriden.
	*/
	void addSection( TableViewSection * section );
	/*!
		Insert new section at the given position.
		Parent of the section will be set to this view.

		\note highlightCellOnClick property of the \a section
		will be overriden.
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

	//! \return Is highlighting of the cell on click enabled?
	bool highlightCellOnClick() const;
	//! Enable/disable highlighting of the cell on click.
	void setHighlightCellOnClick( bool on );

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
