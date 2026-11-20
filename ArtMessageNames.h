#pragma once
// ART turns a message name into UID with NMutils::hashString (an sdbm variant) this mirrors it
#include "NmRsInvokeUIDs.h"
#include <cstdint>
#include <cstring>
namespace ArtMessages {

// Same as NMutils::hashString hash = c + some math of 6 and 6 turn into 16 or something I forgot how to do math
inline uint32_t HashString(const char* str) {
    if (!str) return 0;
    uint32_t h = 0;
    for (const char* c = str; *c; ++c)
        h = (uint32_t)(int)(signed char)*c + (h << 6) + (h << 16) - h;
    return h;
}
static const char* const kNames[] = {
    "activePose",
    "addPatch",
    "animPose",
    "applyBulletImpulse",
    "applyImpulse",
    "armHang",
    "armsWindmill",
    "armsWindmillAdaptive",
    "balancerCollisionsReaction",
    "bodyBalance",
    "bodyFoetal",
    "bodyRelax",
    "bodyRollUp",
    "bodyWrithe",
    "braceForImpact",
    "buoyancy",
    "carried",
    "catchFall",
    "configureBalance",
    "configureBalanceReset",
    "configureBullets",
    "configureBulletsExtra",
    "configureConstraints",
    "configureExtraBullets",
    "configureLimits",
    "configureSelfAvoidance",
    "configureShotInjuredArm",
    "configureShotInjuredLeg",
    "configureSoftLimit",
    "dangle",
    "debugRig",
    "debugSkeleton",
    "defineAttachedObject",
    "dragged",
    "electrocute",
    "fallOverWall",
    "fireWeapon",
    "forceLeanInDirection",
    "forceLeanRandom",
    "forceLeanToPosition",
    "forceLeanTowardsObject",
    "forceToBodyPart",
    "grab",
    "hardConstraint",
    "headLook",
    "highFall",
    "hipsLeanInDirection",
    "hipsLeanRandom",
    "hipsLeanToPosition",
    "hipsLeanTowardsObject",
    "incomingTransforms",
    "injuredOnGround",
    "landing",
    "lastManStanding",
    "leanInDirection",
    "leanRandom",
    "leanToPosition",
    "leanTowardsObject",
    "learnedCrawl",
    "onFire",
    "overrideStartingVelocity",
    "pedalLegs",
    "pointArm",
    "pointGun",
    "pointGunExtra",
    "registerWeapon",
    "relaxUnwind",
    "rollDownStairs",
    "setCharacterCollisions",
    "setCharacterDamping",
    "setCharacterHealth",
    "setCharacterStrength",
    "setCharacterUnderwater",
    "setFallingReaction",
    "setFrictionScale",
    "setMuscleStiffness",
    "setStiffness",
    "setWeaponMode",
    "shootDodge",
    "shot",
    "shotConfigureArms",
    "shotFallToKnees",
    "shotFromBehind",
    "shotHeadLook",
    "shotInGuts",
    "shotNewBullet",
    "shotRelax",
    "shotShockSpin",
    "shotSnap",
    "smartFall",
    "softKeyframe",
    "splitBody",
    "staggerFall",
    "stayUpright",
    "stopAllBehaviours",
    "stumble",
    "teeter",
    "upperBodyFlinch",
    "yanked"
};
static const int kNameCount = (int)(sizeof(kNames) / sizeof(kNames[0]));
// BSpy marks messages with a prefix "#" = blocked, "D_" = delayed returns the bare name
inline const char* StripPrefix(const char* name) {
    if (name[0] == '#') return name + 1;
    if (name[0] == 'D' && name[1] == '_') return name + 2;
    return name;
}

inline bool IsKnownName(const char* name) {
    const char* n = StripPrefix(name);
    for (int i = 0; i < kNameCount; ++i)
        if (strcmp(kNames[i], n) == 0) return true;
    return false;
}

// UID then name for a directInvoke hook returns nullptr if the UID is not a known message
inline const char* NameFromUid(uint32_t uid) {
    if (uid == (uint32_t)nmInvoke_StopAllBehaviours) return "nmInvoke_StopAllBehaviours";
    for (int i = 0; i < kNameCount; ++i)
        if (HashString(kNames[i]) == uid) return kNames[i];
    return nullptr;
}

}
