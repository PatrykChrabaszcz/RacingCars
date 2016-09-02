#include "track.h"
Track::Track(int numberOfCellsH, int numberOfCellsV, int cellSize)
{
  this->numberOfCellsH = numberOfCellsH;
  this->numberOfCellsV = numberOfCellsV;
  this->cellSize = cellSize;

  for(int y=0;y<numberOfCellsV;y++)
    for(int x=0; x<numberOfCellsH; x++){
      cellVector.push_back(new Cell(x,y,Cell::Up, Cell::Grass));
      }
}
Track::~Track(){
  foreach (Cell *cell, cellVector) {
    delete cell;
  }
}

bool Track::checkRectanglesOverlap(QPointF *firstRect, QPointF *secondRect)
{
  
  //For each edge project vertices from both rectangles and check if they intersect
  for(int i=0;i<4;i++){
    QPointF firstRectVertexOne = firstRect[i];
    QPointF firstRectVertexTwo;
    if(i==3)
      firstRectVertexTwo = firstRect[0];
    else
      firstRectVertexTwo = firstRect[i+1];
    
    QPointF baseVector = firstRectVertexTwo - firstRectVertexOne;
    QPointF vector;
    
    float minOne;
    float maxOne;
    float minTwo;
    float maxTwo;
    
    //For given edge project firstRectangle vertices, find min and max
    for(int j=0;j<4;j++){
      if(i==j)
        continue;
      QPointF actualVertex = firstRect[j];
      
      vector = actualVertex - firstRectVertexOne;
      if(j == 0){
        minOne = QPointF::dotProduct(baseVector,vector);
        maxOne = minOne;
      }
      else{
        float result = QPointF::dotProduct(baseVector,vector);
        if(result>maxOne)
          maxOne = result;
        else if(result<minOne)
          minOne = result;
      } 
    }

    //For a given edge projet secondRectangle vertices, find min and max
    for(int j=0;j<4;j++){
      QPointF actualVertex = secondRect[j];

      vector = actualVertex - firstRectVertexOne;
      if(j == 0){
        minTwo = QPointF::dotProduct(baseVector,vector);
        maxTwo = minTwo;
      }
      else{
        float result = QPointF::dotProduct(baseVector,vector);
        if(result>maxTwo)
          maxTwo = result;
        else if(result<minTwo)
          minTwo = result;
      }
    }
    if(maxTwo<minOne || minTwo>maxOne)
      return false;
  }

  for(int i=0;i<4;i++){
    QPointF secondRectVertexOne = secondRect[i];
    QPointF secondRectVertexTwo;
    if(i==3)
      secondRectVertexTwo = secondRect[0];
    else
      secondRectVertexTwo = secondRect[i+1];

    QPointF baseVector = secondRectVertexTwo - secondRectVertexOne;
    QPointF vector;

    float minOne;
    float maxOne;
    float minTwo;
    float maxTwo;

    //For given edge project firstRectangle vertices, find min and max
    for(int j=0;j<4;j++){
      if(i==j)
        continue;
      QPointF actualVertex = secondRect[j];

      vector = actualVertex - secondRectVertexOne;
      if(j == 0){
        minOne = QPointF::dotProduct(baseVector,vector);
        maxOne = minOne;
      }
      else{
        float result = QPointF::dotProduct(baseVector,vector);
        if(result>maxOne)
          maxOne = result;
        else if(result<minOne)
          minOne = result;
      }
    }

    //For a given edge projet secondRectangle vertices, find min and max
    for(int j=0;j<4;j++){
      QPointF actualVertex = firstRect[j];

      vector = actualVertex - secondRectVertexOne;
      if(j == 0){
        minTwo = QPointF::dotProduct(baseVector,vector);
        maxTwo = minTwo;
      }
      else{
        float result = QPointF::dotProduct(baseVector,vector);
        if(result>maxTwo)
          maxTwo = result;
        else if(result<minTwo)
          minTwo = result;
      }
    }
    if(maxTwo<minOne || minTwo>maxOne)
      return false;
  }
  return true;

}
void Track::loadTrack(std::string filePath){

  QFile file (QString::fromStdString(filePath));
  file.open(QIODevice::ReadOnly);
  QDataStream myStream(&file);

  assert(file.isOpen());

  int numberOfCellsH;
  int numberOfCellsV;
  Cell::Type type;
  Cell::Direction direction;
  int tempInt;

  cellVector.clear();

  myStream >> numberOfCellsH;
  myStream >> numberOfCellsV;
  for(int y=0;y<numberOfCellsV;y++){
    for(int x=0;x<numberOfCellsH;x++){

      myStream >> tempInt;
      type = Cell::Type(tempInt);
      myStream>>tempInt;
      direction = Cell::Direction(tempInt);

      cellVector.push_back(new Cell(x,y,direction, type));
    }
  }
}

void Track::saveTrack(std::string filePath){
  QFile file(QString::fromStdString(filePath));
  file.open(QIODevice::WriteOnly);
  QDataStream myStream(&file);

  assert(file.isOpen());

  myStream << numberOfCellsH;
  myStream << numberOfCellsV;
  for(int y=0;y<numberOfCellsV;y++){
    for(int x=0;x<numberOfCellsH;x++){
      myStream << cellVector.at(x+numberOfCellsH*y)->getType();
      myStream << cellVector.at(x+numberOfCellsH*y)->getPointingDirection();
    }
  }
}

int Track::getCellSize(){
  return cellSize;
}

int Track::getNumberOfCellsH(){
  return numberOfCellsH;
}

int Track::getNumberOfCellsV(){
  return numberOfCellsV;
}

float Track::getCellTerrainPenalty(int x, int y){

  return cellVector.at(x+numberOfCellsH*y)->getTerrainPenalty();

}
void Track::editCell(int x, int y, Cell::Type type){
  assert(x>=0);
  assert(x<numberOfCellsH);
  assert(y>=0);
  assert(y<numberOfCellsV);
  cellVector.at(x+numberOfCellsH*y)->setType(type);
}

void Track::rotateCell(int x, int y){
  assert(x>=0);
  assert(x<numberOfCellsH);
  assert(y>=0);
  assert(y<numberOfCellsV);

  cellVector.at(x+numberOfCellsH*y)->rotate();
}

bool Track::checkIfReady(){
  //External cells cannot be roads
  for(int i=0;i<getNumberOfCellsH();i++){
    if(Cell::checkIfCellIsRoad(getCellType(i,0)) ||
       Cell::checkIfCellIsRoad(getCellType(i,numberOfCellsV-1))){
        assert(false);
      return false;
    }
  }
  for(int i=0;i<getNumberOfCellsV();i++){
    if(Cell::checkIfCellIsRoad(getCellType(0,i)) ||
       Cell::checkIfCellIsRoad(getCellType(numberOfCellsH-1,i))){
        assert(false);
      return false;
    }
  }
  //Each road Cell must be connected to exacly 2 road cells
  for(int x=1;x<getNumberOfCellsH()-1;x++){
    for(int y=1; y<getNumberOfCellsH()-1;y++){
      int counter = 0;
      if(Cell::checkIfCellIsRoad(getCellType(x,y))){
        if(Cell::checkIfCellIsRoad(getCellType(x+1,y)))
          counter++;
        if(Cell::checkIfCellIsRoad(getCellType(x-1,y)))
          counter++;
        if(Cell::checkIfCellIsRoad(getCellType(x,y+1)))
          counter++;
        if(Cell::checkIfCellIsRoad(getCellType(x,y-1)))
          counter++;
        if(counter != 2){
          return false;
        }
      }
    }
  }
  //There must be only one Start and BeforeStart Cell

  int startCellCounter = 0;
  foreach(Cell *cell, cellVector){
    if(cell->getType() == Cell::BeforeStartPosition){
      startCellCounter++;
      beforeStartCellIndex = QPoint(cell->getIndexX(),cell->getIndexY());
      }
    if(cell->getType() == Cell::StartPosition){
      startCellCounter++;
      startCellIndex = QPoint(cell->getIndexX(),cell->getIndexY());
    }
  }
  if(startCellCounter != 2){
    return false;
  }
  //Start and beforeStart cells must be next to each other
  if((abs(beforeStartCellIndex.x() - startCellIndex.x()) == 1 &&
      abs(beforeStartCellIndex.y() - startCellIndex.y()) == 0) ||
     (abs(beforeStartCellIndex.x() - startCellIndex.x()) == 0 &&
      abs(beforeStartCellIndex.y() - startCellIndex.y()) == 1) ){
    ;//Good
  }
  else{
    return false;
  }

  if(carVector.empty())
    return false;

  return true;
}
void Track::setCarsStartPosition(){

  Cell::Direction direction;
  if(beforeStartCellIndex.x()>startCellIndex.x())
    direction = Cell::Left;
  if(beforeStartCellIndex.x()<startCellIndex.x())
    direction = Cell::Right;
  if(beforeStartCellIndex.y()>startCellIndex.y())
    direction = Cell::Up;
  if(beforeStartCellIndex.y()<startCellIndex.y())
    direction = Cell::Down;

  //Sets Car Position and Rotation

    //First Car if exists
  if(carVector.size()>=1){
   carVector[0]->setPosition(beforeStartCellIndex.x() * cellSize + carVector[0]->getHeight()/2,
                                  beforeStartCellIndex.y() * cellSize + carVector[0]->getHeight()/2);
   carVector[0]->setAngle((int)direction);
  }

    //Second Car if exists
  if(carVector.size()>=2){
   carVector[1]->setPosition(beforeStartCellIndex.x() * cellSize + cellSize - carVector[1]->getHeight()/2,
                                  beforeStartCellIndex.y() * cellSize + carVector[1]->getHeight()/2);
   carVector[1]->setAngle((int)direction);
  }
  //Third Car if exists
  if(carVector.size()>=3){
    carVector[2]->setPosition(beforeStartCellIndex.x() * cellSize+cellSize - carVector[2]->getHeight()/2,
                                    beforeStartCellIndex.y() * cellSize+cellSize - carVector[2]->getHeight()/2);
    carVector[2]->setAngle((int)direction);
  }
  //Fourth Car if exists
  if(carVector.size()>=4){
    carVector[3]->setPosition(beforeStartCellIndex.x() * cellSize + carVector[3]->getHeight()/2,
                                   beforeStartCellIndex.y() * cellSize+cellSize - carVector[3]->getHeight()/2);
    carVector[3]->setAngle((int)direction);
  }
}

void Track::addCar(AbstractCar *newCar){
  carVector.push_back(newCar);
}

void Track::clearCarVector(){
  carVector.clear();
}

Cell::Type Track::getCellType(int x, int y){
  return cellVector.at(x+numberOfCellsH*y)->getType();
}


