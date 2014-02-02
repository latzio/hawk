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

struct HawkBodyDef {
    b2World* world;
};

class HawkBody {
public:
    /**
     * Root class of HawkBody instances that interact with Box2D.
     * A HawkBody owns a b2Body and wraps it for the HawkEngine.
     *
     * All HawkBody operations are performed in SI Units, such as meters and seconds.
     *
     */
    HawkBody(const HawkBodyDef& def)
        : m_world(def.world)
        , m_body(0)
    {
        ASSERT(m_world);
    }
    virtual ~HawkBody() { }

    virtual void createBody(const HawkPoint&);
    void destroyBody();

    void createSprite(const char* path);

    Sprite* sprite() { return &m_sprite; }

    void draw() { m_sprite.draw(); }
    b2Body* body() { return m_body; }

    float width() const { return m_sprite.Width(); }
    float height() const { return m_sprite.Height(); }

    void createFixtureFromSprite();

protected:
    virtual b2BodyType bodyType() const { return b2_staticBody; }

protected:
    b2World* m_world;
    b2Body* m_body;
    Sprite m_sprite;
};

struct DynamicHawkBodyDef : HawkBodyDef {
    HawkVector speed;
    HawkVector burst;
    bool fixedRotation;
};

class DynamicHawkBody : public HawkBody {
public:
    DynamicHawkBody(const DynamicHawkBodyDef& def)
        : HawkBody(def)
        , m_horizontal(Coast)
        , m_vertical(Coast)
        , m_speed(def.speed)
        , m_burst(def.burst)
        , m_fixedRotation(def.fixedRotation)
    { }
    virtual ~DynamicHawkBody() { }

    // Movement this object should take when applyImpulses is called on its next update.
    // Note that bursts are reset to Coast when processed, but cruises remain until updated manually.
    enum Movement {
        Coast, // Slide/glide as gravity and friction dictate. Rest idle.
        Stop, // Actively try to slow down and stop.
        NegativeCruise, // Move leftward or downward.
        PositiveCruise, // Move rightward or upward.
        NegativeBurst, // Burst leftward or downward.
        PositiveBurst, // Burst rightward or upward.
    };

    virtual void createBody(const HawkPoint&);
    void applyImpulses();

    void setHorizontalMovement(Movement movement) { m_horizontal = movement; }
    void setVerticalMovement(Movement movement) { m_vertical = movement; }

private:
    virtual b2BodyType bodyType() const { return b2_dynamicBody; }

    Movement m_horizontal;
    Movement m_vertical;

    // Speed is maximum crusing speed when moving constantly.
    // Burst is used when moving once such as a jump or roll.
    const HawkVector m_speed;
    const HawkVector m_burst;
    const bool m_fixedRotation;
};

#endif /* HAWKBODY_H_ */
