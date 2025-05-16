#pragma once
#include "Dxlib.h"
#include <math.h>
#include <string>
#include <vector>

using namespace std;

typedef struct _AnimationInfo {
    int id;
    string fileName;
    bool loopFlag;
} AnimationInfo;

// AnimationClipƒNƒ‰ƒX
class AnimationClip {
public:
    AnimationClip();
    ~AnimationClip();

    void Load(string fileName);

    int GetAnimHandle() {
        return animHandle;
    }

    void SetAnimHandle(int handle) {
        animHandle = handle;
    }

    bool GetLoopFlag() {
        return loopFlag;
    }

    void SetLoopFlag(bool flag) {
        loopFlag = flag;
    }


    bool loopFlag;

private:
    int animHandle;
};


