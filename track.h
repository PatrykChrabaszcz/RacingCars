#ifndef TRACK_H
#define TRACK_H

#include <cell.h>
#include <abstractcar.h>
#include <stdlib.h>
#include <assert.h>
#include <list>
#include <fstream>

#include <QPoint>
#include <QFile>
#include <QDataStream>
#include <QTimer>
class AbstractCar;
///
///Ta klasa tworzy/usuwa/edytuje komórki bazowe (Cells).
/// Posiada funkcje zapisu/odczytu z pliku.
/// Posiada wskaźniki do samochodów znajdujących się na trasie.
/// Posiada funkcję sprawdzającą poprawność trasy oraz funkcję ustawiającą samochody na początku trasy.
///

class Track
{
public:
  Track(int numberOfCellsH = 10, int numberOfCellsV = 10, int cellSize = 10);
  ~Track();
  std::vector<AbstractCar *> carVector;
  std::vector<Cell *> cellVector;

  /** Track File format :
   * -First int is track width measured in number of cells
   * -Second int is track height measured in number of cells
   * -Then there are (width * height) cells each described as two int numbers
   * indicating cell type and cell rotation
   */


  void loadTrack(std::string filepath);
  void saveTrack(std::string filePath);

  //Edit Track cell
  void editCell(int x, int y, Cell::Type);
  void rotateCell(int x, int y);

  //Check Track ready state
  bool checkIfReady();

  //Add Car into carVector
  void addCar(AbstractCar *newCar);
  void clearCarVector();

  Cell::Type getCellType(int x, int y);
  float getCellTerrainPenalty(int x, int y);
  int getCellSize();
  int getNumberOfCellsH();
  int getNumberOfCellsV();

  void setCarsStartPosition();
  bool static checkRectanglesOverlap(QPointF *firstRect, QPointF *secondRect);

private:
  ///
  ///Number of cells measured in horizontal direction
  ///
  int numberOfCellsH;
  ///
  ///Number of cells measured in vertical direction
  ///
  int numberOfCellsV;

  ///
  ///Indicator of track state, if true we can start game
  ///
  bool ready;
  ///
  ///Size of each cell in pixels
  ///
  int cellSize;

  QPoint beforeStartCellIndex;
  QPoint startCellIndex;
};

#endif // TRACK_H
