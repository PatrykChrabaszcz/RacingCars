#ifndef COMPUTERCAR_H
#define COMPUTERCAR_H
#include <abstractcar.h>

class ComputerCar : public AbstractCar
{
public:
  ComputerCar();

  void goStep(Track *parentTrack);
  void updateMovingDirection(Track *parentTrack, Direction direction, bool status);

  void init();

private:
  Cell::Direction movingDirection;

  Cell::Direction lastMovingDirection;
  bool started;
  bool directionChoosed;
  int lastIndexX;
  int lastIndexY;
  bool checkLeft(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction);
  bool checkRight(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction);
  bool checkUp(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction);
  bool checkDown(Track *parentTrack, int actualX, int actualY, Cell::Direction &direction);
};

#endif // COMPUTERCAR_H
