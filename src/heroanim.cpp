#include "heroanim.h"
#include "game.h"
#include <QDebug>

HeroAnim::HeroAnim(QObject *parent)
    : QObject{parent}, COUNT_OF_WALK_FRAMES(7), L_JUMP_P(QPoint(0, 2*Game::TILE_SIZE)),
      R_JUMP_P(QPoint(Game::TILE_SIZE, 2*Game::TILE_SIZE)), m_currentFrame(0), m_pixmap(Game::PATH_TO_HERO_PIXMAP),
      m_isMoveLeft(false), m_isMoveRight(false), m_isJump(false)
{
    m_currentPixmap = m_pixmap.copy(0,0, Game::TILE_SIZE, Game::TILE_SIZE);
    connect(&m_timer, &QTimer::timeout, this, &HeroAnim::updatePixmap);
    m_timer.start(500);
}

QPixmap HeroAnim::currentPixmap() const
{
    return m_currentPixmap;
}

void HeroAnim::setMoveRight(bool val)
{
    m_isMoveRight = val;
    m_isMoveLeft = false;
}

void HeroAnim::setMoveLeft(bool val)
{
    m_isMoveLeft = val;
    m_isMoveRight = false;
}

void HeroAnim::setJump(bool val)
{
    m_isJump = val;
}

void HeroAnim::updatePixmap()
{
    if(!m_isJump && !m_isMoveRight)
    {
        m_currentPixmap = m_pixmap.copy( R_JUMP_P.x(), R_JUMP_P.y(),
                                       Game::TILE_SIZE, Game::TILE_SIZE);
        m_currentFrame = 0;
        return;
    }
    else if(!m_isJump)
    {
        m_currentPixmap = m_pixmap.copy( L_JUMP_P.x(), L_JUMP_P.y(),
                                       Game::TILE_SIZE, Game::TILE_SIZE);
        m_currentFrame = 0;
        return;
    }

    if(m_isMoveLeft)
    {
        m_currentPixmap = m_pixmap.copy( m_currentFrame*Game::TILE_SIZE, Game::TILE_SIZE,
                                         Game::TILE_SIZE, Game::TILE_SIZE);
        m_currentFrame++;
        m_currentFrame %= COUNT_OF_WALK_FRAMES;
        return;
    }
    if(m_isMoveRight)
    {
        m_currentPixmap = m_pixmap.copy( m_currentFrame*Game::TILE_SIZE, 0,
                                         Game::TILE_SIZE, Game::TILE_SIZE);
        m_currentFrame++;
        m_currentFrame %= COUNT_OF_WALK_FRAMES;
        return;
    }
    m_currentPixmap = m_pixmap.copy(0,0, Game::TILE_SIZE, Game::TILE_SIZE);
}
