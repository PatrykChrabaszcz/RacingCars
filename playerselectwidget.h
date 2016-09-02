#ifndef PLAYERSELECTWIDGET_H
#define PLAYERSELECTWIDGET_H

#include <player.h>

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
  class PlayerSelectWidget;
}

class PlayerSelectWidget : public QDialog
{
  Q_OBJECT

  enum Key{
    Up,
    Down,
    Right,
    Left
  };

public:
  explicit PlayerSelectWidget(QWidget *parent = 0);
  ~PlayerSelectWidget();

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();
signals:
  void playerCreated(Player::PlayerType, QString name, int keyUp, int keyDown, int keyRight, int keyLeft,
                     float maxSpeed,float minSpeed,float acceleration,float decceleration,
                     float idleDecceleration,float health,float turningSpeed);
protected:
  void keyPressEvent(QKeyEvent *event);
  bool eventFilter(QObject * object, QEvent * event);
private:
  QString name;
  bool readingKeys;
  int actualKey;

  int keyUp;
  int keyDown;
  int keyRight;
  int keyLeft;
  void installEventFilters();
  void removeEventFilters();
  Ui::PlayerSelectWidget *ui;
};

#endif // PLAYERSELECTWIDGET_H
