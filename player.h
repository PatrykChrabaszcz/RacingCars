#ifndef PLAYER_H
#define PLAYER_H

#include <abstractcar.h>
#include <list>

#include <QKeyEvent>
///
///Obiekty tej klasy tworzone są przez klasę Game,
///każdy gracz posiada wskaźnik na swój samochód ( sterowany przez komputer bądź przez użytkownika) .
///W przypadku gdy samochodem steruje użytkownik należy zdefiniować klawisze na które reaguje samochód.
///
class Player
{
public:

  enum PlayerType{
    UserPlayer,
    ComputerPlayer,
  };

  Player(AbstractCar *, std::string name);
  ~Player();
  void defineKeys(int keyUp, int keyDown, int keyRight, int keyLeft);
  void keyChanged(int key, bool status);
  AbstractCar *car;
  std::string getName();
private:
  std::string name;
  int keyUp;
  int keyDown;
  int keyRight;
  int keyLeft;
};

#endif // PLAYER_H
