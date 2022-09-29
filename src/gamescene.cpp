#include "gamescene.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, m_loopSpeed(int(1000.0f/Game::FPS)), m_loopTime(0.0f), m_deltaTime(0.0f)
{
    setSceneRect(0,0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    m_game.update();
    drawTiles();

    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/Game::FPS));
    m_elapsedTimer.start();

    setBackgroundBrush(Game::BG_COLOR);
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    if( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        m_game.update();
        clear();
        drawTiles();
    }
}

void GameScene::drawTiles()
{
    // Draw Level
    int nTileWidth = Game::TILE_SIZE;
    int nTileHeight = Game::TILE_SIZE;
    int nVisibleTilesX = Game::RESOLUTION.width() / nTileWidth;
    int nVisibleTilesY = Game::RESOLUTION.height() / nTileHeight;

    // Calculate Top-Leftmost visible tile
    float fOffsetX = m_game.fCameraPosX - (float)nVisibleTilesX / 2.0f;
    float fOffsetY = m_game.fCameraPosY - (float)nVisibleTilesY / 2.0f;

    // Clamp camera to game boundaries
    if (fOffsetX < 0) fOffsetX = 0;
    if (fOffsetY < 0) fOffsetY = 0;
    if (fOffsetX > m_game.nLevelWidth - nVisibleTilesX) fOffsetX = m_game.nLevelWidth - nVisibleTilesX;
    if (fOffsetY > m_game.nLevelHeight - nVisibleTilesY) fOffsetY = m_game.nLevelHeight - nVisibleTilesY;

    // Get offsets for smooth movement
    float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
    float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

    // Draw visible tile map
    for (int x = -1; x < nVisibleTilesX + 2; x++)
    {
        for (int y = -1; y < nVisibleTilesY + 1; y++)
        {
            QChar sTileID = m_game.getTile(x + fOffsetX, y + fOffsetY);
            //check sTile in Game::m_mapTile if exist draw Pixmap
            if(m_game.tileMap().contains(sTileID))
            {
                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
                pItem->setPixmap(m_game.tileMap()[sTileID].scaled(nTileWidth, nTileHeight));
                pItem->setPos(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);
                addItem(pItem);
            }
        }
    }

    // Draw Player
    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem(m_game.m_heroAnim.currentPixmap().scaled(Game::TILE_SIZE, Game::TILE_SIZE));
    pItem->setPos((m_game.fPlayerPosX - fOffsetX) * nTileWidth, (m_game.fPlayerPosY - fOffsetY) * nTileWidth);
    addItem(pItem);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
    {
        m_game.setMoveUp(true);
    }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
    {
        m_game.setMoveRight(true);
    }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
    {
        m_game.setMoveDown(true);
    }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
    {
        m_game.setMoveLeft(true);
    }
        break;
    case Qt::Key_Space:
    {
        m_game.setPressedSpace(true);
    }
        break;
    case Qt::Key_Control:
        m_game.setPressedCtrl(true);
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
    {
        m_game.setMoveUp(false);
    }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
    {
        m_game.setMoveRight(false);
    }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
    {
        m_game.setMoveDown(false);
    }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
    {
        m_game.setMoveLeft(false);
    }
        break;
    case Qt::Key_Space:
    {
        m_game.setPressedSpace(false);
    }
        break;
    case Qt::Key_Control:
    {
        m_game.setPressedCtrl(false);
    }
        break;
    }
    QGraphicsScene::keyReleaseEvent(event);
}
