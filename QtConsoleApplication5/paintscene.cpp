#include "paintscene.h"
#include "Ellips.h"
#include "triangle.h"
#include "square.h"

PaintScene::PaintScene(QObject* parent) : QGraphicsScene(parent)
{

}

PaintScene::~PaintScene()
{
	delete pressedItem;
	delete tempFigure;
	delete targedItem;
	delete Line;
}

int PaintScene::typeCommand() const
{
	return m_typeCommand;
}

void PaintScene::setTypeCommand(const int type)
{
	m_typeCommand = type;
	if (type == 6)
	{
		savetojson();
	}
	else if (type == 7)
	{
		loadtojson();
	}
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton && event->buttons() != Qt::RightButton)
	{
		switch (m_typeCommand) {
		case Move: {
			if (pressedItem != nullptr && flagescape == false && flagrightclick == false) {
				if (pressedItem->lines.isEmpty() == false) {
					pressedItem->adjustLines(); //подгоняем линии под итем
					pressedItem->setPos(event->scenePos() + m_shiftMouseCoords);
					pressedItem->redrawLines();
				}
				else {
					pressedItem->setPos(event->scenePos() + m_shiftMouseCoords); //если без линий-просто перетаскиваем
				}
			}
			break;
		}
		case Link:
		{
			if (pressedItem != nullptr && flagescape == false && flagrightclick == false) {
				p2 = event->scenePos();
				targedItem = dynamic_cast<Figure*>(itemAt(event->scenePos().x(), event->scenePos().y(), QTransform()));
				if (targedItem != nullptr && targedItem != pressedItem && pressedItem != nullptr) {
					Line->setLine(QLineF(p1, targedItem->boundingRect().center() + targedItem->scenePos()));
				}
				else
					Line->setLine(QLineF(p1, p2));
			}
			break;
		}
		case Del:
		{
			break;
		}
		case Save:
		{
			break;
		}
		case Load:
		{
			break;
		}
		default: {
			if (tempFigure != nullptr && flagescape == false && flagrightclick == false)
			{
				tempFigure->setEndPoint(event->scenePos());
				this->update(QRectF(0, 0, this->width(), this->height()));
				break;
			}

		}
		}
		if (flagrightclick == false && event->buttons() == Qt::RightButton)
		{
			flagrightclick = true;
		}
	}
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton ) {
		switch (m_typeCommand) {
		case SquareType: {
			tempFigure = nullptr;
			tempFigure = new Square();
			tempFigure->setStartPoint(event->scenePos());
			tempFigure->setEndPoint(event->scenePos());
			addItem(tempFigure);
			break;
		}
		case EllipsType: {
			tempFigure = new Ellips();
			tempFigure->setStartPoint(event->scenePos());
			tempFigure->setEndPoint(event->scenePos());
			addItem(tempFigure);
			break;
		}
		case TriangleType: {
			tempFigure = new Triangle();
			tempFigure->setStartPoint(event->scenePos());
			tempFigure->setEndPoint(event->scenePos());
			addItem(tempFigure);
			break;
		}
		case Move: {
			pressedItem = dynamic_cast<Figure*>(itemAt(event->scenePos().x(), event->scenePos().y(), QTransform()));
			if (pressedItem != nullptr) {
				pressedItem->setCursor(QCursor(Qt::ClosedHandCursor));
				for (auto& item : saveptrfigure)
				{
					if (item == pressedItem)
					{
						saveptrfigure.removeAll(item);
						break;
					}
				}
				m_shiftMouseCoords = pressedItem->pos() - event->scenePos();
			}
			break;
		}
		case Link: {
			pressedItem = dynamic_cast<Figure*>(itemAt(event->scenePos().x(), event->scenePos().y(), QTransform()));
			if (pressedItem != nullptr) {
				m_shiftMouseCoords = pressedItem->scenePos() - event->scenePos();
				p1 = pressedItem->boundingRect().center() + pressedItem->pos();
				Line = new QGraphicsLineItem;
				Line->setLine(QLineF(p1, event->scenePos()));
				addItem(Line);
			}
			break;
		}
		case Del:
		{
			pressedItem = dynamic_cast<Figure*>(itemAt(event->scenePos().x(), event->scenePos().y(), QTransform()));
			if (pressedItem != nullptr)
			{
				for (auto& item : saveptrfigure)
				{
					if (item == pressedItem)
					{
						saveptrfigure.removeAll(item);
						break;
					}
				}
				deleteFigure(pressedItem);
			}
			break;
		}
		case Save:
		{
			break;
		}
		case Load:
		{
			break;
		}
		default: {
			tempFigure = new Square();
			tempFigure->setEndPoint(event->scenePos());
			tempFigure->setStartPoint(event->scenePos());
			addItem(tempFigure);
			break;
		}
		}
	}
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
		switch (m_typeCommand) {
		case Link: {
			if (pressedItem != nullptr)
			{
				delete Line;
				if (targedItem != nullptr && targedItem != pressedItem && (Line = pressedItem->lineTo(targedItem)) != nullptr)
				{
					Line->setZValue(-1);
					addItem(Line);
				}
			}
			pressedItem = nullptr;
			targedItem = nullptr;
			break;
		}
		case Move:
		{
			if (pressedItem != nullptr) {
				pressedItem->setCursor(QCursor(Qt::ArrowCursor));
				savePtr(pressedItem);
				pressedItem = nullptr;
			}
			break;
		}
		case Del:
		{
			break;
		}
		case Save:
		{
			break;
		}
		case Load:
		{
			break;
		}
		default:
		{
			if (flagescape == false && flagrightclick == false) {
				savePtr(tempFigure);
			}
			else 
			{
				removeItem(tempFigure);
			}
			tempFigure = nullptr;
			break;
		}
		}
	flagescape = false;
	flagrightclick = false;
}

void PaintScene::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Escape) {
		flagescape = true;
	}
}

void PaintScene::writetolist()
{
	count = 1;
	if (saveptrfigure.size() != 0)
	{
		for (const auto& figure : saveptrfigure)
		{
			FigureData list;
			LineData line;
			if (figure != nullptr) {
				list.id = count;
				Square* temp = dynamic_cast<Square*>(figure);
				if (temp != nullptr)
				{
					list.type = 0;
				}
				else
				{
					Ellips* temp = dynamic_cast<Ellips*>(figure);
					if (temp != nullptr)
					{
						list.type = 1;
					}
					else
					{
						list.type = 2;
					}
				}
				if (figure->startPoint().x() > figure->endPoint().x())
				{
					list.cordx = figure->endPoint().x() - figure->startPoint().x();
					list.cordy = figure->endPoint().y() - figure->startPoint().y();
				}
				else
				{
					list.cordx = qAbs(figure->startPoint().x() - figure->endPoint().x());
					list.cordy = qAbs(figure->startPoint().y() - figure->endPoint().y());
				}
				list.centrx = figure->boundingRect().center().x() + figure->pos().x();
				list.centry = figure->boundingRect().center().y() + figure->pos().y();
				savelist.push_back(list);

				if (figure->lines.size() != 0)
				{
					for (const auto& figureline : figure->lines)
					{
						line.id1 = count;
						int tempcount = 1;
						for (const auto& figuretemp : saveptrfigure)
						{
							if (figuretemp != figure && tempcount > count)
							{
								for (auto& templine : figuretemp->lines)
								{
									if (templine == figureline)
									{
										line.id2 = tempcount;
										savelistline.push_back(line);
										break;
									}
								}
							}
							tempcount++;
						}
					}
				}
				count++;
			}
		}
	}
}

void PaintScene::savePtr(Figure* figure)
{
	saveptrfigure.push_back(figure);
}

void PaintScene::figuretojson()
{
	QJsonDocument doc;
	QJsonArray docarr;
	QVariantMap map;
	QJsonObject jobj;
	QJsonArray doctowrite;
	GlobalPath = QFileDialog::getOpenFileName(nullptr, "", "*.json");
	file.setFileName(GlobalPath);
	file.open(QIODevice::WriteOnly);
	for (auto& item : savelist)
	{
		map.insert("id", item.id);
		map.insert("type", item.type);
		map.insert("cordx", item.cordx);
		map.insert("cordy", item.cordy);
		map.insert("centrx", item.centrx);
		map.insert("centry", item.centry);
		jobj = QJsonObject::fromVariantMap(map);
		doctowrite.append(jobj);
	}
	doc.setArray(doctowrite);
	file.write("{\"Figure\":" + doc.toJson());



	savelist.clear();
}

void PaintScene::linetojson()
{
	if (savelistline.size() != 0)
	{
		file.write(",");
		QJsonDocument doc;
		QJsonArray docarr;
		QVariantMap map;
		QJsonObject jobj;
		QJsonArray doctowrite;
		for (auto& item : savelistline)
		{
			map.insert("id1", item.id1);
			map.insert("id2", item.id2);
			jobj = QJsonObject::fromVariantMap(map);
			doctowrite.append(jobj);
		}
		doc.setArray(doctowrite);
		file.write("\"Line\":" + doc.toJson());
		savelistline.clear();
	}
	file.write("}");
	file.close();
}

void PaintScene::savetojson()
{
	writetolist();
	figuretojson();
	linetojson();
}

void PaintScene::clearScene()
{
	for (auto& figure : saveptrfigure)
	{
		deleteFigure(figure);
	}
	saveptrfigure.clear();
}
void PaintScene::figurefromjson()
{
	QJsonDocument doc;
	QJsonArray docarr;
	GlobalPath = QFileDialog::getOpenFileName(nullptr, "", "*.json");
	file.setFileName(GlobalPath);
	if (file.open(QIODevice::ReadOnly)) {
		clearScene();
		doc = QJsonDocument::fromJson(QByteArray(file.readAll()));
		docarr = QJsonValue(doc.object().value("Figure")).toArray();
		FigureData line;
		for (int i = 0; i < docarr.count(); i++)
		{
			line.id = docarr.at(i).toObject().value("id").toInt();
			line.type = docarr.at(i).toObject().value("type").toInt();
			line.cordx = docarr.at(i).toObject().value("cordx").toInt();
			line.cordy = docarr.at(i).toObject().value("cordy").toInt();
			line.centrx = docarr.at(i).toObject().value("centrx").toInt();
			line.centry = docarr.at(i).toObject().value("centry").toInt();
			savelist.push_back(line);
		}
		file.close();
	}
}

void PaintScene::linefromjson()
{
	QJsonDocument doc;
	QJsonArray docarr;
	if (file.open(QIODevice::ReadOnly))
	{
		doc = QJsonDocument::fromJson(QByteArray(file.readAll()));
			docarr = QJsonValue(doc.object().value("Line")).toArray();
			LineData line;
			for (int i = 0; i < docarr.count(); i++)
			{
				line.id1 = docarr.at(i).toObject().value("id1").toInt();
					line.id2 = docarr.at(i).toObject().value("id2").toInt();
					savelistline.push_back(line);
			}
		file.close();
	}
}

void PaintScene::paintloaddata()
{
	for (auto& item : savelist)
	{
		QPointF startpoint;
		QPointF endpoint;

			 startpoint = { qreal(item.centrx - (item.cordx / 2)) , qreal(item.centry - (item.cordy / 2)) };
			 endpoint = { qreal(item.centrx + (item.cordx / 2)), qreal(item.centry + (item.cordy / 2)) };
		if (item.type == 0)
		{
			tempFigure = new Square();
		}
		else if (item.type == 1)
		{
			tempFigure = new Ellips();
		}
		else
		{
			tempFigure = new Triangle();
		}
		tempFigure->setStartPoint(startpoint);
		tempFigure->setEndPoint(endpoint);
		saveptrfigure.push_back(tempFigure);
		addItem(tempFigure);
	}
	for (auto& item : savelistline)
	{
		for (auto& temp : savelist)
		{
			if (item.id1 == temp.id)
			{
				for (auto& figure : saveptrfigure)
				{
					if (figure->boundingRect().center() == QPointF(qreal(temp.centrx), qreal(temp.centry)))
					{
						pressedItem = figure;
						break;
					}
				}
			}
		}
		for (auto& temp : savelist)
		{
			if (item.id2 == temp.id)
			{
				for (auto& figure : saveptrfigure)
				{
					if (figure != pressedItem && figure->boundingRect().center() == QPointF(qreal(temp.centrx), qreal(temp.centry)))
					{
						targedItem = figure;
						break;
					}
				}
			}
		}
		if (targedItem != nullptr && targedItem != pressedItem && (Line = pressedItem->lineTo(targedItem)) != nullptr)
		{
			Line->setZValue(-1);
			addItem(Line);
		}
	}
	tempFigure = nullptr;
	pressedItem = nullptr;
	targedItem = nullptr;
	savelistline.clear();
	savelist.clear();
}


void PaintScene::loadtojson()
{
	figurefromjson();
	linefromjson();
	paintloaddata();
}

void PaintScene::deleteFigure(Figure* figure)
{
	if (figure->lines.size() > 0)
	{
		for (auto& line : figure->lines)
		{

			tempFigure = dynamic_cast<Figure*>(itemAt(line->line().p2(), QTransform()));
			if (tempFigure != nullptr && tempFigure != figure) {
				tempFigure->lines.removeAll(line);
				tempFigure = nullptr;
				delete line;
			}
			else
			{
				tempFigure = dynamic_cast<Figure*>(itemAt(line->line().p1(), QTransform()));
				if (tempFigure != nullptr)
				{
					tempFigure->lines.removeAll(line);
					tempFigure = nullptr;
					delete line;
				}
			}
		}
	}
	delete figure;
}
