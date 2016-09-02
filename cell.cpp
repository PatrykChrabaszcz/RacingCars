#include "cell.h"

Cell::Cell(int indexX, int indexY, Cell::Direction pointingDirection, Type type)
{
  this->indexX = indexX;
  this->indexY = indexY;
  this->pointingDirection = pointingDirection;
  this->type = type;
}

void Cell::setType(Cell::Type type){
  this->type = type;
}
void Cell::rotate(){
  int angle = (int)pointingDirection;
  angle += 90;
  angle %= 360;
  pointingDirection = Cell::Direction(angle);
}

int Cell::getIndexX(){
  return indexX;
}
int Cell::getIndexY(){
  return indexY;
}

float Cell::getTerrainPenalty(){
  if(Cell::checkIfCellIsRoad(type)){
      return 1;
    }
  switch (type) {
    case Cell::Grass:
      return 0.2;
    case Cell::Stone:
      return 0.5;
    case Cell::Water:
      return 0.05;
    case Cell::Sand:
      return 0.1;
    default:
      assert(false);
      break;
  }
}
Cell::Type Cell::getType(){
  return type;
}
Cell::Direction Cell::getPointingDirection(){
  return pointingDirection;
}

//Static
bool Cell::checkIfCellIsRoad(Cell::Type type)
{
  if(type == Cell::RoadCorner )
    return true;
  if(type == Cell::RoadStraight)
    return true;
  if(type == Cell::BeforeStartPosition)
    return true;
  if(type == Cell::StartPosition)
    return true;

  return false;
}
