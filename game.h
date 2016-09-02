#ifndef GAME_H
#define GAME_H
#include <track.h>
#include <player.h>
#include <usercar.h>
#include <computercar.h>
#include <list>
///
///Klasa odpowiada za uruchamianie gry, dodawanie nowych zawodników, pauzowanie gry,
///sprawdzanie warunków końca. Posiada wskaźnik do obiektu typu Track.
///
class Game
{
public:
  Game();
  ~Game();
  ///
  ///Start Game checks if track is present and ready
  ///
  bool startGame();
  void stopGame();
  void pauseGame();
  void resumeGame();
  bool initGame();

  void setTrack (Track *track);
  void setMaxLaps(int laps);
  void createTrack(int numberOfCellsH, int numberOfCellsV, int cellSize);
  bool checkUsers();
  Player* addPlayer(Player::PlayerType type, std::string name, int keyUp, int keyDown, int keyRight, int keyLeft);
  void clearPlayers();
  std::vector<Player *> playerVector;
  Player * checkWinner(int laps);
  Track *track;
  int getMaxLaps();
  void goStep();
  bool isStarted();
  bool isPaused();

private:
  bool initialized;
  bool started;
  bool paused;
  int maxLaps;
};

#endif // GAME_H
