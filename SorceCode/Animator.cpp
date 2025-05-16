
#include "Animator.h"

Animator::Animator()
{
    playTime = 0.f;
    playSpeed = 1.f;
    totalTime = 0.f;
    attachIndex = 0;
    playAnimId = -1;
    bundleAnim = false;
}

Animator::~Animator()
{
    animClipVec.clear();
    animClipVec.shrink_to_fit();
}


void Animator::CreateAnimInstance(Animator* srcAnimator)
{
    vector<AnimationClip*>srcAnimClip = srcAnimator->GetAnimationClips();

    vector<AnimationClip*>::iterator it = srcAnimClip.begin();
    while (it != srcAnimClip.end()) {
        AnimationClip* clipPtr = *it;

        AnimationClip* instanceAnimClipPtr = new AnimationClip();
        int AnimHandle = MV1DuplicateModel(clipPtr->GetAnimHandle());
        instanceAnimClipPtr->SetAnimHandle(AnimHandle);
        instanceAnimClipPtr->SetLoopFlag(clipPtr->GetLoopFlag());

        animClipVec.push_back(instanceAnimClipPtr);
        ++it;
    }
}


void Animator::Load(int MHandle, vector <AnimationInfo> animInfo)
{
    int num_of_anim = MV1GetAnimNum(MHandle);
    bundleAnim = true;
    for (int i = 0; i < num_of_anim; i++) {
        AnimationClip* animClipPtr = new AnimationClip();
        animClipPtr->SetLoopFlag(animInfo[i].loopFlag);
        // AnimationClipのアニメーションハンドルは全てモデルハンドルをセット
        animClipPtr->SetAnimHandle(MHandle);
        animClipVec.push_back(animClipPtr);
    }
}


void Animator::Load(int MHandle, vector <AnimationInfo> animInfo,int num)
{
    int num_of_anim = num;
    bundleAnim = true;
    for (int i = 0; i < num_of_anim; i++) {
        AnimationClip* animClipPtr = new AnimationClip();
        animClipPtr->SetLoopFlag(animInfo[i].loopFlag);
        // AnimationClipのアニメーションハンドルは全てモデルハンドルをセット
        animClipPtr->SetAnimHandle(MHandle);
        animClipVec.push_back(animClipPtr);
    }
}


void Animator::Load(vector <AnimationInfo> animInfo)
{
    for (int i = 0; i < animInfo.size(); i++) {
        AnimationClip* animClipPtr = new AnimationClip();
        animClipPtr->SetLoopFlag(animInfo[i].loopFlag);
        animClipPtr->Load(animInfo[i].fileName);
        animClipVec.push_back(animClipPtr);
    }
}


void Animator::Play(int MHandle, int anim_id)
{
    if (playAnimId != anim_id) {
        //playTime = 0.f;
        //playAnimId = anim_id;
        //MV1DetachAnim(MHandle, attachIndex);
        modelHandle = MHandle;
        //attachIndex = MV1AttachAnim(MHandle, 0, animClipVec[anim_id]->GetAnimHandle(), FALSE);
        //totalTime = MV1GetAttachAnimTotalTime(MHandle, attachIndex);
        //MV1SetAttachAnimTime(MHandle, attachIndex, playTime);

        if (playStatus == ANIM_PLAY || playStatus == ANIM_PLAY_NEUTRAL) {
            // モーションブレンド指示
            playStatus = ANIM_MOTION_BLEND_START;
        }
        else if (playStatus == ANIM_MOTION_BLEND_PLAY) {
            // モーションブレンド中にアニメーション変更指示が来た
            playStatus = ANIM_PLAY_START;
            MV1DetachAnim(modelHandle, attachIndex2);	// ブレンド中のアニメーションをデタッチ

            MV1DetachAnim(modelHandle, attachIndex);
            if (!bundleAnim) {
                // アニメーションファイルが分割ロードされている場合のアニメーションアタッチ
                attachIndex = MV1AttachAnim(modelHandle, 0, animClipVec[anim_id]->GetAnimHandle());
            }
            else {
                // アニメーションデータがモデルファイルに結合されている場合のアニメーションアタッチ
                attachIndex = MV1AttachAnim(modelHandle, anim_id, animClipVec[anim_id]->GetAnimHandle());
            }
            totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
            playStatus = ANIM_PLAY;	// 再生中へ
            playTime = 0.0f;
        }
        else {
            // 再生指示
            playStatus = ANIM_PLAY_START;
        }
        if (bundleAnim) {
            if (Monster == 1) {
                if (anim_id == 0) {
                    attackFrag = true;
                }
                else {
                    attackFrag = false;
                }
            }
            if (Monster == 2) {
                if (anim_id == 2) {
                    attackFrag = true;
                }
                else {
                    attackFrag = false;
                }
            }
        }
        // 再生指示された情報をクラスの広域変数へセット
        playAnimHandle = animClipVec[anim_id]->GetAnimHandle();
        playAnimId = anim_id;
        playLoop = animClipVec[anim_id]->GetLoopFlag();
    }
}


bool Animator::Update()
{
    void SetPlaySpeed();
    bool result = true;
    switch (playStatus) {
    case ANIM_MOTION_BLEND_START: // モーションブレンド開始
        if (!bundleAnim) {
            // アニメーションファイルが分割ロードされている場合のアニメーションアタッチ
            attachIndex2 = MV1AttachAnim(modelHandle, 0, playAnimHandle);
        }
        else {
            // アニメーションデータがモデルファイルに結合されている場合のアニメーションアタッチ
            attachIndex2 = MV1AttachAnim(modelHandle, playAnimId, playAnimHandle);
        }
        blendRate = 0.0f;
        playStatus = ANIM_MOTION_BLEND_PLAY;
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
        break;

    case ANIM_MOTION_BLEND_PLAY: // モーションブレンド中
        blendRate += 0.1f;
        if (blendRate >= 1.0f) {
            // モーションブレンド終了
            MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
            MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
            MV1DetachAnim(modelHandle, attachIndex);
            playStatus = ANIM_PLAY;			// 再生中へ
            playTime = 0.0f;
            attachIndex = attachIndex2;
            totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
            break;
        }

        MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
        break;

    case ANIM_PLAY_START:	// 通常再生開始
        //anim_ptr = PlayAnimPtr;
        MV1DetachAnim(modelHandle, attachIndex);
        if (!bundleAnim) {
            // アニメーションファイルが分割ロードされている場合のアニメーションアタッチ
            attachIndex = MV1AttachAnim(modelHandle, 0, playAnimHandle);
        }
        else {
            // アニメーションデータがモデルファイルに結合されている場合のアニメーションアタッチ
            attachIndex = MV1AttachAnim(modelHandle, playAnimId, playAnimHandle);
        }
        totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
        playStatus = ANIM_PLAY;	// 再生中へ
        playTime = 0.0f;
        break;

    case ANIM_PLAY:	// 再生中

        playTime += playSpeed;
        if (playTime > totalTime) {

            playTime = 0.0f;  // 繰り返し
            if (playLoop == FALSE) {
                // ループ再生ではない
                playStatus = ANIM_PLAY_NEUTRAL;	// ニュートラルへ
                break;
            }
        }

        MV1SetAttachAnimTime(modelHandle, attachIndex, playTime);  // モーションタイムのセット（タイムラインを進める）
        break;

    case ANIM_PLAY_NEUTRAL:	// 再生終了しニュートラルへ
        if (bundleAnim) {
            if (Monster == 1) {
                Play(modelHandle, 2);
            }
            else if (Monster == 2) {
                Play(modelHandle, 1);
            }
            else {
                Play(modelHandle, 2);
            }
        }
        else {
            Play(modelHandle, 0);
        }
        
        result = false;
        break;
    }

    return result;
}

/**
* @brief 現在のアニメーション再生時間を取得
* @param[in] なし
* @param[out] なし
* @return なし
* @details 現在再生中のアニメーションの再生時間を取得します。
*/
float Animator::GetPlayTime()
{
    return playTime;
}

/**
* @brief アニメーション再生スピードをセット
* @param[in] speed（再生スピード）
* @param[out] なし
* @return なし
* @details 再生するアニメーションスピードをセットします。
*/
void Animator::SetPlaySpeed(float speed)
{
    playSpeed = speed;
}