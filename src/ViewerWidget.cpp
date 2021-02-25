#include   "ViewerWidget.h"

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
	}

}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
	areaSize.setWidth(size.width());
	areaSize.setHeight(size.height());
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();
	

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

//Draw functions
void ViewerWidget::freeDraw(QPoint end, QPen pen)
{
	painter->setPen(pen);
	painter->drawLine(freeDrawBegin, end);

	update();
}

void ViewerWidget::clear()
{
	img->fill(Qt::white);
	update();
}

void ViewerWidget::VykresliOsi(int Hustota) {

	int i;
	int zlomok = 22;		//aby sa vsetko scalovalo dobre; empiricky som prisiel k cislu 22

	QPen pero;
	pero.setWidth(2);
	painter->setPen(pero);
	
	int x1 = areaSize.width() / zlomok;
	int y1 = areaSize.height() / 2;

	QPoint B1(x1 - (x1 / (zlomok / 5)), y1);
	QPoint B2(areaSize.width() - x1 + (x1 / (zlomok / 5)), y1);
	painter->drawLine(B1, B2);		//os x

	int x2 = areaSize.width() / 2;
	int y2 = areaSize.height() / zlomok;
	QPoint B3(x2, y2 - y2 / (zlomok / 5));
	QPoint B4(x2, areaSize.height() - y2 + y2 / (zlomok / 5));
	painter->drawLine(B3,B4);		//os y

	int x_stred = areaSize.width() / 2;
	int y_stred = areaSize.height() / 2;
	int drazka;

	if (areaSize.width() > areaSize.height()) {		//nastavenie rovnakej velkosti drazok, ale aby to bola mensia velkost
		drazka = areaSize.height() / (zlomok * 5);
	}
	else {
		drazka = areaSize.width() / (zlomok * 5);
	}

	double x_dielik = (x_stred - x1 ) / (Hustota / 2.0);		//dielik, po ktorom sa maju drazky posuvat od stredu
	int x_vrch = (areaSize.height() / 2) - drazka;
	int x_spodok = (areaSize.height() / 2) + drazka;

	double y_dielik = (y_stred - y2) / (Hustota / 2.0);
	int y_vlavo = (areaSize.width() / 2) - drazka;
	int y_vpravo = (areaSize.width() / 2) + drazka;

	QPoint C1;
	QPoint C2;
	QPoint C3;
	QPoint C4;
	
	for (i = 0; i <= Hustota; i++) {				//drazka najdalej je od stredu vo vzdialenosti x_stred - x1   ;   y_stred - y1
		//C1.setX(areaSize.width() - x1 - round(i * x_dielik));
		C1.setX(x1 + i * x_dielik);
		C2.setX(x1 + i * x_dielik);
		C1.setY(x_vrch);
		//C2.setX(areaSize.width() - x1 - round(i * x_dielik));
		C2.setY(x_spodok);
		painter->drawLine(C1, C2);
		
		//painter->drawLine(C1, C2);

		C3.setY(y2 + i * y_dielik);
		C4.setY(y2 + i * y_dielik);
		C3.setX(y_vlavo);
		//C3.setY(areaSize.height() - y2 - round(i * y_dielik));
		C4.setX(y_vpravo);
		//C4.setY(areaSize.height() - y2 - round(i * y_dielik));
		painter->drawLine(C3, C4);

		//painter->drawLine(C3, C4);
	}

	QPoint D1(B1.x(), B1.y() - (B1.y() / (zlomok * 2)));		//body pre napisanie "x" a "y"
	QPoint D2(B3.x() + (B3.x() / (zlomok * 2)),B3.y());

	if (B1.x() > B3.y()) {									//nastavenie rovnakej velkosti oznacenia osi, ale aby to bolo oznacene podla mensieho dieliku
		painter->setFont(QFont("Arial", B3.y() / 2));
	}
	else {
		painter->setFont(QFont("Arial", B1.x() / 2));
	}
	painter->drawText(D1,"x");
	painter->drawText(D2, "y");
	update();
}

void ViewerWidget::KresliFunkciu(int N, int Hustota, int Rezim){
	double* HodnotyFcie = new double[Hustota+1];		//vytvorim si staticke pole pre hodnoty fcie
	int i;
	double dielik = (N*2) * M_PI / Hustota;
	int zlomok = 22;

	QPen pero;
	pero.setWidth(5);
	pero.setColor("red");
	painter->setPen(pero);

	if (Rezim > 2) {
		for (i = 0; i <= Hustota; i++) {
			HodnotyFcie[i] = qCos((-Hustota * dielik) / 2 + i * dielik);
		}
	}
	else {
		for (i = 0; i <= Hustota; i++) {
			HodnotyFcie[i] = qSin((-Hustota * dielik) / 2 + i * dielik);
		}
	}
	
	if (Rezim % 3 == 0) {
		for (i = 0; i <= Hustota; i++) {
			int x = round(areaSize.width() / zlomok + i * ((areaSize.width() - 2.0*((double)areaSize.width() / zlomok)) / Hustota));
			int y = round((areaSize.height() / 2) - (HodnotyFcie[i] * ((areaSize.height() / 2.0) - ((double)areaSize.height() / zlomok))));
			painter->drawPoint(x,y);
		}
	}
	else if (Rezim % 3 == 1) {
		for (i = 0; i < Hustota; i++) {
			int X1= round(areaSize.width() / zlomok + i * ((areaSize.width() - 2.0*((double)areaSize.width() / zlomok)) / Hustota));
			int Y1= round((areaSize.height() / 2.0) - (HodnotyFcie[i] * ((areaSize.height() / 2.0) - ((double)areaSize.height() / zlomok))));
			int X2= round(areaSize.width() / zlomok + (i+1) * ((areaSize.width() - 2.0*((double)areaSize.width() / zlomok)) / Hustota));
			int Y2= round((areaSize.height() / 2.0) - (HodnotyFcie[i+1] * ((areaSize.height() / 2.0) - ((double)areaSize.height() / zlomok))));

			painter->drawLine(X1, Y1, X2, Y2);
		}
	}
	else {
		for (i = 0; i <= Hustota; i++) {
			int X1 = round(areaSize.width() / zlomok + i * ((areaSize.width() - 2.0*((double)areaSize.width() / zlomok)) / Hustota) - areaSize.width()/(Hustota*2));
			int Sirka1 = round(((areaSize.width() - 2.0*(areaSize.width() / zlomok)) / Hustota));
			int Vyska1 = round(-HodnotyFcie[i] * ((areaSize.height() / 2) - (areaSize.height() / zlomok)));

			QBrush mojbrush("red");
			painter->drawRect(X1, areaSize.height() / 2, Sirka1, Vyska1);
			painter->fillRect(X1, areaSize.height() / 2, Sirka1, Vyska1,mojbrush);
		}
	}

	delete[] HodnotyFcie;
	update();
}


//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}