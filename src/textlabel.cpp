
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

// QtMWidgets include.
#include "textlabel.hpp"

// Qt include.
#include <QStaticText>
#include <QPainter>
#include <QResizeEvent>
#include <QFontMetrics>


namespace QtMWidgets {

//
// TextLabelPrivate
//

class TextLabelPrivate {
public:
	TextLabelPrivate( TextLabel * parent )
		:	q( parent )
	{
	}

	void init();

	TextLabel * q;
	QStaticText staticText;
}; // class TextLabelPrivate

void
TextLabelPrivate::init()
{
	staticText.setTextFormat( Qt::AutoText );

	QTextOption opt;
	opt.setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
	opt.setFlags( QTextOption::IncludeTrailingSpaces );
	opt.setWrapMode( QTextOption::WordWrap );
	staticText.setTextOption( opt );

	staticText.setTextWidth( q->fontMetrics().averageCharWidth() * 10 );

	q->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
}


//
// TextLabel
//

TextLabel::TextLabel( QWidget * parent, Qt::WindowFlags f )
	:	QFrame( parent, f )
	,	d( new TextLabelPrivate( this ) )
{
	d->init();
}

TextLabel::TextLabel( const QString & text,
	QWidget * parent, Qt::WindowFlags f )
	:	QFrame( parent, f )
	,	d( new TextLabelPrivate( this ) )
{
	d->init();
	setText( text );
}

TextLabel::~TextLabel()
{
}

QString
TextLabel::text() const
{
	return d->staticText.text();
}

void
TextLabel::setText( const QString & text )
{
	d->staticText.setText( text );
}

Qt::TextFormat
TextLabel::textFormat() const
{
	return d->staticText.textFormat();
}

void
TextLabel::setTextFormat( Qt::TextFormat format )
{
	d->staticText.setTextFormat( format );
}

QTextOption
TextLabel::textOption() const
{
	return d->staticText.textOption();
}

void
TextLabel::setTextOption( const QTextOption & textOption )
{
	d->staticText.setTextOption( textOption );
}

int
TextLabel::heightForWidth( int w ) const
{
	QStaticText st = d->staticText;
	st.setTextWidth( w );
	return st.size().height() + 2 * frameWidth();
}

QSize
TextLabel::minimumSizeHint() const
{
	QStaticText st = d->staticText;
	st.setTextWidth( fontMetrics().averageCharWidth() * 10 );
	const QSizeF size = st.size();
	const int frame = 2 * frameWidth();

	return QSize( size.width() + frame, size.height() + frame );
}

QSize
TextLabel::sizeHint() const
{
	return minimumSizeHint();
}

void
TextLabel::paintEvent( QPaintEvent * e )
{
	QFrame::paintEvent( e );

	QPainter p( this );
	const QRect cr = contentsRect();

	p.setClipRect( cr );

	int vAlign = d->staticText.textOption().alignment() & Qt::AlignVertical_Mask;

	QPoint topLeft = cr.topLeft();

	switch( vAlign )
	{
		case Qt::AlignBottom :
			topLeft = QPoint( topLeft.x(),
				cr.bottomLeft().y() - qRound( d->staticText.size().height() ) );
		break;

		case Qt::AlignVCenter :
			topLeft = QPoint( topLeft.x(),
				topLeft.y() + cr.height() / 2 -
					qRound( d->staticText.size().height() ) / 2 );
		break;

		default :
			break;
	}

	p.drawStaticText( topLeft, d->staticText );
}

void
TextLabel::resizeEvent( QResizeEvent * e )
{
	d->staticText.setTextWidth( e->size().width() - 2 * frameWidth() );
	e->accept();
}

} /* namespace QtMWidgets */
