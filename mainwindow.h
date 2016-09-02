#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <track.h>
#include <playerselectwidget.h>
#include <game.h>

#include <QTimer>
#include <QDebug>
#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void goStep();

  void on_pushButtonEditMode_clicked();

  void on_comboBoxEditCellType_currentIndexChanged(int index);

  void on_pushButtonAddPlayer_clicked();

  void on_pushButtonStartGame_clicked();

  void on_pushButtonLoadTrack_clicked();

  void on_pushButtonSaveTrack_clicked();

  void on_pushButtonClearPlayers_clicked();

  void on_pushButtonPauseGame_clicked();

  void on_pushButtonLoadTrackOne_clicked();

  void on_pushButtonLoadTrackTwo_clicked();

  void on_pushButtonLoadTrackThree_clicked();

public slots:
  bool createPlayer(Player::PlayerType type, QString name, int keyUp, int keyDown, int keyRight, int keyLeft,
                    float maxSpeed, float minSpeed, float acceleration, float decceleration,
                    float idleDecceleration, float turningSpeed, float health);

  void startGame();
protected:
  bool eventFilter(QObject *object, QEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
private:
  void setUiEditMode(bool editMode);
  void setSecondsToStart(int i);
  void initPlayerInfo();
  void installEventFilters();
  void removeEventFilters();
  Ui::MainWindow *ui;
  Game *game;
  //Track *track;
  bool editMode;
  PlayerSelectWidget myPlayerSelectWidget;

  QTimer *gameStepTimer;
  QTimer *gameTimer;

  int secondsToStart;
};

#endif // MAINWINDOW_H
