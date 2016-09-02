#include "playerselectwidget.h"
#include "ui_playerselectwidget.h"
PlayerSelectWidget::PlayerSelectWidget(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PlayerSelectWidget)
{
  readingKeys = false;
  actualKey = 0;
  ui->setupUi(this);
}

PlayerSelectWidget::~PlayerSelectWidget()
{
  delete ui;
}

void PlayerSelectWidget::keyPressEvent(QKeyEvent *event){
  if(event->isAutoRepeat())
    return;
  if(event->type() == QEvent::KeyRelease)
    return;

  if(event->key() == Qt::Key_Return){
    QDialog::keyPressEvent(event);
    }
  if(readingKeys){
    switch (actualKey){
      case PlayerSelectWidget::Up :{
        keyUp = event->key();
        actualKey = PlayerSelectWidget::Down;
        ui->lineEditInformation->setText("Press Down Key");
        break;
      }
      case PlayerSelectWidget::Down :{
        if(keyUp == event->key())
          return;
        keyDown = event->key();
        actualKey = PlayerSelectWidget::Right;
        ui->lineEditInformation->setText("Press Right Key");
        break;
      }
      case PlayerSelectWidget::Right:{
        if(keyUp == event->key() || keyDown == event->key())
          return;
        keyRight = event->key();
        actualKey = PlayerSelectWidget::Left;
        ui->lineEditInformation->setText("Press Left Key");
        break;
      }
      case PlayerSelectWidget::Left:{
        if(keyUp == event->key() || keyDown == event->key() || keyRight == event->key())
          return;
        keyLeft = event->key();
        readingKeys = false;

        emit playerCreated(Player::UserPlayer, name, keyUp, keyDown, keyRight, keyLeft,
                           ui->horizontalSliderMaxSpeed->value(), ui->horizontalSliderMinSpeed->value(),
                           ui->horizontalSliderAcceleration->value(), ui->horizontalSliderDecceleration->value(),
                           ui->horizontalSliderIdleDecceleration->value(), ui->horizontalSliderTurn->value(),
                           ui->horizontalSliderHealth->value());
        ui->lineEditName->clear();
        removeEventFilters();
        this->hide();
        break;
      }
    }
  }
}

void PlayerSelectWidget::on_buttonBox_accepted()
{
  focusNextChild();
  name = ui->lineEditName->text();
  if(name.isEmpty()){
    ui->lineEditInformation->setText("Name can't be empty");
    return;
  }

  Player::PlayerType type = Player::PlayerType(ui->comboBox->currentIndex());
  if(type == Player::UserPlayer){
    installEventFilters();
    ui->lineEditInformation->setText("Press Up Key");
    actualKey = PlayerSelectWidget::Up;
    keyUp = 0;
    keyDown = 0;
    keyRight = 0;
    keyLeft = 0;
    readingKeys = true;
  }
  else if(type == Player::ComputerPlayer){
    emit playerCreated(Player::ComputerPlayer, name, 0,0,0,0,
                       ui->horizontalSliderMaxSpeed->value(), ui->horizontalSliderMinSpeed->value(),
                       ui->horizontalSliderAcceleration->value(), ui->horizontalSliderDecceleration->value(),
                       ui->horizontalSliderIdleDecceleration->value(),
                       ui->horizontalSliderTurn->value(),
                       ui->horizontalSliderHealth->value());
    ui->lineEditName->clear();
    removeEventFilters();
    this->hide();

  }
  else
    assert(false);
}

void PlayerSelectWidget::installEventFilters(){
  ui->lineEditName->installEventFilter(this);
  ui->buttonBox->installEventFilter(this);
  ui->comboBox->installEventFilter(this);
  ui->lineEditInformation->installEventFilter(this);
}

void PlayerSelectWidget::removeEventFilters(){
  ui->lineEditName->removeEventFilter(this);
  ui->buttonBox->removeEventFilter(this);
  ui->comboBox->removeEventFilter(this);
  ui->lineEditInformation->removeEventFilter(this);
}

bool PlayerSelectWidget::eventFilter(QObject *object, QEvent *event){
  if(event->type() == QEvent::KeyPress){
    keyPressEvent(dynamic_cast<QKeyEvent*>(event));

  }
  else
    return QObject::eventFilter(object,event);
  return true;
}

void PlayerSelectWidget::on_buttonBox_rejected()
{
  this->hide();
}
