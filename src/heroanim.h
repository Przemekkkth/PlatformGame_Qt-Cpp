#ifndef HEROANIM_H
#define HEROANIM_H

#include <QObject>
#include <QPixmap>
#include <QTimer>

class HeroAnim : public QObject
{
    Q_OBJECT
public:
    explicit HeroAnim(QObject *parent = nullptr);
    QPixmap currentPixmap() const;
    void setMoveRight(bool val);
    void setMoveLeft(bool val);
    void setJump(bool val);
signals:

private slots:
    void updatePixmap();

private:
    const int COUNT_OF_WALK_FRAMES;
    const QPoint L_JUMP_P;
    const QPoint R_JUMP_P;
    int m_currentFrame;
    QPixmap m_pixmap;
    QPixmap m_currentPixmap;
    bool m_isMoveLeft, m_isMoveRight, m_isJump;
    QTimer m_timer;
};

#endif // HEROANIM_H
