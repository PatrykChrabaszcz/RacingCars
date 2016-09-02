#include "trackgui.h"
TrackGUI::TrackGUI(QWidget *parent) :
  QGraphicsView(parent)
{
  setFrameShape(QGraphicsView::NoFrame);
  editCellType = Cell::Type(0);
  scene = new QGraphicsScene(this);
  displayedTrack = NULL;
  setScene(scene);
}

void TrackGUI::setTrack(Track *displayedTrack){
  this->displayedTrack = displayedTrack;
}

void TrackGUI::mousePressEvent(QMouseEvent *event){
  QPointF mousePoint = mapToScene(event->x(),event->y());
  mousePoint = QPointF(mousePoint.x()/sizeFactorX,mousePoint.y()/sizeFactorY);

  QPoint cellIndex = QPoint(mousePoint.x()/displayedTrack->getCellSize(),mousePoint.y()/displayedTrack->getCellSize());

  if(cellIndex.x()<0 || cellIndex.y()<0)
    return;
  if(cellIndex.x()>displayedTrack->getNumberOfCellsH() || cellIndex.y() > displayedTrack->getNumberOfCellsV())
    return;

  if(event->button() == Qt::LeftButton){
    //Edit cell
    displayedTrack->editCell(cellIndex.x(), cellIndex.y(), editCellType);
  }
  else if(event->button() == Qt::RightButton){
      displayedTrack->rotateCell(cellIndex.x(),cellIndex.y());
  }

  //Update scene display
  setMyScene();
}

void TrackGUI::changeEditCellType(Cell::Type type){
  editCellType = type;
}

void TrackGUI::setMyScene(){
  scene->clear();
  carVector.clear();
  int size = displayedTrack->getCellSize();

  sizeFactorX = width()/(displayedTrack->getNumberOfCellsH()*size);
  sizeFactorY = height()/(displayedTrack->getNumberOfCellsV()*size);
  QBrush brush;
  QImage brushImage;

  foreach (Cell *cell, displayedTrack->cellVector) {
    int x = cell->getIndexX() * size * sizeFactorX;
    int y = cell->getIndexY() * size * sizeFactorY;

    switch(cell->getType()){
      case Cell::RoadStraight :{

        brushImage = QImage(":/Tekstury/roadTextureSmallStraight.jpg");
        break;
        }
      case Cell::RoadCorner :{
        brushImage = QImage(":/Tekstury/roadTextureSmallCorner.jpg");
        break;
      }
      case Cell::StartPosition:{
        brushImage = QImage(":/Tekstury/startTextureSmall.jpg");
        break;
      }
      case Cell::BeforeStartPosition :{
        brushImage = QImage(":/Tekstury/beforeStartTextureSmall.jpg");
        break;
      }
      case Cell::Sand :{
        brushImage = QImage(":/Tekstury/sandTextureSmallOne.jpg");
        break;
      }
      case Cell::Stone:{
        brushImage = QImage(":/Tekstury/rockTextureSmallOne.jpg");
        break;
      }
      case Cell::Grass :{
        brushImage = QImage(":/Tekstury/grassTextureSmallOne.jpg");
        break;
      }case Cell::Water :{
        brushImage = QImage(":/Tekstury/waterTextureSmallOne.jpg");
        break;
      }
    }
    QTransform myTransform;
    myTransform.rotate(cell->getPointingDirection());
    brushImage = brushImage.scaled(QSize(size*sizeFactorX,size*sizeFactorY));
    brushImage = brushImage.transformed(myTransform);
    brush = QBrush(brushImage);

    this->scene->addRect(x,y,size*sizeFactorX,size*sizeFactorY,QPen(Qt::black),brush);
  }
  for(unsigned int i=0; i<displayedTrack->carVector.size(); i++){
    switch(i+1){
      case AbstractCar::Blue:{
        brushImage = QImage(":/Tekstury/blueCar.png");
        break;
      }
      case AbstractCar::Red:{
        brushImage = QImage(":/Tekstury/redCar.png");
        break;
      }
      case AbstractCar::Green:{
        brushImage = QImage(":/Tekstury/greenCar.png");
        break;
      }
      case AbstractCar::Yellow:{
        brushImage = QImage(":/Tekstury/yellowCar.png");
        break;
      }
      default:{
        assert(false);
        break;
      }
    }
    brushImage = brushImage.scaled(QSize(displayedTrack->carVector[i]->getWidth()*sizeFactorX,
                                       displayedTrack->carVector[i]->getHeight()*sizeFactorY),
                                       Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    brush = QBrush(brushImage);

    QGraphicsRectItem *actualCar = scene->addRect(0,0,
                                                    displayedTrack->carVector[i]->getWidth()*sizeFactorX,
                                                    displayedTrack->carVector[i]->getHeight()*sizeFactorY,
                                                    QPen(Qt::transparent), brush);
    actualCar->setTransformOriginPoint(displayedTrack->carVector[i]->getWidth()/2*sizeFactorX,
                                       displayedTrack->carVector[i]->getHeight()/2*sizeFactorY);

    actualCar->setRotation(displayedTrack->carVector[i]->getAngle());

    actualCar->setPos((displayedTrack->carVector[i]->getPositionX() - displayedTrack->carVector[i]->getWidth()/2)*sizeFactorX,
                      (displayedTrack->carVector[i]->getPositionY() - displayedTrack->carVector[i]->getHeight()/2)*sizeFactorY);
    carVector.push_back(actualCar);
  }
  this->setScene(scene);
}

void TrackGUI::paintEvent(QPaintEvent *event){
  if(displayedTrack == NULL)
    return;

  for(unsigned int i=0; i<carVector.size(); i++){
    QGraphicsRectItem *car = carVector[i];
    car->setRotation(displayedTrack->carVector[i]->getAngle());
    car->setPos((displayedTrack->carVector[i]->getPositionX() - displayedTrack->carVector[i]->getWidth()/2)*sizeFactorX,
                (displayedTrack->carVector[i]->getPositionY() - displayedTrack->carVector[i]->getHeight()/2)*sizeFactorY);
  }
  QGraphicsView::paintEvent(event);
}
