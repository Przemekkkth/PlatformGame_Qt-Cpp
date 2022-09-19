#include "game.h"
#include <QDebug>

const QString Game::TITLE = "JUMPER";
const QSize Game::RESOLUTION = QSize(256, 240);
Game::Game()
    : m_isMoveLeft(false), m_isMoveRight(false), m_isMoveUp(false), m_isMoveDown(false),
      m_isPressedSpace(false)
{
    create();
}

void Game::create()
{
    nLevelWidth = 64;
    nLevelHeight = 16;
    sLevel += "................................................................";
    sLevel += "................................................................";
    sLevel += ".......ooooo....................................................";
    sLevel += "........ooo.....................................................";
    sLevel += ".......................########.................................";
    sLevel += ".....BB?BBBB?BB.......###..............#.#......................";
    sLevel += "....................###................#.#......................";
    sLevel += "...................####.........................................";
    sLevel += "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG.##############.....########";
    sLevel += "...................................#.#...............###........";
    sLevel += "........................############.#............###...........";
    sLevel += "........................#............#.........###..............";
    sLevel += "........................#.############......###.................";
    sLevel += "........................#................###....................";
    sLevel += "........................#################.......................";
    sLevel += "................................................................";

    spriteTiles = new QPixmap(":/res/tiles.png");
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
            fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) * fElapsedTime;
            nDirModY = 1;
        }

        if (isMoveRight())
        {
            fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) * fElapsedTime;
            nDirModY = 0;
        }

        if (isPressedSpace())
        {
            if (fPlayerVelY == 0)
            {
                fPlayerVelY = -12.0f;
                nDirModX = 1;
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
}

bool Game::isMoveRight()
{
    return m_isMoveRight;
}

void Game::setMoveRight(bool val)
{
    m_isMoveRight = val;
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

void Game::update()
{
    handleInput();
    float fElapsedTime = 0.01f;
    fPlayerVelY += 20.0f * fElapsedTime;

    // Drag
    if (bPlayerOnGround)
    {
        fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
        if (fabs(fPlayerVelX) < 0.01f)
            fPlayerVelX = 0.0f;
    }

    // Clamp velocities
    if (fPlayerVelX > 10.0f)
        fPlayerVelX = 10.0f;

    if (fPlayerVelX < -10.0f)
        fPlayerVelX = -10.0f;

    if (fPlayerVelY > 100.0f)
        fPlayerVelY = 100.0f;

    if (fPlayerVelY < -100.0f)
        fPlayerVelY = -100.0f;

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
        if (getTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != '.' || getTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != '.')
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
    if (fPlayerVelY <= 0) // Moving Up
    {
        if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != '.' || getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY) != '.')
        {
            fNewPlayerPosY = (int)fNewPlayerPosY + 1;
            fPlayerVelY = 0;
        }
    }
    else // Moving Down
    {
        if (getTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY + 1.0f) != '.' || getTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != '.')
        {
            fNewPlayerPosY = (int)fNewPlayerPosY;
            fPlayerVelY = 0;
            bPlayerOnGround = true; // Player has a solid surface underfoot
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
