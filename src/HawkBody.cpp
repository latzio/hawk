/*
 * HawkBody.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: nitz
 */

#include "HawkBody.h"

void HawkBody::createBody(const HawkPoint& point, Dynamics dynamics)
{
    if (m_body)
        destroyBody();

    b2BodyDef def;
    def.type = static_cast<b2BodyType>(dynamics);
    def.position = Hawk::toMeters(point);

    m_body = m_world->CreateBody(&def);
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

