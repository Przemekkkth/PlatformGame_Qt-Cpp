#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "gamescene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View();

signals:

private:
    GameScene* m_gameScene;
};

#endif // VIEW_H
