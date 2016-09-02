#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  editMode = false;
  ui->setupUi(this);

  ui->labelWinner->setVisible(false);
  setUiEditMode(false);
  ui->labelSecondsToStart->setVisible(false);

  connect(&myPlayerSelectWidget,SIGNAL(playerCreated(Player::PlayerType,QString,int,int,int,int,float,float,float,float,float,float,float)),
          this,SLOT(createPlayer(Player::PlayerType,QString,int,int,int,int,float,float,float,float,float,float,float)));

  gameStepTimer = new QTimer(this);
  gameStepTimer->setInterval(20);
  connect(gameStepTimer,SIGNAL(timeout()),this,SLOT(goStep()));

  gameTimer = new QTimer(this);
  gameTimer->setInterval(1000);
  connect(gameTimer,SIGNAL(timeout()),this,SLOT(startGame()));

  game = new Game;
  Track *track = new Track(10,10,10);
  game->setTrack(track);
  ui->track->setTrack(game->track);
  ui->track->setMyScene();

}
MainWindow::~MainWindow(){
  delete game->track;
  delete game;
  delete ui;
}

void MainWindow::goStep(){
  game->goStep();

  Player *winner = game->checkWinner(game->getMaxLaps());
  if(winner != NULL){

      ui->pushButtonStartGame->setText("Start Game");
      ui->pushButtonStartGame->setStyleSheet("border-radius: 5px;\nbackground-color: lightgreen");
      ui->pushButtonEditMode->setVisible(true);
      game->stopGame();
      gameStepTimer->stop();
      ui->labelWinner->setText(QString::fromStdString(winner->getName()) + " Win");
      QFont f( "Arial", 40, QFont::Bold);
      ui->labelWinner->setFont(f);
      ui->labelWinner->show();
  }
  if(game->playerVector.size() >= 1){
    int laps = game->playerVector[0]->car->getLaps();
    float health = game->playerVector[0]->car->getHealth();
    ui->progressBarPlayerOneHealth->setValue(health);
    ui->labelPlayerOneLaps->setText(QString::number(laps));
  }

  if(game->playerVector.size() >= 2){
    int laps = game->playerVector[1]->car->getLaps();
    float health = game->playerVector[1]->car->getHealth();
    ui->progressBarPlayerTwoHealth->setValue(health);
    ui->labelPlayerTwoLaps->setText(QString::number(laps));
  }

  if(game->playerVector.size() >= 3){
    int laps = game->playerVector[2]->car->getLaps();
    float health = game->playerVector[2]->car->getHealth();
    ui->progressBarPlayerThreeHealth->setValue(health);
    ui->labelPlayerThreeLaps->setText(QString::number(laps));
  }

  if(game->playerVector.size() >= 4){
    int laps = game->playerVector[3]->car->getLaps();
    float health = game->playerVector[3]->car->getHealth();
    ui->progressBarPlayerFourHealth->setValue(health);
    ui->labelPlayerFourLaps->setText(QString::number(laps));
  }
}

bool MainWindow::createPlayer(Player::PlayerType type , QString name , int keyUp, int keyDown , int keyRight, int keyLeft,
                              float maxSpeed, float minSpeed, float acceleration, float decceleration,
                              float idleDecceleration, float turningSpeed, float health ){
  if(game->playerVector.size() >= 4)
    return false;

  Player *player = game->addPlayer(type , name.toStdString(), keyUp, keyDown, keyRight, keyLeft);

  player->car->changeParameters(maxSpeed/100 * AbstractCar::BestSpeed,
                                (-1)*minSpeed/100* AbstractCar::BestSpeed,
                                acceleration/100 * AbstractCar::BestAcceleration,
                                decceleration/100 * AbstractCar::BestDecceleration,
                                idleDecceleration/100 * AbstractCar::BestAcceleration,
                                (float)health/100 * AbstractCar::BestHealth,
                                turningSpeed/100 * AbstractCar::BestTurningSpeed);
  switch (game->playerVector.size()){
    case 1:
      ui->labelPlayerOneName->setText(name);
      if(type == Player::ComputerPlayer)
        ui->labelPlayerOneType->setText("Computer");
      else if(type == Player::UserPlayer)
        ui->labelPlayerOneType->setText("User");
      else
        assert(false);
      break;
    case 2:
      ui->labelPlayerTwoName->setText(name);
      if(type == Player::ComputerPlayer)
        ui->labelPlayerTwoType->setText("Computer");
      else if(type == Player::UserPlayer)
        ui->labelPlayerTwoType->setText("User");
      else
        assert(false);
      break;
    case 3:
      ui->labelPlayerThreeName->setText(name);
      if(type == Player::ComputerPlayer)
        ui->labelPlayerThreeType->setText("Computer");
      else if(type == Player::UserPlayer)
        ui->labelPlayerThreeType->setText("User");
      else
        assert(false);
      break;
    case 4:
      ui->labelPlayerFourName->setText(name);
      if(type == Player::ComputerPlayer)
        ui->labelPlayerFourType->setText("Computer");
      else if(type == Player::UserPlayer)
        ui->labelPlayerFourType->setText("User");
      else
        assert(false);
      break;
    }
  return true;
}

void MainWindow::installEventFilters(){
  ui->comboBoxEditCellType->installEventFilter(this);
  ui->pushButtonStartGame->installEventFilter(this);
  ui->pushButtonAddPlayer->installEventFilter(this);
  ui->pushButtonLoadTrack->installEventFilter(this);
  ui->pushButtonSaveTrack->installEventFilter(this);
  ui->lineEditTrackPath->installEventFilter(this);

}

void MainWindow::removeEventFilters(){
  ui->comboBoxEditCellType->removeEventFilter(this);
  ui->pushButtonStartGame->removeEventFilter(this);
  ui->pushButtonAddPlayer->removeEventFilter(this);
  ui->pushButtonLoadTrack->removeEventFilter(this);
  ui->pushButtonSaveTrack->removeEventFilter(this);
  ui->lineEditTrackPath->removeEventFilter(this);

}
bool MainWindow::eventFilter(QObject *object, QEvent *event )
{
  if(event->type() == QEvent::KeyPress){
    keyPressEvent(dynamic_cast<QKeyEvent*>(event));
    return true;
  }
  if(event->type() == QEvent::KeyRelease ){
    keyReleaseEvent(dynamic_cast<QKeyEvent*>(event));
    return true;
  }
  return false;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if(!event->isAutoRepeat())
    foreach(Player *player, game->playerVector){
      player->keyChanged(event->key(),true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
  if(!event->isAutoRepeat())
  foreach(Player *player, game->playerVector){
    player->keyChanged(event->key(),false);
  }
}

void MainWindow::on_pushButtonEditMode_clicked()
{
  if(editMode == false){
    editMode = true;
    removeEventFilters();
  }
  else{
    editMode = false;
    installEventFilters();
  }
  setUiEditMode(editMode);
}

void MainWindow::on_comboBoxEditCellType_currentIndexChanged(int index){
  ui->track->changeEditCellType(Cell::Type(index));
}

void MainWindow::on_pushButtonAddPlayer_clicked(){
  myPlayerSelectWidget.show();
}

void MainWindow::on_pushButtonStartGame_clicked(){
  if(game->isStarted()){
    ui->pushButtonStartGame->setText("Start Game");
    ui->pushButtonStartGame->setStyleSheet("border-radius: 5px;\nbackground-color: lightgreen");
    ui->pushButtonEditMode->setVisible(true);
    game->stopGame();
    gameStepTimer->stop();
  }
  else{
    if(game->initGame()){
      ui->pushButtonStartGame->setText("Stop Game");
      ui->pushButtonStartGame->setStyleSheet("border-radius: 5px;\nbackground-color: red");
      ui->pushButtonEditMode->setVisible(false);
      ui->labelWinner->setVisible(false);

      setSecondsToStart(3);
      gameTimer->start();
      ui->track->setMyScene();
    }
  }
}
void MainWindow::startGame(){

  setSecondsToStart(secondsToStart-1);

  if(secondsToStart == 0){
      game->setMaxLaps(ui->spinBoxNumberOfMaxLaps->value());
      game->startGame();
      gameStepTimer->start();
  }
  if(secondsToStart < 0){
    gameTimer->stop();
  }

}

void MainWindow::on_pushButtonLoadTrack_clicked(){
  game->track->loadTrack(ui->lineEditTrackPath->text().toStdString());
  ui->track->setMyScene();
}

void MainWindow::on_pushButtonSaveTrack_clicked(){
  game->track->saveTrack(ui->lineEditTrackPath->text().toStdString());
}

void MainWindow::on_pushButtonClearPlayers_clicked()
{
  game->clearPlayers();
  initPlayerInfo();
  ui->track->setMyScene();
}
void MainWindow::initPlayerInfo(){
  ui->labelPlayerOneLaps->setText("-1");
  ui->labelPlayerTwoLaps->setText("-1");
  ui->labelPlayerThreeLaps->setText("-1");
  ui->labelPlayerFourLaps->setText("-1");

  ui->labelPlayerOneName->setText("Empty");
  ui->labelPlayerTwoName->setText("Empty");
  ui->labelPlayerThreeName->setText("Empty");
  ui->labelPlayerFourName->setText("Empty");

  ui->labelPlayerOneType->setText("Empty");
  ui->labelPlayerTwoType->setText("Empty");
  ui->labelPlayerThreeType->setText("Empty");
  ui->labelPlayerFourType->setText("Empty");

  ui->progressBarPlayerOneHealth->setValue(0);
  ui->progressBarPlayerTwoHealth->setValue(0);
  ui->progressBarPlayerThreeHealth->setValue(0);
  ui->progressBarPlayerFourHealth->setValue(0);
}

void MainWindow::setSecondsToStart(int i){
  secondsToStart = i;
  ui->labelSecondsToStart->setText(QString::number(secondsToStart));
  if (i == 3)
    ui->labelSecondsToStart->setStyleSheet("color: red");
  if(i == 2)
    ui->labelSecondsToStart->setStyleSheet("color: orange");
  if(i == 1)
    ui->labelSecondsToStart->setStyleSheet("color: green");
  if(i == 0){
    ui->labelSecondsToStart->setText("Go");
    ui->labelSecondsToStart->setStyleSheet("color: green");
  }
  if(i < 0)
    ui->labelSecondsToStart->setVisible(false);
  else
    ui->labelSecondsToStart->setVisible(true);

}

void MainWindow::setUiEditMode(bool editMode){
  //Edit
  ui->pushButtonLoadTrack->setVisible(editMode);
  ui->pushButtonSaveTrack->setVisible(editMode);
  ui->lineEditTrackPath->setVisible(editMode);
  ui->comboBoxEditCellType->setVisible(editMode);
  ui->comboBoxEditCellType->setVisible(editMode);
  ui->pushButtonAddPlayer->setVisible(editMode);
  ui->pushButtonClearPlayers->setVisible(editMode);
  ui->pushButtonLoadTrackOne->setVisible(editMode);
  ui->pushButtonLoadTrackTwo->setVisible(editMode);
  ui->pushButtonLoadTrackThree->setVisible(editMode);
  ui->spinBoxNumberOfMaxLaps->setVisible(editMode);
  ui->labelLaps->setVisible(editMode);


  ui->track->setEnabled(editMode);
  if(editMode){
      ui->labelWinner->setVisible(!editMode);
      ui->pushButtonEditMode->setText("EditMode is On");
      ui->pushButtonEditMode->setStyleSheet("border-radius: 10px; background-color: lightgreen;" );
    }
  else{
    ui->pushButtonEditMode->setText("EditMode is Off");
    ui->pushButtonEditMode->setStyleSheet("border-radius: 10px; background-color: red;" );
  }

  //GamePlay
  ui->pushButtonStartGame->setVisible(!editMode);
  ui->pushButtonPauseGame->setVisible(!editMode);
}

void MainWindow::on_pushButtonPauseGame_clicked()
{
  if(!game->isStarted())
    return;
  if(!game->isPaused()){
    game->pauseGame();
    this->gameStepTimer->stop();
    this->ui->pushButtonPauseGame->setText("Resume Game");

  }
  else{
    game->resumeGame();
    this->gameStepTimer->start();
    this->ui->pushButtonPauseGame->setText("Pause Game");
  }
}

void MainWindow::on_pushButtonLoadTrackOne_clicked(){

  game->track->loadTrack(":/Tracks/trackOne.txt");
  ui->track->setMyScene();
}

void MainWindow::on_pushButtonLoadTrackTwo_clicked(){
  game->track->loadTrack(":/Tracks/trackTwo.txt");
  ui->track->setMyScene();
}

void MainWindow::on_pushButtonLoadTrackThree_clicked(){
  game->track->loadTrack(":/Tracks/trackThree.txt");
  ui->track->setMyScene();
}
