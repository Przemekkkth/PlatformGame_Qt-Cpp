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
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    if( m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;
        //setBackgroundBrush(Qt::cyan);
        m_game.update();
        clear();
        drawTiles();
    }
}

void GameScene::drawTiles()
{
    // Draw Level
    int nTileWidth = 32;
    int nTileHeight = 32;
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
    for (int x = -1; x < nVisibleTilesX + 1; x++)
    {
        for (int y = -1; y < nVisibleTilesY + 1; y++)
        {
            QChar sTileID = m_game.getTile(x + fOffsetX, y + fOffsetY);
            switch (sTileID.toLatin1())
            {
            case '.': // Sky
                //Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_CYAN);
            {
                QGraphicsRectItem* rItem = new QGraphicsRectItem();
                //rItem->setRect(0,0, ((x + 1) * nTileWidth - fTileOffsetX - x * nTileWidth - fTileOffsetX), ((y + 1) * nTileHeight - fTileOffsetY - y * nTileHeight - fTileOffsetY));
                rItem->setRect(0,0, nTileWidth, nTileHeight);
                rItem->setPos((x * nTileWidth - fTileOffsetX), (y * nTileHeight - fTileOffsetY));
                rItem->setBrush(QBrush(Qt::cyan));
                rItem->setPen(QPen(Qt::cyan));
                //qDebug() << "rect " << rItem->boundingRect();
                addItem(rItem);
            }
                break;
            case '#': // Solid Block
                //Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_RED);
                //DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 2 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
            {
                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
                pItem->setPixmap(QPixmap(Game::PATH_TO_ROCK_PIXMAP).scaled(nTileWidth, nTileHeight));
                pItem->setPos(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);
                addItem(pItem);
            }
                break;
            case 'G': // Ground Block
                //DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
            {
                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
                pItem->setPixmap(QPixmap(Game::PATH_TO_WOOD_PIXMAP).scaled(nTileWidth, nTileHeight));
                pItem->setPos(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY);
                addItem(pItem);
            }
                break;
            case 'B': // Brick Block
                //DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 0 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
                break;
            case '?': // Question Block
                //DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 1 * nTileWidth, 1 * nTileHeight, nTileWidth, nTileHeight);
                break;
            case 'o': // Coin
                //Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_CYAN);
                //DrawPartialSprite(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, spriteTiles, 3 * nTileWidth, 0 * nTileHeight, nTileWidth, nTileHeight);
                break;
            default:
                //Fill(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, PIXEL_SOLID, FG_BLACK);
                break;
            }
        }
    }

    // Draw Player
    //Fill((fPlayerPosX - fOffsetX) * nTileWidth,
    //(fPlayerPosY - fOffsetY) * nTileWidth,
    //(fPlayerPosX - fOffsetX + 1.0f) * nTileWidth,
    //(fPlayerPosY - fOffsetY + 1.0f) * nTileHeight, PIXEL_SOLID, FG_GREEN);
    QGraphicsRectItem* rItem = new QGraphicsRectItem();
    rItem->setRect(0,0, ((m_game.fPlayerPosX - fOffsetX + 1.0f) * nTileWidth) - ((m_game.fPlayerPosX - fOffsetX) * nTileWidth),
                   ((m_game.fPlayerPosY - fOffsetY + 1.0f) * nTileHeight) - ((m_game.fPlayerPosY - fOffsetY) * nTileWidth));
    rItem->setPos((m_game.fPlayerPosX - fOffsetX) * nTileWidth, (m_game.fPlayerPosY - fOffsetY) * nTileWidth);
    rItem->setBrush(QBrush(Qt::green));
    rItem->setPen(QPen(Qt::red));
    addItem(rItem);

    //DrawPartialSprite((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, spriteMan, nDirModX * nTileWidth, nDirModY * nTileHeight, nTileWidth, nTileHeight);
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
    }
    QGraphicsScene::keyReleaseEvent(event);
}
