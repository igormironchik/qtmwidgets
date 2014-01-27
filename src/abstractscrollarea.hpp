
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


namespace QtMWidgets {

//
// AbstractScrollArea
//

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

	\li Draw the contents of the area in the viewport according
		to the values of the offsets.
	\li Use \c{viewport->update()} to update the contents of the
		viewport instead of \link{QWidget::update()}{update()}\endlink
		as all painting operations take place on the viewport.

	With a scroll indicator policy of ScrollIndicatorAsNeeded (the default),
	AbstractScrollArea shows scroll indicators when user scrolls area,
	and hides them automatically with timeout.

	ScrollArea, which inherits AbstractScrollArea, provides smooth
	scrolling for any QWidget (i.e., the widget is scrolled pixel by
	pixel). You only need to subclass AbstractScrollArea if you need
	more specialized behavior. This is, for instance, true if the
	entire contents of the area is not suitable for being drawn on a
	QWidget or if you do not want smooth scrolling.

	\sa ScrollArea
*/

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__ABSTRACTSCROLLAREA_HPP__INCLUDED
