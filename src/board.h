#ifndef BOARD_H
#define BOARD_H
#include <QGraphicsRectItem>
#include <QVector>

class Tile;
class GameScene;
class Board
{
public:
    Board(GameScene* sc);
    void generateTiles();

    bool moveHorizontally(bool rightDirection);
    bool moveVertically(bool upDirection);
    void update();

    void drawBackground();
    void drawTiles();
    void drawGameOver();
    void draw();
    void checkStatus();
private:
    int m_points = 0;
    bool m_gameOver = false;
    QVector<QVector<Tile*>> m_grid;
    GameScene* m_scene;
    const int GRID_SIZE;
};

#endif // BOARD_H
