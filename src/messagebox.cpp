
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

// QtMWidgets include.
#include "messagebox.hpp"
#include "fingergeometry.hpp"
#include "scrollarea.hpp"
#include "textlabel.hpp"


namespace QtMWidgets {

class MsgBoxButton;

//
// MsgBoxButtonPrivate
//

class MsgBoxButtonPrivate {
public:
	MsgBoxButtonPrivate( const QString & t, MsgBoxButton * parent )
		:	q( parent )
		,	text( t )
	{
	}

	void init();

	MsgBoxButton * q;
	QString text;
}; // class MsgBoxButtonPrivate


//
// MsgBoxButton
//

class MsgBoxButton
	:	public QAbstractButton
{
public:
	explicit MsgBoxButton( const QString & text, QWidget * parent = 0 )
		:	QAbstractButton( parent )
		,	d( new MsgBoxButtonPrivate( text, this ) )
	{
		d->init();
	}

	virtual ~MsgBoxButton()
	{
	}

	virtual QSize minimumSizeHint() const
	{
		const int margin = fontMetrics().height() / 3;

		const QSize s = fontMetrics()
			.boundingRect( QRect(), Qt::AlignCenter, d->text )
			.marginsAdded( QMargins( margin, margin, margin, margin ) )
			.size();

		return QSize( qMax( s.width(), FingerGeometry::width() ),
			qMax( s.height(), FingerGeometry::height() ) );
	}

	virtual QSize sizeHint() const
	{
		return minimumSizeHint();
	}

protected:
	virtual void paintEvent( QPaintEvent * )
	{
		QPainter p( this );

		p.setPen( palette().color( QPalette::WindowText ) );

		p.drawText( rect(), d->text, QTextOption( Qt::AlignCenter ) );
	}

private:
	Q_DISABLE_COPY( MsgBoxButton )

	QScopedPointer< MsgBoxButtonPrivate > d;
}; // class MsgBoxButton


//
// MsgBoxButtonPrivate
//

void
MsgBoxButtonPrivate::init()
{
	q->setBackgroundRole( QPalette::Window );
	q->setAutoFillBackground( true );
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
}


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
	virtual void paintEvent( QPaintEvent * )
	{
		QPainter p( this );

		p.setPen( palette().color( QPalette::WindowText ) );

		p.drawText( rect().marginsRemoved( QMargins( d->margin, 0,
				d->margin, 0 ) ),
			d->preparedTitle, QTextOption( Qt::AlignLeft | Qt::AlignVCenter ) );
	}

	virtual void resizeEvent( QResizeEvent * )
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
		,	vbox( 0 )
		,	hbox( 0 )
		,	title( 0 )
		,	scrollArea( 0 )
		,	textLabel( 0 )
		,	okButton( 0 )
		,	clickedButton( 0 )
		,	screenMargin( 6 )
		,	window( w )
	{
		init( titl, txt );
	}

	void init( const QString & titl, const QString & txt );
	void adjustSize();

	MessageBox * q;
	QMap< QAbstractButton*, MessageBox::ButtonRole > buttonsMap;
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
}; // class MessageBoxPrivate

void
MessageBoxPrivate::init( const QString & titl, const QString & txt )
{
	q->setModal( true );

	vbox = new QVBoxLayout( q );
	vbox->setSpacing( 0 );
	vbox->setContentsMargins( 3, 3, 3, 3 );

	title = new MsgBoxTitle( titl, q );
	vbox->addWidget( title );

	QFrame * h1 = new QFrame( q );
	h1->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	vbox->addWidget( h1 );

	scrollArea = new ScrollArea( q );
	scrollArea->setWidgetResizable( true );

	textLabel = new TextLabel( q );
	textLabel->setBackgroundRole( QPalette::Window );
	textLabel->setAutoFillBackground( true );
	textLabel->setText( txt );
	scrollArea->setWidget( textLabel );

	vbox->addWidget( scrollArea );

	QFrame * h2 = new QFrame( q );
	h2->setFrameStyle( QFrame::HLine | QFrame::Sunken );
	vbox->addWidget( h2 );

	hbox = new QHBoxLayout;
	hbox->setSpacing( 0 );
	hbox->setContentsMargins( 0, 0, 0, 0 );

	okButton = new MsgBoxButton( QObject::tr( "OK" ), q );
	buttonsMap.insert( okButton, MessageBox::AcceptRole );
	buttons.append( okButton );
	hbox->addWidget( okButton );

	vbox->addLayout( hbox );
}

void
MessageBoxPrivate::adjustSize()
{
	if( window )
	{
		const QSize ws = window->window()->size();
		const QSize s = q->size();
		const QRect wr = window->window()->rect();

		if( s.width() > ws.width() - screenMargin * 2 ||
			s.height() > ws.height() - screenMargin * 2 )
		{
			q->resize( QSize( qMin( s.width(), ws.width() - screenMargin * 2 ),
				qMin( s.height(), ws.height() - screenMargin * 2 ) ) );

			q->move( wr.x() + ( ws.width() - q->width() ) / 2,
				wr.y() + ( ws.height() - q->height() ) / 2 );

			vbox->update();
		}
	}
}


//
// MessageBox
//

MessageBox::MessageBox( const QString & title,
	const QString & text, QWidget * parent )
	:	QDialog( parent, Qt::Dialog | Qt::ToolTip | Qt::FramelessWindowHint )
	,	d( new MessageBoxPrivate( title, text, this, parent ) )
{
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
		QFrame * line = new QFrame( this );
		line->setFrameStyle( QFrame::VLine | QFrame::Sunken );

		d->buttonSeparators.append( line );
		d->buttonsMap.insert( button, role );
		d->buttons.append( button );

		d->hbox->addWidget( line );
		d->hbox->addWidget( button );

		connect( button, &QAbstractButton::clicked,
			this, &MessageBox::_q_clicked );
	}
}

QAbstractButton *
MessageBox::addButton( const QString & text, ButtonRole role )
{
	MsgBoxButton * button = new MsgBoxButton( text, this );

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
MessageBox::resizeEvent( QResizeEvent * e )
{
	d->adjustSize();

	e->accept();
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
