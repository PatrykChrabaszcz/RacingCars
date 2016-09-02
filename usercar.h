#ifndef USERCAR_H
#define USERCAR_H
#include <abstractcar.h>

class UserCar : public AbstractCar
{
public:
  UserCar();
  void updateMovingDirection(Track *parentTrack, Direction direction, bool status);
};

#endif // USERCAR_H
