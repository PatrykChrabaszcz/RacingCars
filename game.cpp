#include "game.h"

Game::Game()
{
  initialized = false;
  stopGame();
  pauseGame();
}
Game::~Game(){
  foreach (Player *player, playerVector) {
    delete player;
  }
}

void Game::setTrack (Track *track){
  this->track = track;
}
void Game::createTrack(int numberOfCellsH, int numberOfCellsV, int cellSize){
  track = new Track(numberOfCellsH,numberOfCellsV,cellSize);
}

void Game::goStep(){
  if(started && !paused){
    foreach(Player *player, playerVector){
      player->car->goStep(track);
    }
  }
}

bool Game::isStarted(){
  return started;
}
bool Game::isPaused(){
  return paused;
}

void Game::setMaxLaps(int laps){
  maxLaps = laps;
}

int Game::getMaxLaps()
{
  return maxLaps;;
}

Player * Game::checkWinner(int laps){
  foreach (Player *player, playerVector) {
      if(player->car->getLaps()>=laps)
        return player;
    }
  return NULL;
}

bool Game::initGame(){
  track->carVector.clear();

  foreach(Player *player, playerVector){
    track->addCar(player->car);
    player->car->init();
  }
  if(track->checkIfReady()){
    track->setCarsStartPosition();
    initialized = true;
    return true;
  }
  return false;
}

bool Game::startGame(){
  initGame();

  if(initialized){
    started = true;
    paused = false;
    return true;
  }
  return false;
}

void Game::pauseGame(){
  paused = true;
}

void Game::resumeGame(){
  paused = false;
}

void Game::stopGame(){
  started = false;
  initialized = false;
}

bool Game::checkUsers(){
  if(playerVector.empty())
     return false;
  return true;
}

Player* Game::addPlayer(Player::PlayerType type, std::string name, int keyUp, int keyDown, int keyRight, int keyLeft){

  if(playerVector.size()>=4 )
    return NULL;
  AbstractCar *playerCar;
  if(type == Player::UserPlayer){
      playerCar = new UserCar();
    }
  else if(type == Player::ComputerPlayer){
      playerCar = new ComputerCar();
    }
  else
    assert(false);

  Player *newPlayer = new Player(playerCar, name);
  newPlayer->defineKeys(keyUp,keyDown,keyRight,keyLeft);
  playerVector.push_back(newPlayer);

  //Also returns actual pointer if you wanna perform some operations
  return newPlayer;
}

void Game::clearPlayers(){
  foreach(Player *player, playerVector){
    delete player;
  }

  playerVector.clear();
  track->clearCarVector();
}
