
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

#ifndef QTMWIDGETS__ABSTRACTSCROLLAREA_HPP__INCLUDED
#define QTMWIDGETS__ABSTRACTSCROLLAREA_HPP__INCLUDED

// Qt include.
#include <QFrame>
#include <QScopedPointer>


namespace QtMWidgets {

class Scroller;


//
// AbstractScrollArea
//

class AbstractScrollAreaPrivate;

/*!
	\brief The AbstractScrollArea widget provides a scrolling area with
	on-demand scrolling indicators.

	AbstractScrollArea is a low-level abstraction of a scrolling
	area. The area provides a central widget called the viewport, in
	which the contents of the area is to be scrolled (i.e, the
	visible parts of the contents are rendered in the viewport).

	It is possible to reserve a margin area around the viewport, see
	setViewportMargins(). Subclasses of AbstractScrollArea should
	implement margins.

	When inheriting AbstractScrollArea, you need to do the
	following:

	\li Control the scrolled area size and top-left corner position
		of shown part of scrolled area.
	\li Draw the contents of the area in the viewport according
		to the value of the top-left position of shown area.
	\li Use \c{viewport->update()} to update the contents of the
		viewport instead of \link{QWidget::update()}{update()}\endlink
		as all painting operations take place on the viewport.

	ScrollArea, which inherits AbstractScrollArea, provides smooth
	scrolling for any QWidget (i.e., the widget is scrolled pixel by
	pixel). You only need to subclass AbstractScrollArea if you need
	more specialized behaviour. This is, for instance, true if the
	entire contents of the area is not suitable for being drawn on a
	QWidget or if you do not want smooth scrolling.

	\sa ScrollArea
*/
class AbstractScrollArea
	:	public QFrame
{
	Q_OBJECT

	/*!
		\property indicatorColor

		\brief Color used to paint scrolling indicators.

		By default this color is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor indicatorColor READ indicatorColor WRITE setIndicatorColor )
	/*!
		\property verticalScrollIndicatorPolicy

		\brief Policy specified to display or not vertical scroll indicator.

		By default this property is ScrollIndicatorAsNeeded.
	*/
	Q_PROPERTY( ScrollIndicatorPolicy verticalScrollIndicatorPolicy
		READ verticalScrollIndicatorPolicy
		WRITE setVerticalScrollIndicatorPolicy )
	/*!
		\property horizontalScrollIndicatorPolicy

		\brief Policy specified to display or not horizontal scroll indicator.

		By default this property is ScrollIndicatorAsNeeded.
	*/
	Q_PROPERTY( ScrollIndicatorPolicy horizontalScrollIndicatorPolicy
		READ horizontalScrollIndicatorPolicy
		WRITE setHorizontalScrollIndicatorPolicy )
	/*!
		\property blurColor

		\brief Color of the blur effect.

		By default, this property is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor blurColor READ blurColor WRITE setBlurColor )
	/*!
		\property blurPolicy

		\brief Policy of the blur effect.

		By default, this property is BlurAlwaysOff.
	*/
	Q_PROPERTY( BlurPolicy blurPolicy READ blurPolicy WRITE setBlurPolicy )

public:
	/*!
		This enum describes the various modes of scroll indicators.
	*/
	enum ScrollIndicatorPolicy {
		/*!
			AbstractScrollArea shows scroll indicators when content
			is too large to fit and not otherwise. This is default.
		*/
		ScrollIndicatorAsNeeded = 0,
		//! AbstractScrollArea never shows scroll indicators.
		ScrollIndicatorAlwaysOff = 1,
		//! AbstractScrollArea always shows scroll indicators.
		ScrollIndicatorAlwaysOn = 2
	}; // enum ScrollIndicatorPolicy

	/*!
		This enum describes the various modes of blur effect.

		Blur effect shows when scrolling reached the end, that
		indicates that scrolling in this direction not more possible.
	*/
	enum BlurPolicy {
		//! Blur both scrolling directions.
		BlurBothDirections = 0,
		//! Blur horizontal scrolling only.
		BlurHorizontalOnly = 1,
		//! Blur vertical scrolling only.
		BlurVerticalOnly = 2,
		//! Blur always off.
		BlurAlwaysOff = 3
	}; // enum BlurPolicy

	Q_ENUM( ScrollIndicatorPolicy )
	Q_ENUM( BlurPolicy )

public:
	AbstractScrollArea( QWidget * parent = 0 );
	virtual ~AbstractScrollArea();

	//! \return Viewport widget.
	QWidget * viewport() const;
	//! Set viewport widget.
	void setViewport( QWidget * widget );
	/*!
		This method is called by AbstractScrollArea after setViewport(\a
		viewport) has been called. Reimplement this function in a
		subclass of AbstractScrollArea to initialize the new \a viewport
		before it is used.

		\sa setViewport()
	*/
	virtual void setupViewport( QWidget * viewport );

	//! \return Scroller interface.
	Scroller * scroller() const;

	//! \return Color used to paint scrolling indicators.
	QColor indicatorColor() const;
	//! Set color used to paint scrolling indicator.
	void setIndicatorColor( const QColor & c );

	//! \return Vertical scroll indicator policy.
	ScrollIndicatorPolicy verticalScrollIndicatorPolicy() const;
	//! Set vertical scroll indicator policy.
	void setVerticalScrollIndicatorPolicy( ScrollIndicatorPolicy policy );

	//! \return Horizontal scroll indicator policy.
	ScrollIndicatorPolicy horizontalScrollIndicatorPolicy() const;
	//! Set horizontal scroll indicator policy.
	void setHorizontalScrollIndicatorPolicy( ScrollIndicatorPolicy policy );

	//! \return Blur color.
	const QColor & blurColor() const;
	//! Set blur color.
	void setBlurColor( const QColor & c );

	//! \return Blur policy.
	BlurPolicy blurPolicy() const;
	//! Set blur policy.
	void setBlurPolicy( BlurPolicy policy );

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	explicit AbstractScrollArea( AbstractScrollAreaPrivate * dd,
		QWidget * parent = 0 );

	//! Set viewport margins.
	void setViewportMargins( int left, int top, int right, int bottom );
	//! Set viewport margins.
	void setViewportMargins( const QMargins & margins );

	/*!
		Scroll contents. Should be reimplemented in a subclass.
	*/
	virtual void scrollContentsBy( int dx, int dy ) = 0;

	//! \return Size of the scrolled area.
	const QSize & scrolledAreaSize() const;
	//! Set scrolled area size.
	void setScrolledAreaSize( const QSize & s );

	//! \return Top-left corner of the shown scrolled area.
	const QPoint & topLeftPointShownArea() const;
	/*!
		Set top-left corner of the shown scrolled area.

		Do not call this method inside scrollContentsBy(),
		because it leads to the recursion.
	*/
	void setTopLeftPointShownArea( const QPoint & p );

	//! Start animation of fading scroll indicators.
	void startScrollIndicatorsAnimation();

	void resizeEvent( QResizeEvent * e ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;
	void mouseMoveEvent( QMouseEvent * e ) override;
	void wheelEvent( QWheelEvent * e ) override;

protected:
	QScopedPointer< AbstractScrollAreaPrivate > d;

	friend class AbstractScrollAreaPrivate;

private slots:
	void _q_animateScrollIndicators();
	void _q_kineticScrolling( int dx, int dy );
	void _q_kineticScrollingAboutToStart();
	void _q_kineticScrollingFinished();
	void _q_horBlurAnim( const QVariant & value );
	void _q_horBlurAnimFinished();
	void _q_vertBlurAnim( const QVariant & value );
	void _q_vertBlurAnimFinished();
	void _q_startBlurAnim();

private:
	Q_DISABLE_COPY( AbstractScrollArea )
}; // class AbstractScrollArea

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__ABSTRACTSCROLLAREA_HPP__INCLUDED
