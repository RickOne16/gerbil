#ifndef AUTOHIDEWIDGET_H
#define AUTOHIDEWIDGET_H

#include <QWidget>
#include <QGraphicsProxyWidget>

class AutohideWidget : public QWidget
{
	Q_OBJECT
public:
	enum border {
		LEFT, RIGHT, TOP, BOTTOM
	};

#define AutohideWidget_border_String { "LEFT", "RIGHT", "TOP", "BOTTOM" }

	// visibility state
	enum scrollstate {
		SCROLL_IN, // scrolling into/residing in view
		SCROLL_OUT, // scrolling out of view / hidden
		STAY_IN, // scroll into view and stay there
		STAY_OUT // scroll out and keep out
	};

	/* does not take a parent. Widgets in QGraphicsView must always be
	 * top-level (no parent).
	 */
	explicit AutohideWidget();

	/* current widget state */
	scrollstate scrollState() { return state; }

	/* this method is called with the corresponding proxy item as argument.
	 * the widget will use it as its anchor in the graphics scene.
	 * @loc the border this widget resides in
	 */
	void init(QGraphicsProxyWidget *proxy, border loc);
	
	/* adjust position and stretch the widget to fill the whole border */
	void reposition();

	/* decide if scrolling needed based on mouse position */
	void triggerScrolling(QPoint pos);

	/* Call this instead of QWidget::adjustSize()
	 * Whenever the widget gets a new size, we also have to reset its position.
	 */
	void adjust();

	/** Returns the border where this widget resides. */
	border getLocation() const {
		return location;
	}

	void setTriggerOffset(int offset) {
		triggerOffset = offset;
	}

	// how many pixels far from the widget a scroll-in is triggered
	int triggerOffset;

	// how many pixels the widget lurks into the view while scrolled out
	static const int OutOffset = 14;

signals:
	
public slots:
	// get into view, stay there indefinitely if enforce is set
	virtual void scrollIn(bool enforce = false);
	// get out of view, stay there indefinitely if enforce is set
	virtual void scrollOut(bool enforce = false);
	// start the actual scrolling (scrolling out is delayed a bit)
	void scrollOutNow();

protected:
	// which border we are in
	border location;
	// scrolled in our out, forced or not
	scrollstate state;

	// a small pixmap indicating that we can be scrolled-in, used in paintEvent
	QPixmap indicator;
	// only show it in certain states
	bool show_indicator;

	// our connection to the graphics view
	QGraphicsProxyWidget *proxy;

	// draw a nice arrow indicator and then the interior widgets (base class)
	virtual void paintEvent(QPaintEvent *);

	virtual void timerEvent(QTimerEvent *e);
	virtual void changeEvent(QEvent *e);
};

std::ostream& operator<<(std::ostream& o, AutohideWidget::border e);

#endif // AUTOHIDEWIDGET_H
