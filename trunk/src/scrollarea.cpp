
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
#include "scrollarea.hpp"
#include "private/scrollarea_p.hpp"
#include "private/layoutengine.hpp"

// Qt include.
#include <QEvent>
#include <QStyle>
#include <QLayout>
#include <QVariant>
#include <QPainter>


namespace QtMWidgets {

//
// ScrollAreaPrivate
//

void
ScrollAreaPrivate::updateWidgetPosition()
{
	ScrollArea * q = q_func();

	const Qt::LayoutDirection dir = q->layoutDirection();

	const QRect scrolled = QStyle::visualRect( dir, viewport->rect(),
		QRect( QPoint( -topLeftCorner.x(), -topLeftCorner.y() ), widget->size() ) );
	const QRect aligned = QStyle::alignedRect( dir, alignment, widget->size(),
		viewport->rect() );

	widget->move( widget->width() < viewport->width() ? aligned.x() : scrolled.x(),
		widget->height() < viewport->height() ? aligned.y() : scrolled.y() );
}

void
ScrollAreaPrivate::updateScrolledSize()
{
	ScrollArea * q = q_func();

	if( !widget )
		return;

	QSize p = viewport->size();

	QSize min = qSmartMinSize( widget );
	QSize max = qSmartMaxSize( widget );

	if( resizable )
	{
		if( ( widget->layout() ? widget->layout()->hasHeightForWidth() : widget->sizePolicy().hasHeightForWidth() ) )
		{
			QSize p_hfw = p.expandedTo( min ).boundedTo( max );
			int h = widget->heightForWidth( p_hfw.width() );
			min = QSize( p_hfw.width(), qMax( p.height(), h ) );
		}

		widget->resize( p.expandedTo( min ).boundedTo( max ) );
	}

	q->setScrolledAreaSize( widget->size() );

	updateWidgetPosition();
}


//
// ScrollArea
//

ScrollArea::ScrollArea( QWidget * parent )
	:	AbstractScrollArea( new ScrollAreaPrivate( this ), parent )
{
	ScrollAreaPrivate * d = d_func();
	d->viewport->setBackgroundRole( QPalette::NoRole );
}

ScrollArea::ScrollArea( ScrollAreaPrivate * dd, QWidget * parent )
	:	AbstractScrollArea( dd, parent )
{
	ScrollAreaPrivate * d = d_func();
	d->viewport->setBackgroundRole( QPalette::NoRole );
}

ScrollArea::~ScrollArea()
{
}

QWidget *
ScrollArea::widget() const
{
	const ScrollAreaPrivate * d = d_func();
	return d->widget;
}

void
ScrollArea::setWidget( QWidget * widget )
{
	ScrollAreaPrivate * d = d_func();

	if( widget == d->widget || !widget )
		return;

	delete d->widget;
	d->widget = 0;
	if( widget->parentWidget() != d->viewport )
		widget->setParent( d->viewport );
	if( !widget->testAttribute( Qt::WA_Resized ) )
		widget->resize( widget->sizeHint() );
	d->widget = widget;
	d->horIndicator->setParent( d->widget );
	d->horIndicator->raise();
	d->vertIndicator->setParent( d->widget );
	d->vertIndicator->raise();
	d->widget->setAutoFillBackground( true );
	widget->installEventFilter( this );
	d->updateScrolledSize();
	d->widget->show();
}

QWidget *
ScrollArea::takeWidget()
{
	ScrollAreaPrivate * d = d_func();
	QWidget * w = d->widget;
	w->removeEventFilter( this );
	d->horIndicator->setParent( d->viewport );
	d->vertIndicator->setParent( d->viewport );
	d->widget = 0;
	if( w )
		w->setParent( 0 );
	return w;
}

bool
ScrollArea::widgetResizable() const
{
	const ScrollAreaPrivate * d = d_func();
	return d->resizable;
}

void
ScrollArea::setWidgetResizable( bool resizable )
{
	ScrollAreaPrivate * d = d_func();

	if( d->resizable != resizable )
	{
		d->resizable = resizable;
		updateGeometry();
		d->updateScrolledSize();
	}
}

bool
ScrollArea::focusNextPrevChild( bool next )
{
	if( QWidget::focusNextPrevChild( next ) )
	{
		if( QWidget * fw = focusWidget() )
			ensureWidgetVisible( fw );

		return true;
	}

	return false;
}

Qt::Alignment
ScrollArea::alignment() const
{
	const ScrollAreaPrivate * d = d_func();

	return d->alignment;
}

void
ScrollArea::setAlignment( Qt::Alignment align )
{
	ScrollAreaPrivate * d = d_func();

	if( d->alignment != align )
	{
		d->alignment = align;

		if( d->widget )
			d->updateWidgetPosition();
	}
}

void
ScrollArea::ensureVisible( int x, int y, int xmargin, int ymargin )
{
	ScrollAreaPrivate * d = d_func();

	int logicalX = QStyle::visualPos( layoutDirection(), d->viewport->rect(),
		QPoint( x, y ) ).x();

	int posX = 0;
	int posY = 0;

	if( logicalX - xmargin < topLeftPointShownArea().x() )
		posX = qMax( 0, logicalX - xmargin );
	else if( logicalX > topLeftPointShownArea().x() + d->viewport->width() - xmargin )
		posX = qMin( logicalX - d->viewport->width() + xmargin,
			scrolledAreaSize().width() );

	if( y - ymargin < topLeftPointShownArea().y() )
		posY = qMax( 0, y - ymargin );
	else if( y > topLeftPointShownArea().y() + d->viewport->height() - ymargin )
		posY= qMin( y - d->viewport->height() + ymargin,
			scrolledAreaSize().height() );

	setTopLeftPointShownArea( QPoint( posX, posY ) );
}

void
ScrollArea::ensureWidgetVisible( QWidget * childWidget,
	int xmargin, int ymargin )
{
	ScrollAreaPrivate * d = d_func();

	if( !d->widget->isAncestorOf( childWidget ) )
		return;

	const QRect microFocus =
		childWidget->inputMethodQuery( Qt::ImCursorRectangle ).toRect();
	const QRect defaultMicroFocus =
		childWidget->QWidget::inputMethodQuery( Qt::ImCursorRectangle ).toRect();
	QRect focusRect = ( microFocus != defaultMicroFocus )
		? QRect( childWidget->mapTo( d->widget, microFocus.topLeft() ), microFocus.size() )
		: QRect( childWidget->mapTo( d->widget, QPoint( 0,0 ) ), childWidget->size() );
	const QRect visibleRect( -d->widget->pos(), d->viewport->size() );

	if( visibleRect.contains( focusRect ) )
		return;

	focusRect.adjust( -xmargin, -ymargin, xmargin, ymargin );

	int posX = 0;
	int posY = 0;

	if( focusRect.width() > visibleRect.width() )
		posX = focusRect.center().x() - d->viewport->width() / 2;
	else if( focusRect.right() > visibleRect.right() )
		posX = focusRect.right() - d->viewport->width();
	else if( focusRect.left() < visibleRect.left() )
		posX = focusRect.left();

	if( focusRect.height() > visibleRect.height() )
		posY = focusRect.center().y() - d->viewport->height() / 2;
	else if( focusRect.bottom() > visibleRect.bottom() )
		posY = focusRect.bottom() - d->viewport->height();
	else if( focusRect.top() < visibleRect.top() )
		posY = focusRect.top();

	setTopLeftPointShownArea( QPoint( posX, posY ) );
}

QSize
ScrollArea::sizeHint() const
{
	const ScrollAreaPrivate * d = d_func();

	QSize sz( 0, 0 );
	const int h = fontMetrics().height();

	if( d->widget )
		sz += ( d->resizable ? d->widget->sizeHint() : d->widget->size() );
	else
		sz += QSize( 12 * h, 8 * h );

	const QSize marginsSize = QSize( d->left + d->right, d->top + d->bottom );
	const QSize frameSize = QSize( frameWidth() * 2, frameWidth() * 2 );

	sz += marginsSize + frameSize;

	return sz.boundedTo( QSize( 36 * h, 24 * h ) );
}

bool
ScrollArea::eventFilter( QObject * o, QEvent * e )
{
	ScrollAreaPrivate * d = d_func();

	if( o == d->widget && e->type() == QEvent::Resize )
		d->updateScrolledSize();

	return AbstractScrollArea::eventFilter( o, e );
}

void
ScrollArea::resizeEvent( QResizeEvent * e )
{
	AbstractScrollArea::resizeEvent( e );

	ScrollAreaPrivate * d = d_func();
	d->updateScrolledSize();
}

void
ScrollArea::scrollContentsBy( int, int )
{
	ScrollAreaPrivate * d = d_func();

	if( !d->widget )
		return;

	d->updateWidgetPosition();
}

} /* namespace QtMWidgets */
