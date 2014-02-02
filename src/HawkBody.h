/*
 * HawkBody.h
 *
 *  Created on: Feb 1, 2014
 *      Author: nitz
 */

#ifndef HAWKBODY_H_
#define HAWKBODY_H_

#include "HawkEngine.h"

#include "Sprite.h"

class HawkBody {
public:
    /**
     * Root class of HawkBody instances that interact with Box2D.
     * A HawkBody owns a b2Body and wraps it for the HawkEngine.
     *
     * All HawkBody operations are performed in SI Units, such as meters and seconds.
     *
     */
    HawkBody(b2World* world)
        : m_world(world)
        , m_body(0)
    {
        ASSERT(world);
    }

    enum Dynamics {
        Dynamic = b2_dynamicBody,
        Static = b2_staticBody,
    };

    void createBody(const HawkPoint&, Dynamics);
    void destroyBody();

    void createSprite(const char* path);

    Sprite* sprite() { return &m_sprite; }

    void draw() { m_sprite.draw(); }
    b2Body* body() { return m_body; }

    float width() const { return m_sprite.Width(); }
    float height() const { return m_sprite.Height(); }

    void createFixtureFromSprite();

private:
    b2World* m_world;
    b2Body* m_body;
    Sprite m_sprite;
};

#endif /* HAWKBODY_H_ */
