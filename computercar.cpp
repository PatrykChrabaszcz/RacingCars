#include "computercar.h"
ComputerCar::ComputerCar()
{
  started = false;
}
void ComputerCar::goStep(Track *parentTrack){
  updateMovingDirection(parentTrack,AbstractCar::Forward,false);
  AbstractCar::goStep(parentTrack);
}
void ComputerCar::init(){
  started = false;
  AbstractCar::init();
}

void ComputerCar::updateMovingDirection(Track *parentTrack, Direction direction, bool status){
  if(!started){
    started = true;
    movingDirection = Cell::Direction(angle);
  }
  directionChoosed = false;
  int indexX = positionX/parentTrack->getCellSize();
  int indexY = positionY/parentTrack->getCellSize();

  if(isOnRoad){
    if(movingDirection == Cell::Down){
      if(!directionChoosed){
        directionChoosed = checkDown(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkRight(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkLeft(parentTrack,indexX,indexY, movingDirection);
      }
    }
    else if(movingDirection == Cell::Right){
      if(!directionChoosed){
        directionChoosed = checkRight(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkDown(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkUp(parentTrack,indexX,indexY, movingDirection);
      }
    }
    else if(movingDirection == Cell::Up){
      if(!directionChoosed){
        directionChoosed = checkUp(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkRight(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkLeft(parentTrack,indexX,indexY, movingDirection);
      }
    }
    else if (movingDirection == Cell::Left){
      if(!directionChoosed){
        directionChoosed = checkLeft(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkUp(parentTrack,indexX,indexY, movingDirection);
      }
      if(!directionChoosed){
        directionChoosed = checkDown(parentTrack,indexX,indexY, movingDirection);
      }
    }
  }

  //Computer car is always moving forward
  forward = true;

  //Find right or left turn
  if(movingDirection == angle)
    return;

  float tempAngle = angle;
  if(tempAngle<movingDirection)
    tempAngle += 360;
  float difference = tempAngle-movingDirection;


  if(difference>0 && difference<180){
    right = false;
    left = true;
   }

  if(difference<360 && difference>=180){
    left = false;
    right = true;
  }
  if(difference < 10 && difference > -10){
      setAngle(movingDirection);
    left = false;
    right = false;
  }
}

bool ComputerCar::checkLeft(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction){
  if(actualX>0){
    if(Cell::checkIfCellIsRoad(parentTrack->getCellType(actualX-1,actualY))){
      direction = Cell::Left;
      return true;
    }
  }
  return false;
}

bool ComputerCar::checkRight(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction){
  if(actualX+1<parentTrack->getNumberOfCellsH()){
    if(Cell::checkIfCellIsRoad(parentTrack->getCellType(actualX+1,actualY))){
      direction = Cell::Right;
      return true;
    }
  }
  return false;
}

bool ComputerCar::checkUp(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction){
  if(actualY>0){
    if(Cell::checkIfCellIsRoad(parentTrack->getCellType(actualX,actualY-1))){
      direction = Cell::Up;
      return true;
    }
  }
}

bool ComputerCar::checkDown(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction){
  if(actualY+1<parentTrack->getNumberOfCellsV()){
    if(Cell::checkIfCellIsRoad(parentTrack->getCellType(actualX,actualY+1))){
        direction = Cell::Down;
      return true;
    }
  }
  return false;
}
