#include "tile.h"
#include "utils.h"
#include "fontmanager.h"
#include <QBrush>
#include <QPen>

Tile::Tile(int value)
    : m_value(value)
{
    setBrush(QBrush(COLOR_STYLE::TILES[log2(m_value) - 1]));
    setPen(QPen(COLOR_STYLE::TILES[log2(m_value) - 1]));

    setRect(0, 0,
             TILE_STYLE::WIDTH,
             TILE_STYLE::HEIGHT);

    m_text = new QGraphicsSimpleTextItem("", this);
    QFont textFont = FontManager::Instance()->getFont(FontManager::FontID::STRING);
    textFont.setPointSize(TILE_STYLE::TEXT_POINT_SIZE);
    m_text->setFont(textFont);
    m_text->setText(QString::number(m_value));
    m_text->setPos(boundingRect().width()/2  - m_text->boundingRect().width()/2,
                   boundingRect().height()/2 - m_text->boundingRect().height()/2);
}

void Tile::setValue(int value)
{
    if(m_value == value)
    {
        return;
    }
    m_value = value;
    setBrush(QBrush(COLOR_STYLE::TILES[log2(m_value) - 1]));
    setPen(QPen(COLOR_STYLE::TILES[log2(m_value) - 1]));
    m_text->setText(QString::number(m_value));
    m_text->setPos(boundingRect().width()/2  - m_text->boundingRect().width()/2,
                   boundingRect().height()/2 - m_text->boundingRect().height()/2);
}

int Tile::value() const
{
    return m_value;
}
