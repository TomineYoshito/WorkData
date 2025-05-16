#include "AnimationClip.h"

AnimationClip::AnimationClip()
{

}

AnimationClip::~AnimationClip()
{

}

void AnimationClip::Load(string fileName)
{
    animHandle = MV1LoadModel(fileName.c_str());
}
