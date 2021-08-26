
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

#ifndef QTMWIDGETS__SLIDER_HPP__INCLUDED
#define QTMWIDGETS__SLIDER_HPP__INCLUDED

// Qt include.
#include <QAbstractSlider>
#include <QScopedPointer>


namespace QtMWidgets {

//
// Slider
//

class SliderPrivate;

/*!
	This is the most common slider, almost identical to QSlider.
	But it doesn't support styles, but it acceptable to use
	on mobile platforms. It has big slider's handle and highlights
	position of the handle in the groove.
*/
class Slider
	:	public QAbstractSlider
{
	Q_OBJECT
	/*!
		\property handleRadius
		\brief radius of the handle of the slider.

		By default handle radius calculated from widget's font and equals
		to QFontMetrics::ascent().
	*/
	Q_PROPERTY( int handleRadius READ handleRadius WRITE setHandleRadius )
	/*!
		\property highlightColor

		\brief color used to highlight the value

		By default this color is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor highlightColor READ highlightColor WRITE setHighlightColor )
	/*!
		\property grooveHeight

		\brief Height (width) of the slider's groove

		By default is 2px.
	*/
	Q_PROPERTY( int grooveHeight READ grooveHeight WRITE setGrooveHeight )

public:
	Slider( QWidget * parent = 0 );
	explicit Slider( Qt::Orientation orientation, QWidget * parent = 0 );

	virtual ~Slider();

	//! \return Handle radius.
	int handleRadius() const;
	//! Set handle radius.
	void setHandleRadius( int r );

	//! \return Highlight color.
	const QColor & highlightColor() const;
	//! Set highlight color.
	void setHighlightColor( const QColor & c );

	//! \return Groove height.
	int grooveHeight() const;
	//! Set groove height.
	void setGrooveHeight( int h );

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

protected:
	void paintEvent( QPaintEvent * e ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;
	void mouseMoveEvent( QMouseEvent * e ) override;

private:
	Q_DISABLE_COPY( Slider )

	QScopedPointer< SliderPrivate > d;
}; // class Slider

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__SLIDER_HPP__INCLUDED
