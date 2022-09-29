#include "game.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
//
const QString Game::PATH_TO_HERO_PIXMAP = ":/res/sprites/hero.png";
const QString Game::PATH_TO_TILES_PIXMAP = ":/res/sprites/tiles.png";
const float Game::PLAYER_VEL_Y_UP = -28.0f;

const QString Game::TITLE = "JUMPER";
const QSize Game::RESOLUTION = QSize(1366, 768);
Game::Game()
    : m_isMoveLeft(false), m_isMoveRight(false), m_isMoveUp(false), m_isMoveDown(false),
      m_isCtrlPressed(false), m_isPressedSpace(false)
{
    create();

    int TILE_SIZE_PIXMAP = 32;
    m_tileMap['B'] = QPixmap(spriteTiles->copy(0,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));                   //BLOCK
    m_tileMap['N'] = QPixmap(spriteTiles->copy(TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));           //BOX
    m_tileMap['C'] = QPixmap(spriteTiles->copy(2*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //COIN
    m_tileMap['P'] = QPixmap(spriteTiles->copy(3*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //PIPE00
    m_tileMap['p'] = QPixmap(spriteTiles->copy(4*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //PIPE10
    m_tileMap['R'] = QPixmap(spriteTiles->copy(3*TILE_SIZE_PIXMAP,TILE_SIZE_PIXMAP,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP)); //PIPE01
    m_tileMap['r'] = QPixmap(spriteTiles->copy(4*TILE_SIZE_PIXMAP,TILE_SIZE_PIXMAP,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP)); //PIPE00
    m_tileMap['?'] = QPixmap(spriteTiles->copy(5*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //QuestionMark
    m_tileMap['#'] = QPixmap(spriteTiles->copy(6*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //Ground
    m_tileMap['+'] = QPixmap(spriteTiles->copy(7*TILE_SIZE_PIXMAP,0,TILE_SIZE_PIXMAP, TILE_SIZE_PIXMAP));         //Wood
}

void Game::create()
{
    loadLevel(":/res/lvl/level1.txt");
    spriteTiles = new QPixmap(PATH_TO_TILES_PIXMAP);
    spriteHero  = new QPixmap(":/res/hero.png");
}

QChar Game::getTile(int x, int y) const
{
    if(x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
    {
        return sLevel[y * nLevelWidth + x];
    }
    else
        return QChar(' ');
}

void Game::setTile(int x, int y, QChar c)
{
    if(x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
    {
        sLevel[y * nLevelWidth + x] = c;
    }
}

bool Game::isFocused()
{
    return true;
}

void Game::handleInput()
{
    float fElapsedTime = 0.01f;
    if (isFocused())
    {
        if (isMoveUp())
        {
            fPlayerVelY = -6.0f;
        }

        if (isMoveDown())
        {
            fPlayerVelY = 6.0f;
        }

        if (isMoveLeft())
        {
            if(!isPressedCtrl())
            {
                fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
            }
            else
            {
                fPlayerVelX += (bPlayerOnGround ? -150.0f : -135.0f) * fElapsedTime;
            }
            nDirModY = 1;
        }

        if (isMoveRight())
        {
            if(!isPressedCtrl())
            {
                fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
            }
            else
            {
                fPlayerVelX += (bPlayerOnGround ? 150.0f : 135.0f) * fElapsedTime;
            }nDirModY = 0;
        }

        if (isPressedSpace())
        {
            if (fPlayerVelY == 0)
            {
                fPlayerVelY = -28.0f;
                nDirModX = 1;
            }
        }
        else
        {
            if (fPlayerVelY != 0)
            {
                if(fPlayerVelY < 0.0f)
                    fPlayerVelY = 0.0f;
                nDirModX = 0;
            }
        }
    }
}

bool Game::isMoveLeft()
{
    return m_isMoveLeft;
}

void Game::setMoveLeft(bool val)
{
    m_isMoveLeft = val;
    m_heroAnim.setMoveLeft(val);
}

bool Game::isMoveRight()
{
    return m_isMoveRight;
}

void Game::setMoveRight(bool val)
{
    m_isMoveRight = val;
    m_heroAnim.setMoveRight(val);
}

bool Game::isMoveUp()
{
    return m_isMoveUp;
}

void Game::setMoveUp(bool val)
{
    m_isMoveUp = val;
}

bool Game::isMoveDown()
{
    return m_isMoveDown;
}

void Game::setMoveDown(bool val)
{
    m_isMoveDown = val;
}

bool Game::isPressedSpace()
{
    return m_isPressedSpace;
}

void Game::setPressedSpace(bool val)
{
    m_isPressedSpace = val;
}

bool Game::isPressedCtrl()
{
    return m_isCtrlPressed;
}

void Game::setPressedCtrl(bool val)
{
    m_isCtrlPressed = val;
}

void Game::clampVelocities()
{
    if(isPressedCtrl() && fPlayerVelX > 12.0f)
    {
        fPlayerVelX = 12.0f;
    }
    else if (!isPressedCtrl() && fPlayerVelX > 10.0f)
    {
        fPlayerVelX = 10.0f;
    }

    if(isPressedCtrl() && fPlayerVelX < -12.0f)
    {
        fPlayerVelX = -12.0f;
    }
    else if (!isPressedCtrl() && fPlayerVelX < -10.0f)
    {
        fPlayerVelX = -10.0f;
    }

    if (fPlayerVelY > 100.0f)
    {
        fPlayerVelY = 100.0f;
    }

    if (fPlayerVelY < -100.0f)
    {
        fPlayerVelY = -100.0f;
    }
}

void Game::update()
{
    handleInput();
    float fElapsedTime = 0.01f;
    fPlayerVelY += 0.8f;

    // Drag
    if (bPlayerOnGround)
    {
        fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
        if (fabs(fPlayerVelX) < 0.01f)
        {
            fPlayerVelX = 0.0f;
        }
    }

    // Clamp velocities
    clampVelocities();

    // Calculate potential new position
    float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
    float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

    // Check for pickups!
    if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f) == 'o')
        setTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 0.0f, '.');

    if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) == 'o')
        setTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f, '.');

    if (getTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f) == 'o')
        setTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 0.0f, '.');

    if (getTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f) == 'o')
        setTile(fNewPlayerPosX + 1.0f, fNewPlayerPosY + 1.0f, '.');

    // Check for Collision
    if (fPlayerVelX <= 0) // Moving Left
    {
        if (getTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != '.'
                ||
            getTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != '.')
        {
            fNewPlayerPosX = (int)fNewPlayerPosX + 1;
            fPlayerVelX = 0;
        }
    }
    else // Moving Right
    {
        if (getTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != '.' || getTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != '.')
        {
            fNewPlayerPosX = (int)fNewPlayerPosX;
            fPlayerVelX = 0;

        }
    }

    bPlayerOnGround = false;
    m_heroAnim.setIsOnGround(false);
    if (fPlayerVelY <= 0) // Moving Up
    {
        if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != '.' || getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != '.')
        {
            fNewPlayerPosY = (int)fNewPlayerPosY + 1;
            fPlayerVelY = 0.1;
        }
    }
    else // Moving Down
    {
        if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != '.' || getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != '.')
        {
            fNewPlayerPosY = (int)fNewPlayerPosY;
            fPlayerVelY = 0;
            bPlayerOnGround = true; // Player has a solid surface underfoot
            m_heroAnim.setIsOnGround(true);
            nDirModX = 0;
        }
    }

    // Apply new position
    fPlayerPosX = fNewPlayerPosX;
    fPlayerPosY = fNewPlayerPosY;

    // Link camera to player position
    fCameraPosX = fPlayerPosX;
    fCameraPosY = fPlayerPosY;


}

QMap<QChar, QPixmap> Game::tileMap() const
{
    return m_tileMap;
}

void Game::loadLevel(QString pathToFile)
{
    QFile file(pathToFile);

    if( file.open(QIODevice::ReadOnly) )
    {
        nLevelHeight = 0;
        nLevelWidth = 0;
        sLevel = "";
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            sLevel += line;
            nLevelHeight++;
            nLevelWidth = line.length();
        }
    }
    file.close();
}
