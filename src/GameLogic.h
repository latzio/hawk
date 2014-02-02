/*
 * Copyright 2011-2012 Research In Motion Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file GameLogic.h
 *
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include "Platform.h"
#include "Sound.h"
#include "bbutil.h"
#include "Sprite.h"

#include <math.h>
#include "Box2D/Box2D.h"

#include <GLES/gl.h>

#ifndef NDEBUG
#include <assert.h>
#define ASSERT(a) (assert(a))
#else
#define ASSERT(a)
#endif

namespace blocks {

class ContactListener : public b2ContactListener {
public:
    ContactListener(const Sound& sound) : m_sound(sound){};
    virtual ~ContactListener(){};

    virtual void BeginContact(b2Contact* contact) {
        m_sound.play();
    }
    virtual void EndContact(b2Contact* contact) {};
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};
private:
    const Sound& m_sound;
};

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

class GameLogic : public HawkInputHandler {
public:
    GameLogic(Platform& platform);
    virtual ~GameLogic(){};
    void run();

private:
    Platform& m_platform;
    bool m_shutdown;
    bool m_gamePaused;
    bool m_gameFinished;

    enum GameState { FetchUser, GamePlay, LeaderBoard };
    GameState m_state;

    font_t* m_font;
    font_t* m_scoreFont;
    font_t* m_leaderboardFont;

    float m_sceneWidth, m_sceneHeight;

    float m_velocityTreshold;
    long m_angstCountdown;

    Sprite m_background;
    Sprite m_leaderBoard;
    Sprite m_smallBlockBelligerent;
    Sprite m_smallBlockResting;
    Sprite m_largeBlockBelligerent;
    Sprite m_largeBlockResting;
    Sprite m_buttonPressed;
    Sprite m_buttonRegular;

    float m_blocksToAddYOffset;

    struct button {
        float posX;
        float posY;
        float sizeX;
        float sizeY;
        bool isPressed;
        Sprite* regular;
        Sprite* pressed;
        float textX;
        float textY;
        font_t* font;
        const char* text;

        void draw() const;

        void setPosition(float x, float y);

        bool isWithin(float clickX, float clickY) const {
            return ((clickX >= posX - sizeX / 2) && (clickX <= posX + sizeX / 2) && (clickY >= posY - sizeX / 2) && (clickY <= posY + sizeY / 2));
        }
    };

    button m_playButton;

    bool m_showClock;
    int m_time;
    int m_scoreTime;
    time_t m_resumeTime;
    int m_score;
    float m_scorePosX, m_scorePosY, m_timerPosX, m_timerPosY;
    const char* m_message;
    float m_messagePosX, m_messagePosY;

    bool m_leaderBoardReady;
    std::vector<Score> m_leaderboard;

    //Box2D things
    float m_timeStep;
    int m_velocityIterations, m_positionIterations;
    b2World m_world;
    b2Body* m_groundBody;
    HawkBody m_ground;
    HawkBody m_player;

    virtual void onLeftPress(float x, float y);
    virtual void onLeftRelease(float x, float y);
    virtual void onExit();
    virtual void onPause();
    virtual void onResume();
    virtual void onSubmitScore();
    virtual void onLeaderboardReady(const std::vector<Score>& leaderboard);
    virtual void onUserReady(const std::string& userName, bool isAnonymous, const std::string& errorString);
    virtual void onPromptOk(const std::string& input);
    virtual void onControlStarted(HawkControl);
    virtual void onControlStopped(HawkControl);

    void endGamePlay(bool win);
    void reset();
    void update();
    void renderFetchUser();
    void renderGame();
    void renderLeadBoard();
    void enable2D();
    void addNextShape(float x, float y);

    Sound m_backgroundMusic;
    Sound m_click1;
    Sound m_click2;
    Sound m_clickReverb;
    Sound m_blockFall;
    ContactListener m_contactListener;
};

} /* namespace blocks */
#endif /* GAMELOGIC_H_ */
