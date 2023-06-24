#include "board.h"
#include <QBrush>
#include <QPen>
#include <ctime>
#include "utils.h"
#include "tile.h"
#include "gamescene.h"
#include "fontmanager.h"

Board::Board(GameScene* sc)
    : m_scene(sc), GRID_SIZE(4)
{
    srand(time(0));
    m_grid = QVector< QVector <Tile*>>(GRID_SIZE, QVector<Tile*>(GRID_SIZE, nullptr));
    for(int i = 0; i < GRID_SIZE; ++i)
    {
        for(int j = 0; j < GRID_SIZE; ++j)
        {
            m_grid[i][j] = nullptr;
        }
    }
    generateTiles();
}

void Board::generateTiles()
{
    QVector<int> rows;
    QVector<int> cols;
    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid.size(); j++)
        {
            if (m_grid[j][i] == nullptr)
            {
                cols.push_back(j);
                rows.push_back(i);
            }
        }
    }
    if (rows.size() == 0)
    {
        m_gameOver = true;
        return;
    }
    int val = rand() % 10;
    if (val != 4){
        val = 2;
    }
    Tile* t = new Tile(val);
    int index = rand() % rows.size();
    m_grid[cols[index]][rows[index]] = t;
    t->setPos((cols[index]+1) * TILE_STYLE::PADDING + (cols[index]) * TILE_STYLE::WIDTH, (rows[index]+1) * TILE_STYLE::PADDING + rows[index] * TILE_STYLE::HEIGHT);
}

bool Board::moveHorizontally(bool rightDirection)
{
    int curr;
    bool moved = false;
    for (int row = 0; row < GRID_SIZE; row++)
    {
        QPoint already_merged(-1, -1);
        for (int col = rightDirection ? GRID_SIZE - 2 : 1;
                       rightDirection ? col >= 0      : col < GRID_SIZE;
                       rightDirection ? col--         : col++)
        {
            if (m_grid[col][row] == nullptr)
            {
                continue;
            }
            curr = col;
            for (int to_col = rightDirection ? col + 1            : col - 1;
                              rightDirection ? to_col < GRID_SIZE : to_col >= 0;
                              rightDirection ? to_col++           : to_col--)
            {
                if (m_grid[to_col][row] != nullptr &&
                    (m_grid[to_col][row]->value() != m_grid[curr][row]->value() || already_merged == QPoint(to_col, row)))
                {
                    break;
                }
                if (m_grid[to_col][row] == nullptr)
                {
                    m_grid[to_col][row] = m_grid[curr][row];
                }
                else
                {
                    m_grid[to_col][row]->setValue(m_grid[to_col][row]->value() + m_grid[curr][row]->value());
                    m_points += 2 * m_grid[curr][row]->value();
                    moved = true;
                    m_grid[curr][row] = nullptr;
                    already_merged = { to_col, row };
                    break;
                }
                moved = true;
                m_grid[curr][row] = nullptr;
                rightDirection ? curr++ : curr--;
            }
        }
    }
    checkStatus();
    return moved;
}

bool Board::moveVertically(bool upDirection)
{
    int curr;
    bool moved = false;
    for (int col = 0; col < GRID_SIZE; col++)
    {
        QPoint already_merged(-1, -1);
        for (int row = upDirection ? 1               : GRID_SIZE - 2;
                       upDirection ? row < GRID_SIZE : row >= 0;
                       upDirection ? row++           : row--)
        {
            if (m_grid[col][row] == nullptr)
            {
                continue;
            }
            curr = row;
            for (int to_row = upDirection ? row - 1     : row + 1;
                              upDirection ? to_row >= 0 : to_row < GRID_SIZE;
                              upDirection ? to_row--    : to_row++)
            {
                if (m_grid[col][to_row] != nullptr &&
                    (m_grid[col][to_row]->value() != m_grid[col][curr]->value() || already_merged == QPoint(col, to_row)))
                {
                    break;
                }
                if (m_grid[col][to_row] == nullptr)
                {
                    m_grid[col][to_row] = m_grid[col][curr];
                }
                else
                {
                    m_grid[col][to_row]->setValue(m_grid[col][to_row]->value() + m_grid[col][curr]->value());
                    m_points += 2 * m_grid[col][curr]->value();
                    moved = true;
                    m_grid[col][curr] = nullptr;
                    already_merged = { col, to_row };
                    break;
                }
                moved = true;
                m_grid[col][curr] = nullptr;
                upDirection ? curr-- : curr++;
            }
        }
    }
    checkStatus();
    return moved;
}

void Board::update()
{
    QPoint pos(TILE_STYLE::PADDING, TILE_STYLE::PADDING);
    for (int row = 0; row < m_grid.size(); row++)
    {
        for (int col = 0; col < m_grid[row].size(); col++)
        {
            if (m_grid[col][row] != nullptr)
            {
                int dx = (pos.x() - m_grid[col][row]->pos().x()) / 10;
                int dy = (pos.y() - m_grid[col][row]->pos().y()) / 10;
                for (int i = 0; i < 10; i++)
                {
                    if (m_grid[col][row]->pos().x() != pos.x())
                    {
                        m_grid[col][row]->setPos(m_grid[col][row]->pos().x() + dx, m_grid[col][row]->pos().y());
                    }
                    else
                    {
                        m_grid[col][row]->setPos(m_grid[col][row]->pos().x(), m_grid[col][row]->pos().y() + dy);
                    }
                }
                m_grid[col][row]->setPos(pos);
            }
            pos.setX(pos.x() + TILE_STYLE::WIDTH + TILE_STYLE::PADDING);
        }
        pos.setX(TILE_STYLE::PADDING);
        pos.setY(pos.y() + TILE_STYLE::HEIGHT + TILE_STYLE::PADDING);
    }
}

void Board::drawBackground()
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem();
    rectItem->setBrush(QBrush(COLOR_STYLE::BACKGROUND));
    rectItem->setPen(QPen(COLOR_STYLE::BACKGROUND));
    rectItem->setRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());

    QPoint pos(TILE_STYLE::PADDING,TILE_STYLE::PADDING);
    for(int i = 0; i < GRID_SIZE; i++)
    {
        for(int j = 0; j < GRID_SIZE; j++)
        {
            QGraphicsRectItem* r = new QGraphicsRectItem(rectItem);
            r->setPen(QPen(COLOR_STYLE::PIECE));
            r->setBrush(QBrush(COLOR_STYLE::PIECE));
            r->setRect(0,0, TILE_STYLE::WIDTH, TILE_STYLE::HEIGHT);
            r->setPos(pos);
            pos.setX(pos.x() + TILE_STYLE::WIDTH + TILE_STYLE::PADDING);
        }
        pos.setX(TILE_STYLE::PADDING);
        pos.setY(pos.y() + TILE_STYLE::HEIGHT + TILE_STYLE::PADDING);
    }
    m_scene->addItem(rectItem);
}

void Board::drawTiles()
{
    foreach(QVector<Tile*> vec, m_grid)
    {
        foreach(Tile* tile, vec)
        {
            if(!tile)
            {
                continue;
            }
            Tile* tileRect = new Tile(tile->value());
            tileRect->setRect(tile->rect());
            tileRect->setPen(tile->pen());
            tileRect->setBrush(tile->brush());
            tileRect->setPos(tile->pos());
            m_scene->addItem(tileRect);
        }
    }
}

void Board::drawGameOver()
{
    if(!m_gameOver)
    {
        return;
    }
    QGraphicsRectItem* rItem = new QGraphicsRectItem;
    rItem->setBrush(QBrush(Qt::black));
    rItem->setPen(QPen(Qt::black));
    rItem->setRect(0,0,300, 300);
    rItem->setPos(SCREEN::PHYSICAL_SIZE.width()/2  - rItem->boundingRect().width()/2,
                  SCREEN::PHYSICAL_SIZE.height()/2 - rItem->boundingRect().height()/2);
    QGraphicsSimpleTextItem* tItem = new QGraphicsSimpleTextItem(rItem);
    QFont textFont = FontManager::Instance()->getFont(FontManager::FontID::STRING);
    tItem->setBrush(QBrush(Qt::white));
    tItem->setPen(QPen(Qt::white));
    textFont.setPointSize(TILE_STYLE::TEXT_POINT_SIZE);
    tItem->setFont(textFont);
    tItem->setText("Score: " + QString::number(m_points));
    tItem->setPos(rItem->boundingRect().width()/2  - tItem->boundingRect().width()/2,
                  rItem->boundingRect().height()/2 - tItem->boundingRect().height()/2);
    m_scene->addItem(rItem);
}

void Board::draw()
{
    drawBackground();
    drawTiles();
    drawGameOver();
}

void Board::checkStatus()
{
    //Find all empty squares, pick one at random from them
    QVector<int> rows;
    QVector<int> cols;

    for (int i = 0; i < m_grid.size(); i++)
    {
        for (int j = 0; j < m_grid.size(); j++)
        {
            if (m_grid[j][i] == nullptr)
            {
                cols.push_back(j);
                rows.push_back(i);
            }
        }
    }

    if (rows.size() == 0)
    {
        m_gameOver = true;
    }
}

