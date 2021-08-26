
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2015 Igor Mironchik

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

// Qt include.
#include <QAbstractButton>
#include <QFontMetrics>
#include <QPainter>
#include <QTextOption>
#include <QMap>
#include <QList>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QApplication>
#include <QScreen>

// QtMWidgets include.
#include "messagebox.hpp"
#include "fingergeometry.hpp"
#include "scrollarea.hpp"
#include "textlabel.hpp"
#include "private/messageboxbutton.hpp"


namespace QtMWidgets {

class MsgBoxTitle;


//
// MsgBoxTitlePrivate
//

class MsgBoxTitlePrivate {
public:
	MsgBoxTitlePrivate( const QString & t, MsgBoxTitle * parent )
		:	q( parent )
		,	title( t )
		,	margin( 10 )
	{
	}

	void init();
	void prepareTitle();

	MsgBoxTitle * q;
	QString title;
	int margin;
	QString preparedTitle;
}; // class MsgBoxTitlePrivate


//
// MsgBoxTitle
//

class MsgBoxTitle
	:	public QWidget
{
public:
	explicit MsgBoxTitle( const QString & title, QWidget * parent = 0 )
		:	QWidget( parent )
		,	d( new MsgBoxTitlePrivate( title, this ) )
	{
		d->init();
	}

	virtual ~MsgBoxTitle()
	{
	}

protected:
	void paintEvent( QPaintEvent * ) override
	{
		QPainter p( this );

		p.setPen( palette().color( QPalette::WindowText ) );

		p.drawText( rect().marginsRemoved( QMargins( d->margin, 0,
				d->margin, 0 ) ),
			d->preparedTitle, QTextOption( Qt::AlignLeft | Qt::AlignVCenter ) );
	}

	void resizeEvent( QResizeEvent * ) override
	{
		d->prepareTitle();
	}

private:
	Q_DISABLE_COPY( MsgBoxTitle )

	QScopedPointer< MsgBoxTitlePrivate > d;
}; // class MsgBoxTitle


//
// MsgBoxTitlePrivate
//

void
MsgBoxTitlePrivate::init()
{
	q->setBackgroundRole( QPalette::Window );
	q->setAutoFillBackground( true );
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	q->setMinimumHeight( qMax( FingerGeometry::height(),
		q->fontMetrics().height() + q->fontMetrics().height() / 3 ) );

	prepareTitle();
}

void
MsgBoxTitlePrivate::prepareTitle()
{
	const QRect r = q->rect().marginsRemoved( QMargins( margin, 0,
		margin, 0 ) );

	const QRect & b = q->fontMetrics().boundingRect( r,
		Qt::AlignLeft, title );

	preparedTitle = title;

	if( b.width() > r.width() )
	{
		const int averageCount = r.width() / q->fontMetrics().averageCharWidth();

		preparedTitle = title.left( averageCount - 3 );
		preparedTitle.append( QLatin1String( "..." ) );
	}
}


//
// MessageBoxPrivate
//

class MessageBoxPrivate {
public:
	MessageBoxPrivate( const QString & titl,
		const QString & txt, MessageBox * parent, QWidget * w )
		:	q( parent )
		,	frame( 0 )
		,	vbox( 0 )
		,	hbox( 0 )
		,	title( 0 )
		,	scrollArea( 0 )
		,	textLabel( 0 )
		,	okButton( 0 )
		,	clickedButton( 0 )
		,	screenMargin( 6 )
		,	window( w )
		,	h1( 0 )
		,	h2( 0 )
	{
		init( titl, txt );
	}

	void init( const QString & titl, const QString & txt );
	void adjustSize();

	MessageBox * q;
	QMap< QAbstractButton*, MessageBox::ButtonRole > buttonsMap;
	QFrame * frame;
	QVBoxLayout * vbox;
	QHBoxLayout * hbox;
	MsgBoxTitle * title;
	ScrollArea * scrollArea;
	TextLabel * textLabel;
	MsgBoxButton * okButton;
	QList< QFrame * > buttonSeparators;
	QAbstractButton * clickedButton;
	QList< QAbstractButton* > buttons;
	int screenMargin;
	QWidget * window;
	QFrame * h1;
	QFrame * h2;
}; // class MessageBoxPrivate

void
MessageBoxPrivate::init( const QString & titl, const QString & txt )
{
	q->setModal( true );

	QVBoxLayout * layout = new QVBoxLayout( q );
	layout->setContentsMargins( 0, 0, 0, 0 );

	frame = new QFrame( q );
	frame->setFrameStyle( QFrame::Box | QFrame::Plain );
	layout->addWidget( frame );

	vbox = new QVBoxLayout( frame );
	vbox->setSpacing( 0 );
	vbox->setContentsMargins( 3, 3, 3, 3 );

	title = new MsgBoxTitle( titl, frame );
	vbox->addWidget( title );

	h1 = new QFrame( frame );
	h1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	vbox->addWidget( h1 );

	scrollArea = new ScrollArea( frame );
	scrollArea->setWidgetResizable( true );

	textLabel = new TextLabel( frame );
	textLabel->setBackgroundRole( QPalette::Window );
    textLabel->setColor( q->palette().color( QPalette::WindowText ) );
	textLabel->setAutoFillBackground( true );
	textLabel->setText( txt );
	scrollArea->setWidget( textLabel );

	vbox->addWidget( scrollArea );

	h2 = new QFrame( frame );
	h2->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	vbox->addWidget( h2 );

	hbox = new QHBoxLayout;
	hbox->setSpacing( 0 );
	hbox->setContentsMargins( 0, 0, 0, 0 );

	okButton = new MsgBoxButton( QObject::tr( "OK" ), frame );
	buttonsMap.insert( okButton, MessageBox::AcceptRole );
	buttons.append( okButton );
	hbox->addWidget( okButton );

	vbox->addLayout( hbox );

	q->resize( vbox->sizeHint() );
}

void
MessageBoxPrivate::adjustSize()
{
	QSize ws = QApplication::primaryScreen()->availableSize();
	QSize s = q->size();
	QRect wr = QApplication::primaryScreen()->availableGeometry();

	if( window )
	{
		ws = window->size();
		wr = window->rect();
	}

	{
		int width = 0;

		foreach( QAbstractButton * btn, buttons )
			width += btn->sizeHint().width();

		if( width > s.width() )
			s = QSize( width,
					textLabel->heightForWidth( width ) + title->height() +
						okButton->height() + h1->height() + h2->height() );
	}

	qreal factor = (qreal) s.height() / (qreal) s.width();

	if( factor > 1.5 )
	{
		const int width = qRound( (qreal) s.width() * factor );

		s = QSize( width,
				textLabel->heightForWidth( width ) + title->height() +
					okButton->height() + h1->height() + h2->height() );
	}

	if( s.width() > ws.width() - screenMargin * 2 )
	{
		const int width = ws.width() - screenMargin * 2;

		s = QSize( width,
			textLabel->heightForWidth( width ) + title->height() +
				okButton->height() + h1->height() + h2->height() );
	}

	if( s.height() > ws.height() - screenMargin * 2 )
		s = QSize( s.width(), ws.height() - screenMargin * 2 );


	q->resize( s );

	QPoint p = QPoint( wr.x() + ( ws.width() - s.width() ) / 2,
		wr.y() + ( ws.height() - s.height() ) / 2 );

	if( window )
		p = window->mapToGlobal( p );

	q->move( p );

	vbox->update();
}


//
// MessageBox
//

MessageBox::MessageBox( const QString & title,
	const QString & text, QWidget * parent )
	:	QDialog( parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint )
	,	d( new MessageBoxPrivate( title, text, this, parent ) )
{
	setAttribute( Qt::WA_MacAlwaysShowToolWindow, true );

	connect( d->okButton, &MsgBoxButton::clicked,
		this, &MessageBox::_q_clicked );

	d->adjustSize();
}

MessageBox::~MessageBox()
{
}

void
MessageBox::addButton( QAbstractButton * button, ButtonRole role )
{
	if( !d->buttonsMap.contains( button ) )
	{
		QFrame * line = new QFrame( d->frame );
		line->setFrameStyle( QFrame::VLine | QFrame::Sunken );

		d->buttonSeparators.append( line );
		d->buttonsMap.insert( button, role );
		d->buttons.append( button );

		d->hbox->addWidget( line );
		d->hbox->addWidget( button );

		connect( button, &QAbstractButton::clicked,
			this, &MessageBox::_q_clicked );

		resize( d->vbox->sizeHint() );

		d->adjustSize();
	}
}

QAbstractButton *
MessageBox::addButton( const QString & text, ButtonRole role )
{
	MsgBoxButton * button = new MsgBoxButton( text, d->frame );

	addButton( button, role );

	return button;
}

QList< QAbstractButton* >
MessageBox::buttons( ButtonRole role ) const
{
	QList< QAbstractButton* > list;

	for( QMap< QAbstractButton*, ButtonRole >::const_iterator
		it = d->buttonsMap.constBegin(), last = d->buttonsMap.constEnd();
		it != last; ++it )
	{
		if( it.value() == role )
			list.append( it.key() );
	}

	return list;
}

MessageBox::ButtonRole
MessageBox::buttonRole( QAbstractButton * button ) const
{
	if( d->buttonsMap.contains( button ) )
		return d->buttonsMap[ button ];
	else
		return InvalidRole;
}

const QList< QAbstractButton* > &
MessageBox::buttons() const
{
	return d->buttons;
}

QAbstractButton *
MessageBox::clickedButton() const
{
	return d->clickedButton;
}

void
MessageBox::removeButton( QAbstractButton * button )
{
	if( d->okButton != button && d->buttonsMap.contains( button ) )
	{
		const int index = d->buttons.indexOf( button );

		if( index != -1 )
		{
			QLayoutItem * b = d->hbox->takeAt( index * 2 );
			QLayoutItem * l = d->hbox->takeAt( index * 2 - 1 );

			if( l->widget() )
				l->widget()->deleteLater();

			delete b;
			delete l;

			d->buttons.removeAt( index );
			d->buttonsMap.remove( button );
			d->buttonSeparators.removeAt( index - 1 );

			disconnect( button, 0, this, 0 );

			button->deleteLater();

			resize( d->vbox->sizeHint() );

			d->adjustSize();
		}
	}
}

QString
MessageBox::text() const
{
	return d->textLabel->text();
}

void
MessageBox::setText( const QString & t )
{
	d->textLabel->setText( t );

	d->adjustSize();
}

Qt::TextFormat
MessageBox::textFormat() const
{
	return d->textLabel->textFormat();
}

void
MessageBox::setTextFormat( Qt::TextFormat fmt )
{
	d->textLabel->setTextFormat( fmt );
}

void
MessageBox::_q_clicked()
{
	d->clickedButton = qobject_cast< QAbstractButton* > ( sender() );

	ButtonRole role = d->buttonsMap[ d->clickedButton ];

	switch( role )
	{
		case AcceptRole :
			accept();
			break;

		case RejectRole :
			reject();
			break;

		default :
			break;
	}

	emit buttonClicked( d->clickedButton );
}

} /* namespace QtMWidgets */
