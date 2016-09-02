#include <abstractcar.h>
AbstractCar::AbstractCar(int width, int height){
  maxSpeed = AbstractCar::BestSpeed;
  minSpeed = (-1)*AbstractCar::BestSpeed;
  acceleration = AbstractCar::BestAcceleration;
  decceleration = AbstractCar::BestDecceleration;
  idleDecceleration = AbstractCar::BestDecceleration;
  turningSpeed = AbstractCar::BestTurningSpeed;

  init();
  this->width = width;
  this->height = height;
}
AbstractCar::~AbstractCar(){

}
void AbstractCar:: init(){
  laps = -1;
  actualSpeed = 0;
  lastCell = Cell::BeforeStartPosition;
  isOnRoad = true;
  isCenterOnStart = true;
  forward = false;
  backward = false;
  right = false;
  left = false;
}

void AbstractCar::changeParameters(float maxSpeed,
                                   float minSpeed,
                                   float acceleration,
                                   float decceleration,
                                   float idleDecceleration,
                                   float health,
                                   float turningSpeed){
  if(maxSpeed>=minSpeed){
    this->maxSpeed = maxSpeed;
    this->minSpeed = minSpeed;
  }
  this->acceleration = acceleration;
  this->decceleration = decceleration;
  this->idleDecceleration = idleDecceleration;
  this->health = health;
  this->turningSpeed = turningSpeed;

}

int AbstractCar::getLaps(){
  return laps;
}

void AbstractCar::goStep(Track *parentTrack)
{
  //Moving disabled when health goes below 0
  if(health<0)
    return;

  if(forward){
    if(actualSpeed>0)
      actualSpeed += acceleration;
    else
      actualSpeed += decceleration;
  }
  else if(backward){
    if(actualSpeed>0)
      actualSpeed -= decceleration;
    else
      actualSpeed -= acceleration;
  }
  else{
    if(actualSpeed>0){
      actualSpeed -= idleDecceleration;
      if(actualSpeed<0)
        actualSpeed = 0;
    }
    else{
      actualSpeed +=idleDecceleration;
      if(actualSpeed>0)
        actualSpeed = 0;
    }
  }

  if(actualSpeed>maxSpeed)
    actualSpeed = maxSpeed;
  if(actualSpeed<minSpeed)
    actualSpeed = minSpeed;

  float lastAngle = angle;
  float difference = actualSpeed * turningSpeed;
  if(difference >turningSpeed){
    difference = turningSpeed;
  }
  if(difference < (-1)*turningSpeed){
      difference = (-1)*turningSpeed;
  }
  if(right){
    angle += actualSpeed * turningSpeed;
  }
  else if (left){
    angle -= actualSpeed * turningSpeed;
  }
  if(angle >360)
    angle = angle-360;
  if(angle<0)
    angle = angle+360;


  float lastPosX = positionX;
  float lastPosY = positionY;
  if(isOnRoad)
    setPosition(positionX+actualSpeed*sin(angle*M_PI/180), positionY-actualSpeed*cos(angle*M_PI/180));

  //Check colision after updating position and disable movement if car wants to leave track
  handleCollision(parentTrack);

  if(!isOnRoad){
    actualSpeed = 0;
    setAngle(lastAngle);
    setPosition(lastPosX, lastPosY);
  }

}



void AbstractCar::setPosition(float x, float y){
  this->positionX = x;
  this->positionY = y;
}


void AbstractCar::handleCollision(Track *parentTrack){

  isOnRoad = false;

  //Coordinates relative to carCentre
  // 0--1
  // |  |
  // 3--2
  QPointF carCorners[4];
  getCarCorners(carCorners);

  int cellSize = parentTrack->getCellSize();

  int indexX = this->positionX/cellSize;
  int indexY = this->positionY/cellSize;

  //For countingLaps

  Cell::Type cellType = parentTrack->getCellType(indexX,indexY);
  if(lastCell == Cell::BeforeStartPosition && cellType == Cell::StartPosition){
    laps ++;
  }
  else if(lastCell == Cell::StartPosition && cellType == Cell::BeforeStartPosition){
    laps --;
  }
  lastCell = cellType;

  //Init values
  terrainPenalty = 0;
  int cellsCounter = 0;

  //For all cells next to centre cell check if car intersects
  for(int x=-1;x<=1;x++){
    if(indexX+x>=0 && indexX+x<parentTrack->getNumberOfCellsH())
      for(int y=-1;y<=1;y++){
        if(indexY+y>=0 && indexY+y<parentTrack->getNumberOfCellsV()){
          QPointF cellCorners[4];
          cellCorners[0].setX((indexX + x) * cellSize);
          cellCorners[0].setY((indexY + y) * cellSize);
          cellCorners[1].setX((indexX + x + 1) * cellSize);
          cellCorners[1].setY((indexY + y) * cellSize);
          cellCorners[2].setX((indexX + x + 1) * cellSize);
          cellCorners[2].setY((indexY + y + 1) * cellSize);
          cellCorners[3].setX((indexX + x) * cellSize);
          cellCorners[3].setY((indexY + y + 1) * cellSize);
          if(Track::checkRectanglesOverlap(carCorners,cellCorners)){

              //isCenterOnStart = (cellType == Cell::BeforeStartPosition || cellType == Cell::StartPosition);
            if( Cell::checkIfCellIsRoad(parentTrack->getCellType(indexX+x,indexY+y))){
               isOnRoad = true;
            }
            terrainPenalty += parentTrack->getCellTerrainPenalty(indexX+x, indexY+y);
            cellsCounter++;
          }
        }
      }
  }
  terrainPenalty /= cellsCounter;
  if(actualSpeed > terrainPenalty*maxSpeed)
    actualSpeed = terrainPenalty*maxSpeed;
  if(actualSpeed < terrainPenalty*minSpeed)
    actualSpeed = terrainPenalty*minSpeed;

  //Check collision with cars;
  foreach(AbstractCar *car, parentTrack->carVector){

    if(this == car)
      continue;

    bool isColidingCar;
    do{
      QPointF *otherCarCorners = new QPointF[4];
      car->getCarCorners(otherCarCorners);

      if(Track::checkRectanglesOverlap(carCorners,otherCarCorners)){
        isColidingCar = true;
        QPointF thisCenter = QPointF(getPositionX(),getPositionY());
        QPointF otherCenter = QPointF(car->getPositionX(),car->getPositionY());
        QPointF difference = otherCenter-thisCenter;
        thisCenter -= 0.01*difference;
        otherCenter += 0.01*difference;
        this->setPosition(thisCenter.x(),thisCenter.y());
        car->setPosition(otherCenter.x(),otherCenter.y());
        this->decreaseHealth();
        car->decreaseHealth();
      }
      else
        isColidingCar = false;
    }while(isColidingCar);
  }
}
void AbstractCar::decreaseHealth(){
  health -= 0.2;
}

void AbstractCar::getCarCorners(QPointF *carCorners){

  carCorners[0].setX(-width/2);
  carCorners[0].setY(-height/2);
  carCorners[1].setX(width/2);
  carCorners[1].setY(-height/2);
  carCorners[2].setX(width/2);
  carCorners[2].setY(height/2);
  carCorners[3].setX(-width/2);
  carCorners[3].setY(height/2);

  //After rotation relative to track origin
  for(int i=0; i < 4; i++){
      float x=carCorners[i].x()*cos(angle*M_PI/180) - carCorners[i].y()*sin(angle*M_PI/180);
      float y=carCorners[i].x()*sin(angle*M_PI/180) + carCorners[i].y()*cos(angle*M_PI/180);

    carCorners[i].setX(x);
    carCorners[i].setY(y);
  }

  //After translation by vector (positionX, positionY)
  for(int i=0; i < 4;i++){
    carCorners[i].setX(carCorners[i].x()+positionX);
    carCorners[i].setY(carCorners[i].y()+positionY);
  }
}
