/*
 * HawkEngine.h
 *
 *  Created on: Feb 1, 2014
 *      Author: nitz
 */

#ifndef HAWKENGINE_H_
#define HAWKENGINE_H_

#ifndef NDEBUG
#include <assert.h>
#define ASSERT(a) (assert(a))
#else
#define ASSERT(a)
#endif

#include "Box2D/Box2D.h"

typedef b2Vec2 HawkPoint;
typedef b2Vec2 HawkVector;
typedef b2Vec3 HawkVector3D;

class Hawk {
public:
    static float pix2M(int pixels) { return pixels * 0.01f; }
    static float pix2M(float pixels) { return pixels * 0.01f; }

    static int m2Pix(int meters) { return meters * 100; }
    static float m2Pix(float meters) { return meters * 100; }

    static HawkPoint toMeters(const HawkPoint& other) { return HawkPoint(pix2M(other.x), pix2M(other.y)); }
    static HawkPoint toPixels(const HawkPoint& other) { return HawkPoint(m2Pix(other.x), m2Pix(other.y)); }

private:
    Hawk();
};

#endif /* HAWKENGINE_H_ */
