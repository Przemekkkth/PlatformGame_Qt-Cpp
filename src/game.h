#ifndef GAME_H
#define GAME_H
#include <QString>
#include <QPixmap>
#include <QSize>
#include "heroanim.h"

class Game
{
public:
    Game();
    void create();
    QChar getTile(int x, int y) const;
    void setTile(int x, int y, QChar c);
    bool isFocused();
    void handleInput();

    bool isMoveLeft();
    void setMoveLeft(bool val);
    bool isMoveRight();
    void setMoveRight(bool val);
    bool isMoveUp();
    void setMoveUp(bool val);
    bool isMoveDown();
    void setMoveDown(bool val);
    bool isPressedSpace();
    void setPressedSpace(bool val);

    void update();

    static const QString TITLE;
    static const QSize RESOLUTION;
    QString sLevel;
    int nLevelWidth, nLevelHeight;

    float fPlayerPosX = 1.0f;
    float fPlayerPosY = 1.0f;
    float fPlayerVelX = 0.0f;
    float fPlayerVelY = 0.0f;
    bool bPlayerOnGround = false;

    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;

    QPixmap* spriteTiles = nullptr;
    QPixmap* spriteHero = nullptr;

    int nDirModX = 0;
    int nDirModY = 0;

    bool m_isMoveLeft, m_isMoveRight, m_isMoveUp, m_isMoveDown;
    bool m_isPressedSpace;

    constexpr static const int FPS = 60;
    static const QString PATH_TO_WOOD_PIXMAP;
    static const QString PATH_TO_ROCK_PIXMAP;
    static const QString PATH_TO_HERO_PIXMAP;
    static const QString PATH_TO_COIN_PIXMAP;
    static const QString PATH_TO_BOX_PIXMAP;
    static const QString PATH_TO_QUESTION_MARK_PIXMAP;

    constexpr static const int TILE_SIZE = 64;

    constexpr static const QColor BG_COLOR = QColor(92, 148, 252);
    HeroAnim m_heroAnim;
};

#endif // GAME_H
