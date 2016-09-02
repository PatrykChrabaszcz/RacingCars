#ifndef TRACKGUI_H
#define TRACKGUI_H

#include <track.h>
#include <cell.h>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsRectItem>

class TrackGUI : public QGraphicsView
{
  Q_OBJECT
public:
  explicit TrackGUI(QWidget *parent = 0);
  void setTrack(Track *displayedTrack);

  void setMyScene();
  void changeEditCellType(Cell::Type type);
  std::vector<QGraphicsRectItem *> carVector;
signals:

public slots:

protected:
  void mousePressEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

private:
  double sizeFactorX;
  double sizeFactorY;
  Track *displayedTrack;
  QGraphicsScene *scene;

  Cell::Type editCellType;

};

#endif // TRACKGUI_H
