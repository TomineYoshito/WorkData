
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
        // AnimationClip�̃A�j���[�V�����n���h���͑S�ă��f���n���h�����Z�b�g
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
        // AnimationClip�̃A�j���[�V�����n���h���͑S�ă��f���n���h�����Z�b�g
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
            // ���[�V�����u�����h�w��
            playStatus = ANIM_MOTION_BLEND_START;
        }
        else if (playStatus == ANIM_MOTION_BLEND_PLAY) {
            // ���[�V�����u�����h���ɃA�j���[�V�����ύX�w��������
            playStatus = ANIM_PLAY_START;
            MV1DetachAnim(modelHandle, attachIndex2);	// �u�����h���̃A�j���[�V�������f�^�b�`

            MV1DetachAnim(modelHandle, attachIndex);
            if (!bundleAnim) {
                // �A�j���[�V�����t�@�C�����������[�h����Ă���ꍇ�̃A�j���[�V�����A�^�b�`
                attachIndex = MV1AttachAnim(modelHandle, 0, animClipVec[anim_id]->GetAnimHandle());
            }
            else {
                // �A�j���[�V�����f�[�^�����f���t�@�C���Ɍ�������Ă���ꍇ�̃A�j���[�V�����A�^�b�`
                attachIndex = MV1AttachAnim(modelHandle, anim_id, animClipVec[anim_id]->GetAnimHandle());
            }
            totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
            playStatus = ANIM_PLAY;	// �Đ�����
            playTime = 0.0f;
        }
        else {
            // �Đ��w��
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
        // �Đ��w�����ꂽ�����N���X�̍L��ϐ��փZ�b�g
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
    case ANIM_MOTION_BLEND_START: // ���[�V�����u�����h�J�n
        if (!bundleAnim) {
            // �A�j���[�V�����t�@�C�����������[�h����Ă���ꍇ�̃A�j���[�V�����A�^�b�`
            attachIndex2 = MV1AttachAnim(modelHandle, 0, playAnimHandle);
        }
        else {
            // �A�j���[�V�����f�[�^�����f���t�@�C���Ɍ�������Ă���ꍇ�̃A�j���[�V�����A�^�b�`
            attachIndex2 = MV1AttachAnim(modelHandle, playAnimId, playAnimHandle);
        }
        blendRate = 0.0f;
        playStatus = ANIM_MOTION_BLEND_PLAY;
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
        break;

    case ANIM_MOTION_BLEND_PLAY: // ���[�V�����u�����h��
        blendRate += 0.1f;
        if (blendRate >= 1.0f) {
            // ���[�V�����u�����h�I��
            MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
            MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
            MV1DetachAnim(modelHandle, attachIndex);
            playStatus = ANIM_PLAY;			// �Đ�����
            playTime = 0.0f;
            attachIndex = attachIndex2;
            totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
            break;
        }

        MV1SetAttachAnimBlendRate(modelHandle, attachIndex, 1.0f - blendRate);
        MV1SetAttachAnimBlendRate(modelHandle, attachIndex2, blendRate);
        break;

    case ANIM_PLAY_START:	// �ʏ�Đ��J�n
        //anim_ptr = PlayAnimPtr;
        MV1DetachAnim(modelHandle, attachIndex);
        if (!bundleAnim) {
            // �A�j���[�V�����t�@�C�����������[�h����Ă���ꍇ�̃A�j���[�V�����A�^�b�`
            attachIndex = MV1AttachAnim(modelHandle, 0, playAnimHandle);
        }
        else {
            // �A�j���[�V�����f�[�^�����f���t�@�C���Ɍ�������Ă���ꍇ�̃A�j���[�V�����A�^�b�`
            attachIndex = MV1AttachAnim(modelHandle, playAnimId, playAnimHandle);
        }
        totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
        playStatus = ANIM_PLAY;	// �Đ�����
        playTime = 0.0f;
        break;

    case ANIM_PLAY:	// �Đ���

        playTime += playSpeed;
        if (playTime > totalTime) {

            playTime = 0.0f;  // �J��Ԃ�
            if (playLoop == FALSE) {
                // ���[�v�Đ��ł͂Ȃ�
                playStatus = ANIM_PLAY_NEUTRAL;	// �j���[�g������
                break;
            }
        }

        MV1SetAttachAnimTime(modelHandle, attachIndex, playTime);  // ���[�V�����^�C���̃Z�b�g�i�^�C�����C����i�߂�j
        break;

    case ANIM_PLAY_NEUTRAL:	// �Đ��I�����j���[�g������
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
* @brief ���݂̃A�j���[�V�����Đ����Ԃ��擾
* @param[in] �Ȃ�
* @param[out] �Ȃ�
* @return �Ȃ�
* @details ���ݍĐ����̃A�j���[�V�����̍Đ����Ԃ��擾���܂��B
*/
float Animator::GetPlayTime()
{
    return playTime;
}

/**
* @brief �A�j���[�V�����Đ��X�s�[�h���Z�b�g
* @param[in] speed�i�Đ��X�s�[�h�j
* @param[out] �Ȃ�
* @return �Ȃ�
* @details �Đ�����A�j���[�V�����X�s�[�h���Z�b�g���܂��B
*/
void Animator::SetPlaySpeed(float speed)
{
    playSpeed = speed;
}