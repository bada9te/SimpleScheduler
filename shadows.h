#ifndef SHADOWS_H
#define SHADOWS_H

#include <QGraphicsDropShadowEffect>

class shadows
{
public:
    shadows();
    ~shadows();

    void create_shadow(QGraphicsDropShadowEffect& shadow);
    void disable_shadow(QGraphicsDropShadowEffect& shadow);
    void turn_off_shadows();
    void turn_on_shadows();

    QGraphicsDropShadowEffect* sh1;
    QGraphicsDropShadowEffect* sh2;
    QGraphicsDropShadowEffect* sh3;
    QGraphicsDropShadowEffect* sh4;
    QGraphicsDropShadowEffect* sh5;
    QGraphicsDropShadowEffect* sh6;
    QGraphicsDropShadowEffect* sh7;
    QGraphicsDropShadowEffect* sh8;
    QGraphicsDropShadowEffect* sh9;
    QGraphicsDropShadowEffect* sh10;
    QGraphicsDropShadowEffect* sh11;
    QGraphicsDropShadowEffect* sh12;
    QGraphicsDropShadowEffect* sh13;
    QGraphicsDropShadowEffect* sh14;
    QGraphicsDropShadowEffect* sh15;
    QGraphicsDropShadowEffect* sh16;
    QGraphicsDropShadowEffect* sh17;
    QGraphicsDropShadowEffect* sh18;
    QGraphicsDropShadowEffect* sh19;
    QGraphicsDropShadowEffect* sh20;
    QGraphicsDropShadowEffect* sh21;
    QGraphicsDropShadowEffect* sh22;
    QGraphicsDropShadowEffect* sh23;
    QGraphicsDropShadowEffect* sh24;
};

#endif // SHADOWS_H
