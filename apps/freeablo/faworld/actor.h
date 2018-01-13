
#pragma once

#include "../farender/animationplayer.h"
#include "actoranimationmanager.h"
#include "actorstats.h"
#include "behaviour.h"
#include "faction.h"
#include "gamelevel.h"
#include "inventory.h"
#include "movementhandler.h"
#include "position.h"
#include "target.h"
#include "world.h"
#include <boost/format.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <map>
#include <misc/misc.h>
#include <statemachine/statemachine.h>

namespace FASaveGame
{
    class Loader;
    class Saver;
}

namespace FAWorld
{
    class Behaviour;
    class World;

    class Actor
    {
    public:
        Actor(const std::string& walkAnimPath = "", const std::string& idleAnimPath = "", const std::string& dieAnimPath = "");
        Actor(const DiabloExe::Npc& npc, const DiabloExe::DiabloExe& exe);
        Actor(const DiabloExe::Monster& monster);
        Actor(FASaveGame::GameLoader& loader, const DiabloExe::DiabloExe& exe);
        virtual ~Actor();

        virtual void save(FASaveGame::GameSaver& saver);

        static const std::string typeId;
        virtual const std::string& getTypeId() { return typeId; }

        void pickupItem(Target::ItemTarget target);

        void teleport(GameLevel* level, Position pos);
        GameLevel* getLevel();

        void attack(Actor* enemy);

        std::string getDieWav() const;
        std::string getHitWav() const;

        bool canIAttack(Actor* actor);
        virtual void update(bool noclip);
        void init();
        void takeDamage(double amount);
        void heal();

        void die();
        bool isDead() const;
        bool isEnemy(Actor* other) const;

        const std::map<std::string, std::string>& getTalkData() const { return mTalkData; }
        const std::string& getNpcId() const { return mNpcId; }
        const std::string& getName() const { return mName; }
        const ActorStats& getStats() const { return mStats; }
        const Position& getPos() const { return mMoveHandler.getCurrentPosition(); }
        int32_t getId() const { return mId; }
        bool isPassable() const { return isDead(); }
        bool hasTarget() const;

        bool canTalk() const { return mTalkData.size() > 0; }
        bool canInteractWith(Actor* actor);

        // public member variables
        MovementHandler mMoveHandler;
        Target mTarget;
        ActorAnimationManager mAnimation;
        bool isAttacking = false;
        bool mInvuln = false;
        Inventory mInventory;

    protected:
        // protected member variables
        StateMachine::StateMachine<Actor>* mActorStateMachine;
        ActorStats mStats;
        std::string mSoundPath;
        Behaviour* mBehaviour = nullptr;
        Faction mFaction;
        std::string mName; ///< Name as it appears in-game
        int32_t mId = -1;
        std::map<std::string, std::string> mTalkData; ///< Lines of dialogue

        // TODO: this var is only used for dialog code, which branches on which npc is being spoken to.
        // Eventually, we should add a proper dialog specification system, and get rid of this.
        std::string mNpcId;
    };
}
