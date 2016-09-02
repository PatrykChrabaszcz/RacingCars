#include "player.h"
Player::Player(AbstractCar * car, std::string name)
{
  this->car = car;
  this->name = name;

  keyUp = 0;
  keyDown = 0;
  keyRight = 0;
  keyLeft = 0;
}

Player::~Player(){
  delete car;
}

std::string Player::getName(){
  return name;
}

void Player::defineKeys(int keyUp, int keyDown, int keyRight, int keyLeft){
  this->keyUp = keyUp;
  this->keyDown = keyDown;
  this->keyRight = keyRight;
  this->keyLeft = keyLeft;
}
void Player::keyChanged(int key, bool status){

  if(key == 0)
    return;

  if(key == keyUp) {
    car->updateMovingDirection(NULL,AbstractCar::Forward,status);
  }
  else if (key == keyDown){
    car->updateMovingDirection(NULL,AbstractCar::Backward,status);
  }
  else if (key == keyRight){
    car->updateMovingDirection(NULL,AbstractCar::Right,status);
  }
  else if (key == keyLeft){
    car->updateMovingDirection(NULL,AbstractCar::Left,status);
  }
}
