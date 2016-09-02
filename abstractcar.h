#ifndef CAR_H
#define CAR_H
#include <stdlib.h>
#include <track.h>
#include <math.h>
///
///Klasa stanowi klasę bazową dla klas UserCar oraz ComputerCar.
///Zawiera parametry samochodu, oraz implementuje dwie najważniejsze
///wywoływane cyklicznie funkcje które uaktualniają pozycję,
///prędkość i inne zmienne obiektu na podstawie tego gdzie samochód się znajduje na trasie.
///void handleCollision(Track *parentTrack);
///virtual void goStep(Track *parentTrack);
///Funkcja updateMovingDirection() jest zaimplementowana w klasach pochodnych.
///W klasie ComputerCar sterowanie samochodu wyznaczane jest na podstawie
///toru (prosty algorytm jazdy w kierunku następnej komórki reprezentującej drogę).
///W klasie UserCar sterowanie jest wyznaczane na podstawie zewnętrznych sygnałów
///( Klawisze wciśnięte przez użytkownika)

#include <QPoint>
class Track;

class AbstractCar
{
public:
  enum carID{
    Blue = 1,
    Red,
    Green,
    Yellow
  };
  enum Direction{
    Forward,
    Backward,
    Left,
    Right,
  };

  AbstractCar(int widht = 3, int height = 4);
  virtual ~AbstractCar();
  virtual void init();

  int getLaps();
  float getWidth() { return width;}
  float getHeight() { return height;}
  float getHealth() { return health;}
  float getPositionX() { return positionX;}
  float getPositionY() { return positionY;}
  float getAngle() { return angle;}
  void getCarCorners(QPointF *carCorners);

  void setPosition(float x, float y);
  void setAngle(float angle) { this->angle = angle;}
  void decreaseHealth();

  void changeParameters(float maxSpeed,
                        float minSpeed,
                        float acceleration,
                        float decceleration,
                        float idleDecceleration,
                        float health,
                        float turningSpeed);
  ///
  ///Checks if car is colliding with other cars, and handle collision, alse check if car is still on Track
  ///it must be done in every game step after updating all cars status
  ///
  void handleCollision(Track *parentTrack);

  ///
  ///Function updating car status every game step
  ///
  virtual void goStep(Track *parentTrack);
  ///
  ///Updating Car movingDirection, if status true selects direction, when false deselects direction
  ///
  virtual void updateMovingDirection(Track *parentTrack, Direction direction, bool status) = 0;

protected:

  Cell::Type lastCell;
  ///
  ///Maximal car speed measured as number of pixels per one game step
  ///
  float maxSpeed;
  float minSpeed;
  ///
  ///Position is changed every game step based on actualSpeed variable
  ///
  float actualSpeed;
  ///
  ///Car acceleration, decceleration defines rate of speed change when pressing key_UP or key_DOWN
  ///
  float acceleration;
  float decceleration;
  float idleDecceleration;
  ///
  ///Defines health of car which is decreased when collision occurs
  ///
  float health;
  ///
  ///Position of car centre on Track, it must be between (0,0) and (trackWidth,trackHeight)
  ///
  float positionX;
  float positionY;
  ///
  ///Size of car, car is defined as simple rectangle
  ///
  float width;
  float height;
  ///
  ///Rotation of car clockwise
  ///
  float angle;

  ///
  ///Speed of rotation in grades per gameStep
  ///
  float turningSpeed;
  ///
  ///Terrain Penalty based on cell types
  ///
  float terrainPenalty;
  ///
  ///Car moving direction, when true car moves that way
  ///
  bool forward;
  bool backward;
  bool right;
  bool left;
  ///
  ///Laps counter
  ///
  int laps;

  bool isOnRoad;
  bool isCenterOnStart;
  bool checkIfIntersectsCell(int indexX, int indexY, int minX, int minY, int maxX, int maxY, int cellSize);

public:

  //Constants
  static constexpr float BestDecceleration = 0.1;
  static constexpr float BestSpeed = 2;
  static constexpr float BestAcceleration= 0.005;
  static constexpr float BestHealth = 1000;
  static constexpr float BestTurningSpeed = 10;
};
#endif // CAR_H
