#include "shadows.h"

shadows::shadows()
{
    sh1 = new QGraphicsDropShadowEffect;
    create_shadow(*sh1);
    sh2 = new QGraphicsDropShadowEffect;
    create_shadow(*sh2);
    sh3 = new QGraphicsDropShadowEffect;
    create_shadow(*sh3);
    sh4 = new QGraphicsDropShadowEffect;
    create_shadow(*sh4);
    sh5 = new QGraphicsDropShadowEffect;
    create_shadow(*sh5);
    sh6 = new QGraphicsDropShadowEffect;
    create_shadow(*sh6);
    sh7 = new QGraphicsDropShadowEffect;
    create_shadow(*sh7);
    sh8 = new QGraphicsDropShadowEffect;
    create_shadow(*sh8);
    sh9 = new QGraphicsDropShadowEffect;
    create_shadow(*sh9);
    sh10 = new QGraphicsDropShadowEffect;
    create_shadow(*sh10);
    sh11 = new QGraphicsDropShadowEffect;
    create_shadow(*sh11);
    sh12 = new QGraphicsDropShadowEffect;
    create_shadow(*sh12);
    sh13 = new QGraphicsDropShadowEffect;
    create_shadow(*sh13);
    sh14 = new QGraphicsDropShadowEffect;
    create_shadow(*sh14);
    sh15 = new QGraphicsDropShadowEffect;
    create_shadow(*sh15);
    sh16 = new QGraphicsDropShadowEffect;
    create_shadow(*sh16);
    sh17 = new QGraphicsDropShadowEffect;
    create_shadow(*sh17);
    sh18 = new QGraphicsDropShadowEffect;
    create_shadow(*sh18);
    sh19 = new QGraphicsDropShadowEffect;
    create_shadow(*sh19);
    sh20 = new QGraphicsDropShadowEffect;
    create_shadow(*sh20);
    sh20 = new QGraphicsDropShadowEffect;
    create_shadow(*sh20);
    sh21 = new QGraphicsDropShadowEffect;
    create_shadow(*sh21);
    sh22 = new QGraphicsDropShadowEffect;
    create_shadow(*sh22);
    sh23 = new QGraphicsDropShadowEffect;
    create_shadow(*sh23);
    sh24 = new QGraphicsDropShadowEffect;
    create_shadow(*sh24);
}

shadows::~shadows()
{
    delete sh1;
    delete sh2;
    delete sh3;
    delete sh4;
    delete sh5;
    delete sh6;
    delete sh7;
    delete sh8;
    delete sh9;
    delete sh10;
    delete sh11;
    delete sh12;
    delete sh13;
    delete sh14;
    delete sh15;
    delete sh16;
    delete sh17;
    delete sh18;
    delete sh19;
    delete sh20;
}

void shadows::create_shadow(QGraphicsDropShadowEffect& shadow)
{
    shadow.setBlurRadius(10);
    shadow.setXOffset(2);
    shadow.setYOffset(2);
    shadow.setColor(Qt::black);
}

void shadows::disable_shadow(QGraphicsDropShadowEffect &shadow)
{
    shadow.setBlurRadius(0);
    shadow.setXOffset(0);
    shadow.setYOffset(0);
    shadow.setColor(Qt::transparent);
}



void shadows::turn_off_shadows()
{
    disable_shadow(*sh1);
    disable_shadow(*sh2);
    disable_shadow(*sh3);
    disable_shadow(*sh4);
    disable_shadow(*sh5);
    disable_shadow(*sh6);
    disable_shadow(*sh7);
    disable_shadow(*sh8);
    disable_shadow(*sh9);
    disable_shadow(*sh10);
    disable_shadow(*sh11);
    disable_shadow(*sh12);
    disable_shadow(*sh13);
    disable_shadow(*sh14);
    disable_shadow(*sh15);
    disable_shadow(*sh16);
    disable_shadow(*sh17);
    disable_shadow(*sh18);
    disable_shadow(*sh19);
    disable_shadow(*sh20);
}

void shadows::turn_on_shadows()
{
    create_shadow(*sh1);
    create_shadow(*sh2);
    create_shadow(*sh3);
    create_shadow(*sh4);
    create_shadow(*sh5);
    create_shadow(*sh6);
    create_shadow(*sh7);
    create_shadow(*sh8);
    create_shadow(*sh9);
    create_shadow(*sh10);
    create_shadow(*sh11);
    create_shadow(*sh12);
    create_shadow(*sh13);
    create_shadow(*sh14);
    create_shadow(*sh15);
    create_shadow(*sh16);
    create_shadow(*sh17);
    create_shadow(*sh18);
    create_shadow(*sh19);
    create_shadow(*sh20);
}
