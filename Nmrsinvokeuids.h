#ifndef NM_RS_INVOKEUIDS_H
#define NM_RS_INVOKEUIDS_H

/**
 * This file contains a list of something UIDs that can be passed to the directInvoke() function
 * of NmRsEngine share this list with RAGE / game side
 */

enum
{
    nmInvoke_Start = 0x1000,   // do not change or else the UIDs will be different between RAGE and game side

    // add new invoke UIDs here "nmInvoke_<description>"

    nmInvoke_StopAllBehaviours,

    // i forgot what to put here but its something valueable 

    nmInvoke_End = 0x2000    // do not change or else you will break the UID range check in directInvoke()
};

#endif // NM_RS_INVOKEUIDS_H
// as if it wasnt known already^ 