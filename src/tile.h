#ifndef TILE_H
#define TILE_H
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>

class Tile : public QGraphicsRectItem
{
public:
    Tile(int value);
    void setValue(int value);
    int value() const;
private:
    int m_value;
    QGraphicsSimpleTextItem* m_text;
};

#endif // TILE_H
