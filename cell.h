#ifndef CELL_H
#define CELL_H
#include <assert.h>

///
///W klasie “Cell” przechowywane są informacje na temat pól planszy.
///Każdy obiekt ma zdefiniowaną swoją pozycję na planszy oraz reprezentowany typ ( droga, trawa, woda, piasek …).
///
class Cell
{
public:
  ///
  ///Direction (rotation) in angles. Each cell can be rotated, only important for UI.
  ///
  enum Direction{
    Up = 0,
    Right = 90,
    Down = 180,
    Left = 270
  };
  /** Cell Types:
   * Road - There is no terrain penalny when you ride on road cell
   * StartPosition - There can be only one startPosition cell on Track, it is used to measure car laps
   * BeforeStartPosition - Only one cell on track, specifies car start position
   * Sand - Cell which decreasse car speed
   * Grass - Similar to Sand
   */

  enum Type{
    RoadStraight = 0,
    RoadCorner,
    StartPosition,
    BeforeStartPosition,
    Sand,
    Stone,
    Grass,
    Water
  };

  Cell(int indexX, int indexY, Direction pointingDirection, Cell::Type type);

  void setType(Cell::Type type);
  ///
  ///90 degrees clockwise rotation
  ///
  void rotate();

  int getIndexX();
  int getIndexY();
  float getTerrainPenalty();
  Type getType();
  Direction getPointingDirection();
  ///
  ///There are four road cell types, function return true if argument is one of them
  ///
  static bool checkIfCellIsRoad(Cell::Type type);
private:
  Direction pointingDirection;
  Type type;
  ///
  ///Cell coordinates (first cell (0,0) second cell (0,1) and so on.
  ///
  int indexX,indexY;
};

#endif // CELL_H
