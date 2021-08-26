
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

#ifndef QTMWIDGETS__TEXTLABEL_HPP__INCLUDED
#define QTMWIDGETS__TEXTLABEL_HPP__INCLUDED

// Qt include.
#include <QFrame>
#include <QScopedPointer>
#include <QTextOption>


namespace QtMWidgets {

//
// TextLabel
//

class TextLabelPrivate;

/*!
	TextLabel is a frame with text. This widget is like the
	QLabel, but that can display only text.
*/
class TextLabel
	:	public QFrame
{
	Q_OBJECT

	/*!
		\property text
		\brief the label's text

		If no text has been set this will return an empty string. Setting
		the text clears any previous content.

		The text will be interpreted either as plain text or as rich
		text, depending on the text format setting; see setTextFormat().
		The default setting is Qt::AutoText; i.e. TextLabel will try to
		auto-detect the format of the text set.

		\sa setTextFormat()
	*/
	Q_PROPERTY( QString text READ text WRITE setText )
	/*!
		\property textFormat
		\brief the label's text format

		See the Qt::TextFormat enum for an explanation of the possible
		options.

		The default format is Qt::AutoText.

		\sa text()
	*/
	Q_PROPERTY( Qt::TextFormat textFormat READ textFormat WRITE setTextFormat )
	/*!
		\property textOption
		\brief the label's text options

		See the QTextOption for an explanation of the possible options.

		The default alignment is Qt::AlignLeft | Qt::AlignVCenter.
		Default flags are QTextOption::IncludeTrailingSpaces.
		The default word wrap mode is QTextOption::WordWrap.
	*/
	Q_PROPERTY( QTextOption textOption READ textOption WRITE setTextOption )
	/*!
		\property margin
		\brief the width of the margin

		The margin is the distance between the innermost pixel of the
		frame and the outermost pixel of contents.

		The default margin is 0.
	*/
	Q_PROPERTY( int margin READ margin WRITE setMargin )
	/*!
		\property color

		Color of the text.
	*/
	Q_PROPERTY( QColor color READ color WRITE setColor )

public:
	/*!
		Constructs an empty label.

		The \a parent and widget flag \a f, arguments are passed
		to the QFrame constructor.
	*/
	TextLabel( QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );
	/*!
		Constructs a label that displays the text \a text.

		The \a parent and widget flag \a f, arguments are passed
		to the QFrame constructor.
	*/
	explicit TextLabel( const QString & text,
		QWidget * parent = 0, Qt::WindowFlags f = Qt::WindowFlags() );

	virtual ~TextLabel();

	//! \return Text.
	QString text() const;
	//! Set text.
	void setText( const QString & text );

	//! \return Text format.
	Qt::TextFormat textFormat() const;
	//! Set text format.
	void setTextFormat( Qt::TextFormat format );

	//! \return Text options.
	QTextOption textOption() const;
	//! Set text option.
	void setTextOption( const QTextOption & textOption );

	//! Set font.
	void setFont( const QFont & font );

	//! \return Margin.
	int margin() const;
	//! Set margin.
	void setMargin( int margin );

	//! \return Color of the text.
	const QColor & color() const;
	//! Set color.
	void setColor( const QColor & c );

	bool hasHeightForWidth() const override;
	int heightForWidth( int w ) const override;
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * e ) override;
	void resizeEvent( QResizeEvent * e ) override;

private:
	Q_DISABLE_COPY( TextLabel )

	QScopedPointer< TextLabelPrivate > d;
}; // class TextLabel

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__TEXTLABEL_HPP__INCLUDED
