#include <QtGui>
#include "GlyphPainter.h"

GlyphPainter::GlyphPainter() {
	background = QBrush(Qt::black);
	pen = QPen(Qt::white);
	pen.setWidth(2);
}

void GlyphPainter::paint(QPainter* painter, QPaintEvent* event, int elapsed) {
	int n = _glyph->size()-1;
	painter->fillRect(event->rect(), background);
	painter->translate(200,200);
	painter->save();
	QPen firstNode(Qt::green);
	firstNode.setWidth(1);
	QPen midNode(Qt::cyan);
	midNode.setWidth(1);
	QPen lastNode(Qt::red);
	lastNode.setWidth(1);
	QPen activeNode(Qt::white);
	activeNode.setWidth(2);

	int i,k;
	double t;
	for(i = 0; i < n; i++) {
		painter->setPen(QPen(Qt::darkGray));
		painter->drawLine(_glyph->at(i)*200, _glyph->at(i+1)*200);
		if(_activePoints->contains(_glyph->at(i))) {
			painter->setPen(activeNode);
		} else if(i == 0){
			painter->setPen(firstNode);
		} else {
			painter->setPen(midNode);
		}
		painter->drawEllipse(_glyph->at(i)*200, 5, 5);
	}
	if(_activePoints->contains(_glyph->at(i))) {
		painter->setPen(activeNode);
	} else {
		painter->setPen(lastNode);
	}
	painter->drawEllipse(_glyph->at(i)*200, 5, 5);

	painter->setPen(QPen(Qt::yellow));
	unsigned long long* b = new unsigned long long[n+1];
	for(k = 0; k <= n; k++) {
		b[k] = choose(n,k);
	}
	for(t = 0; t < 1; t += _t_step) {
		double x = 0, y = 0;
		for(k = 0; k <= n; k++) {
			x += b[k]*pow(1-t, (n-k))*pow(t, k)*_glyph->at(k).x();
			y += b[k]*pow(1-t, (n-k))*pow(t, k)*_glyph->at(k).y();
		}
		painter->drawPoint(QPointF(x,y)*200);
	}

	painter->restore();
}

unsigned long long GlyphPainter::choose(unsigned n, unsigned k)  {
	if(k > n) {
		return 0;
	}

	if(k > n/2) {
		k = n-k;
	}

	long double a = 1;
	unsigned i;
	for(i = 1; i <= k; i++) {
		a = a * (n-k+i) / i;
	}
	return (unsigned long long)ceil(a);
}
