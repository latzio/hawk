/*
 * HawkBody.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: nitz
 */

#include "HawkBody.h"

void HawkBody::createBody(const HawkPoint& point)
{
    if (m_body)
        destroyBody();

    b2BodyDef def;
    def.type = bodyType();
    def.position = Hawk::toMeters(point);

    m_body = m_world->CreateBody(&def);
    m_body->SetUserData(this);
}

void HawkBody::destroyBody()
{
    if (m_body)
        m_world->DestroyBody(m_body);

    m_body = 0;
}

void HawkBody::createSprite(const char* path)
{
    m_sprite.load(path);
}

void HawkBody::createFixtureFromSprite()
{
    // Shave off an extra couple of pixels to make the objects appear to contact a little
    // more fully. Otherwise you can clearly see blank pixels between them.
    HawkVector fixtureSize(Hawk::pix2M(width() - 2.f), Hawk::pix2M(height() - 2.f));
    if (fixtureSize.x <= 0 || fixtureSize.y <= 0)
        return;

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(fixtureSize.x / 2, fixtureSize.y / 2);

    b2FixtureDef def;
    def.shape = &dynamicBox;
    def.density = 5.0f;
    def.friction = 0.7f;

    m_body->CreateFixture(&def);
}

// This method must write to desired.
// It always set Burst back to Coast, Stop to coast once stopped.
static void processSingleAxisOfMovement(const float speed, const float burst, const float current, float& desired, DynamicHawkBody::Movement& movement)
{
    switch (movement) {
    case DynamicHawkBody::Coast:
        desired = current;
        break;
    case DynamicHawkBody::Stop:
        desired = -(current / 4.f);
        if (fabs(desired) < 0.01)
            movement = DynamicHawkBody::Coast;
        break;
    case DynamicHawkBody::NegativeCruise:
        desired = -speed;
        break;
    case DynamicHawkBody::PositiveCruise:
        desired = speed;
        break;
    case DynamicHawkBody::NegativeBurst:
        desired = -burst;
        movement = DynamicHawkBody::Coast;
        break;
    case DynamicHawkBody::PositiveBurst:
        desired = burst;
        movement = DynamicHawkBody::Coast;
        break;
    }
}

void DynamicHawkBody::applyImpulses()
{
    HawkVector currentVelocity = m_body->GetLinearVelocity();
    HawkVector desiredVelocity;

    processSingleAxisOfMovement(m_speed.x, m_burst.x, currentVelocity.x, desiredVelocity.x, m_horizontal);
    processSingleAxisOfMovement(m_speed.y, m_burst.y, currentVelocity.y, desiredVelocity.y, m_vertical);

    if (desiredVelocity.LengthSquared()) {
        HawkVector impulse = (desiredVelocity - currentVelocity);
        impulse.x *= m_body->GetMass();
        impulse.y *= m_body->GetMass();
        m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter(), true);
    }
}


