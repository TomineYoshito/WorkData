#pragma once
#include "Dxlib.h"
#include <vector>
#include "AnimationClip.h"

#define ANIM_PLAY_START (1)
#define ANIM_PLAY (2)
#define ANIM_PLAY_NEUTRAL (3)
#define ANIM_MOTION_BLEND_START (4)
#define ANIM_MOTION_BLEND_PLAY (5)


// AnimatorÉNÉâÉX
class Animator {
public:
    Animator();
    ~Animator();

    void Load(vector <AnimationInfo> animInfo);
    void Load(int MHandle, vector <AnimationInfo> animInfo);
    void Load(int MHandle, vector <AnimationInfo> animInfo, int num);
    void Play(int MHandle, int anim_id);
    void SetMonster(int monster) {
        Monster = monster;
    }
    bool Update();

    float GetPlayTime();
    void SetPlaySpeed(float speed);

    void SetAnimationClip(AnimationClip* clip_ptr) {
        animClipVec.push_back(clip_ptr);
    }

    vector<AnimationClip*> GetAnimationClips() {
        return animClipVec;
    }

    void CreateAnimInstance(Animator* srcAnimator);

    bool GetAttackFrag() {
        return attackFrag;
    }

private:
    vector <AnimationClip*> animClipVec;

    float playTime;
    float playSpeed;
    float totalTime;
    int attachIndex;
    int attachIndex2;
    int playAnimId;
    bool loopFlag;
    int modelHandle;
    int playAnimHandle;

    int Monster;

    float blendRate;
    int playStatus;					// çƒê∂èÛë‘
    bool playLoop;

    bool bundleAnim;
    bool attackFrag;
};
