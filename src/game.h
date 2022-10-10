#ifndef GAME_H
#define GAME_H
#include <QString>
#include <QPixmap>
#include <QSize>
#include "heroanim.h"
#include <QMap>

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
    bool isPressedCtrl();
    void setPressedCtrl(bool val);

    void update();

    static const QString TITLE;
    static const QSize RESOLUTION;
    QString sLevel;
    int nLevelWidth, nLevelHeight;

    float fPlayerPosX = 1.0f;
    float fPlayerPosY = 13.0f;
    float fPlayerVelX = 0.0f;
    float fPlayerVelY = 0.0f;
    bool bPlayerOnGround = false;

    float fCameraPosX = 0.0f;
    float fCameraPosY = 0.0f;

    QPixmap* spriteTiles = nullptr;
    QPixmap* spriteHero = nullptr;

    int nDirModX = 0;
    int nDirModY = 0;

    bool m_isMoveLeft, m_isMoveRight, m_isMoveUp, m_isMoveDown, m_isCtrlPressed;
    bool m_isPressedSpace;

    constexpr static const int FPS = 60;

    static const QString PATH_TO_HERO_PIXMAP;
    static const QString PATH_TO_TILES_PIXMAP;
    static const float PLAYER_VEL_Y_UP;

    constexpr static const int TILE_SIZE = 64;

    constexpr static const QColor BG_COLOR = QColor(92, 148, 252);
    HeroAnim m_heroAnim;

    QMap<QChar, QPixmap> m_tileMap;
    QMap<QChar, QPixmap> tileMap() const;
    void loadLevel(QString pathToFile);
private:
    void clampVelocities();
};

#endif // GAME_H
