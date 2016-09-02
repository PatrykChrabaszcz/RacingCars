#include "usercar.h"
UserCar::UserCar()
{
}

void UserCar::updateMovingDirection(Track *parentTrack, Direction direction, bool status){
  switch (direction) {
    case AbstractCar::Forward:
      forward = status;
      break;
    case AbstractCar::Backward:
      backward = status;
      break;
    case AbstractCar::Right:
      right = status;
      break;
    case AbstractCar::Left:
      left = status;
      break;
    default:
      break;
    }
}
