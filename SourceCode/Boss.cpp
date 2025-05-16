#include "Boss.h"

static vector <AnimationInfo> animInfodoragon = {
    { 0, "attack" ,false},
    { 1, "loading" ,false},
    { 2, "idle" ,true},
    { 3, "idle2" ,false},
    { 4, "attack2" ,false},
    { 5, "dash" ,false},
    { 6, "dash2" ,false},
    { 7, "idle3" ,false},
    { 8, "idle4" ,false},
};

Boss::Boss()
{
    MHandle = MV1LoadModel("Assets/dragonknight_-_color_-_hots/doragonknight.mv1");
    trans = VGet(-8500.0f, 100.0f, 11000.f);
    rotation = VGet(0.f, 0.f, 0.f);
    scale = VGet(3.f, 3.f, 3.f);
    actStatus = 0;

    BHP = 30000.f; //�{�X��HP
    MAXHP = 30000.f;

    anim_id = 2;
    animatorPtr = new Animator();
    //animatorPtr->Load(animInfo);
    animatorPtr->Load(MHandle, animInfodoragon);
    animatorPtr->Play(MHandle, anim_id);
    isAttack = false;
    end = false;
    dead = false;

    Sound1 = LoadSoundMem("Assets/sound/��C1.mp3");
    Sound2 = LoadSoundMem("Assets/sound/�S�u�����̖���1.mp3");
    Sound3 = LoadSoundMem("Assets/sound/�Ō�6.mp3");

    animatorPtr->SetMonster(1);

    //BossCollision();
}

Boss::~Boss()
{

}

void Boss::Move()
{
    anim_id = 2;
    animatorPtr->Play(MHandle, anim_id);
}

void Boss::BossCollision(){
    
    typedef struct _AttackCollider
    {
        string startNodeName;
        string endNodeName;
    } AttackCollider;

    vector<AttackCollider> attackCollider = {
        { "Bone_FrontCloth_53" ,"Bone_Chest_50" },
        { "Bone_Chest_50" ,"Bone_Head_48" },
        { "Bone_LegThigh_R_58" ,"Bone_LegCalf_R_57" },
        { "Bone_LegCalf_R_57" ,"Bone_LegToe_R_54" },
        { "Bone_LegThigh_L_63" ,"Bone_LegCalf_L_62" },
        { "Bone_LegCalf_L_62" ,"Bone_LegToe_L_59" },
        { "Bone_Arm_R_18" ,"Bone_ArmFore_R_17" },
        { "Bone_ArmFore_R_17" ,"Bone_ArmHand_R_16" },
        { "Bone_Arm_L_28" ,"Bone_ArmFore_L_27" },
        { "Bone_ArmFore_L_27" ,"Bone_ArmHand_L_26" },
        { "Ref_Weapon_78" ,"Ref_Weapon_78" },
    };

    for (int i = 0; i < attackCollider.size(); i++) {
        BossAttack* attackPtr = new BossAttack();
        attackPtr->SetPlayer(GetPlayer());
        attackPtr->SetPlayer2(GetPlayer2());
        attackPtr->SetPlayer3(GetPlayer3());
        if (i < attackCollider.size() - 1) {
            attackPtr->name = "Boss";
        }
        //�Ō�̃R���C�_�[�̖��O�𕐊�ɂ���
        else {
            attackPtr->name = "Weapon";
        }
        //attackPtr->name = "Boss";
        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        colliderPtr->colliderType = Collider::CAPSULE_TYPE;
        attackPtr->frameIndices[0] = MV1SearchFrame(MHandle, attackCollider[i].startNodeName.c_str());
        attackPtr->frameIndices[1] = MV1SearchFrame(MHandle, attackCollider[i].endNodeName.c_str());
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        colliderPtr->radius = 40.f;
        colliderPtr->SetGameObject(attackPtr);

        attackVec.push_back(colliderPtr);
    }

}


void Boss::transform_collider()
{
    for (int i = 0; i < attackVec.size(); i++)
    {
        BossAttack* attackPtr = (BossAttack*)attackVec[i];

        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        MATRIX matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
    }
}

void Boss::SetupCollision(CollisionManager* managerPtr)
{
    collisionManagerPtr = managerPtr;

    //�����蔻����R���W�����}�l�[�W���[�ɓo�^
    vector<Collider*>colliderVec = GetCollider();
    for (int i = 0; i < colliderVec.size(); i++)
    {
        collisionManagerPtr->Add(colliderVec[i]);
    }
}

void Boss::Think()
{
    if (RastSpart2) {
        return;
    }
    if (actStatus == ACT_STATUS_STOP) {
        // �~�܂�
        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 4000.f) {
            actStatus = ACT_STATUS_CHASE;
        }
    }
    else if (actStatus == ACT_STATUS_CHASE && !animatorPtr->GetAttackFrag())
    {
        // �ǂ�������
        if (BHP > BRast) {
            animatorPtr->SetPlaySpeed(1);
        }
        else {
            animatorPtr->SetPlaySpeed(1.3);
        }
        animatorPtr->Play(MHandle, 6);
        targetChase();

        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 10000.f)
        {
            actStatus = ACT_STATUS_STOP;
        }
        if (dist < 100.f)
        {
            actStatus = ACT_STATUS_ATTCK;
        }
    }
    else if (actStatus == ACT_STATUS_ATTCK)
    {
        if (BHP > BRast) {
            animatorPtr->SetPlaySpeed(0.7);
        }
        //�{�X��HP�����𒴂���ƍU�����x���オ��
        else {
            animatorPtr->SetPlaySpeed(1.2);
        }
        animatorPtr->Play(MHandle, 0);
        BAttack = true;
        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 2000.f && dist >= 100.f)
        {
            actStatus = ACT_STATUS_CHASE;
        }
    }
}

void Boss::targetChase()
{
    if (targetPos.x != 0.0f && targetPos.z != 0.0f)
    {
        // target_pos�ɐ��l���ݒ肳�ꂽ��A�ȍ~����������

        // �E
        // �v���C���[�ƓG�̃��[���h���W���A�����x�N�g�������
        // �x�N�g���̍������o���悤
        // 
        // �x�N�g�����m�̈����Z�Łi�傫�����܂񂾁j�����x�N�g����������
        // �A���A�������̃x�N�g���̏��Ԃ��ԈႦ��Ƒz�肵�Ă��������Ƃ͋t�����x�N�g���ɂȂ��Ă��܂��̂Œ���
        // ���̏ꍇ�Atarget_pos�i�v���C���[�j�̍��W�֌����Ĉړ����������̂ŁAtarget_pos���x�N�g���̏I�_�ɂȂ�
        // �n�_�͎����i���̏ꍇ�A�G�j�̃��[���h���W�ł���position���n�_�x�N�g���ɂȂ�
        // �I�_�|�n�_�ŏI�_�����ւ̕����x�N�g����������B
        // �n�_�|�I�_�ɂ���Ƌt�����x�N�g���ƂȂ�B
        // 
        // target_pos��position�̈����Z�̏��Ԃ����ւ�����A�ǂ�ȓ����ɂȂ邩�m�F���Ă݂悤�I
        VECTOR chase_dir;
        chase_dir.x = targetPos.x - trans.x;
        chase_dir.y = targetPos.y - trans.y;
        chase_dir.z = targetPos.z - trans.z;

        // �F
        // chase_dir�́A�܂��傫�������x�N�g���ŁA���̂܂܂ł̓x�N�g���̑傫�����傫������
        // ��u�Ńv���C���[�ɒǂ����Ă��܂������x�N�g���Ƃ��Ă͎g���Ȃ��̂ŁA
        // chase_dir�𐳋K���iNormalize�j���ĒP�ʃx�N�g���iUnit Vector�j������
        // �����ɁAVNorm�Ő��K�����Ȃ��i�R�����g�A�E�g�j�Ŏ��s���ʂ��m�F���Ă݂�ƈӖ����킩���I
        chase_dir = VNorm(chase_dir);

        // �G
        // chase_dir�́Axyz�̒l���A1�`-1�̒P�ʃx�N�g���ɂȂ����̂ŁA
        // �G���f����chase_dir�̕����֐i�ނ悤��xz���W�ɉ��Z����B
        // 1.0f�̓x�N�g���̑傫���ŁA1.0f��傫������ƓG�������ړ����A����������ƒx���ړ�����
        if (BHP > BRast) {
            trans.x += chase_dir.x * 15.0f;
            trans.z += chase_dir.z * 15.0f;
        }
        //�{�X��HP���𒴂���ƈړ����x���オ��
        else {
            trans.x += chase_dir.x * 30.0f;
            trans.z += chase_dir.z * 30.0f;
        }
        // �H
        // atan2f���g���āA�G�ƃv���C���[�̌X�����A�G���f����������p�x������Y���̉�]�l�Ƃ���B
        // ��]�l�̓��W�A��������킸
        // ���̃R�[�h���R�����g�A�E�g������ǂ��Ȃ邩�m�F���Ă݂悤
        rotation.y = atan2f(trans.x - targetPos.x, trans.z - targetPos.z);
    }
}

void Boss::SetTargetPos(VECTOR pos)
{
    // �ǂ�������^�[�Q�b�g�̃��[���h���W���Z�b�g�C
        targetPos = pos;
}

void Boss::Status(){
    if (dead) {
        return;
    }

    if (BHP2 > BHP) {
        effectManagerPtr->Play(5, VGet(trans.x,trans.y + 100,trans.z));
    }

    BHP2 = BHP;

    damage = BHP / MAXHP * 100;
    damage2 = damage * 7;

    if (actStatus == ACT_STATUS_CHASE) {
        BStart = true;
    }
    if (BStart) {
        DrawBox(1930 - damage2, 10, 1930, 60, GetColor(255, 0, 0), TRUE); //HP�̕\��
    }
    //�{�X�U�����̉�
    if (BAttack && !AttackSound) {
        cnt++;
        if (cnt > 60 && BHP > 1000) {
            PlaySoundMem(Sound3, DX_PLAYTYPE_BACK);
            cnt = 0;
            AttackSound = true;
        }
        if (cnt > 30 && BHP <= 1000) {
            PlaySoundMem(Sound3, DX_PLAYTYPE_BACK);
            cnt = 0;
            AttackSound = true;
        }
    }

    //�{�X��HP�����𒴂���ƃA�j���[�V����������
    if (BHP <= BRast && !RastSpart) {
        RastSpart = true;
        RastSpart2 = true;
    }
    if (RastSpart && RastSpart2) {
        animatorPtr->SetPlaySpeed(0.4);
        animatorPtr->Play(MHandle, 3);

        if (!SoundFlag) {
            PlaySoundMem(Sound2, DX_PLAYTYPE_BACK);
            SoundFlag = true;
        }
    }

    //�{�X��HP��0�ɂȂ����玀��
    if (BHP <= 0 && !dead) {
        MV1DeleteModel(MHandle);
        dead = true;
        end = true;
    }
    //���񂾌�̏���
    if (end && dead) {
        WaitTimer(1500);
        effectManagerPtr->Play(3, trans);
        if (!SoundFlag) {
            PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
            SoundFlag = true;
        }
        end = false;
    }

    //�ǂɓ���Ȃ��悤�ɂ���
    if (trans.x <= 1850.0f && trans.x >= -1850.0f && trans.z <= -1850.f) {
        trans.z = -1850;
    }
    if (trans.x >= 1850.f && trans.z <= 2850.f && trans.z >= -1850.f) {
        trans.x = 1850;
    }
    if (trans.x <= 1850.0f && trans.x >= -1850.0f && trans.z >= 2850.f) {
        trans.z = 2850;
    }
    if (trans.x <= -1850.f && trans.x >= -1950 && trans.z <= 2850.f && trans.z >= 800.f) {
        trans.x = -1850;
    }
    if (trans.x <= -1850.f && trans.x >= -1950 && trans.z <= 200.f && trans.z >= -1850.f) {
        trans.x = -1850;
    }
    if (trans.x <= -1850.f && trans.x >= -6170.f && trans.z >= 800.f && trans.z <= 900) {
        trans.z = 800;
    }
    if (trans.x <= -1850.f && trans.x >= -6830.f && trans.z <= 200.f && trans.z >= 100) {
        trans.z = 200;
    }
    if (trans.x <= -6830.f && trans.x >= -6930 && trans.z >= 200.f && trans.z <= 2150) {
        trans.x = -6830;
    }
    if (trans.x >= -6170.f && trans.x <= -6070 && trans.z >= 800.f && trans.z <= 2850) {
        trans.x = -6170;
    }
    if (trans.x <= -6830.f && trans.x >= -8850.f && trans.z <= 2150 && trans.z >= 2050) {
        trans.z = 2150;
    }
    if (trans.x <= -6170.f && trans.x >= -8150.f && trans.z >= 2850 && trans.z <= 2950) {
        trans.z = 2850;
    }
    if (trans.x <= -8850.f && trans.x >= -8950 && trans.z >= 2150.f && trans.z <= 7170) {
        trans.x = -8850;
    }
    if (trans.x >= -8150.f && trans.x <= -8050 && trans.z >= 2850.f && trans.z <= 7170) {
        trans.x = -8150;
    }
    if (trans.x <= -8850.f && trans.x >= -13800 && trans.z <= 7170 && trans.z >= 7070) {
        trans.z = 7170;
    }
    if (trans.x <= -4150.f && trans.x >= -8150 && trans.z <= 7170 && trans.z >= 7070) {
        trans.z = 7170;
    }
    if (trans.x <= -13800.f && trans.z >= 7170.f && trans.z <= 11850) {
        trans.x = -13800;
    }
    if (trans.x >= -4150.f && trans.z >= 7170.f && trans.z <= 11850) {
        trans.x = -4150;
    }
    if (trans.x <= -4150.f && trans.x >= -13800 && trans.z >= 11850) {
        trans.z = 11850;
    }
}

void Boss::Update()
{

    Think();

    // �g�����X�t�H�[���}�g���N�X�����
    Transform(MHandle);

    MV1DrawModel(MHandle);

    transform_collider();

    Status();

    // �A�j���[�V�����Đ��X�V
    if (!animatorPtr->Update())
    {
        // �A�j���[�V�����Đ����I�����j���[�g�����֖߂���
        SoundFlag = false;
        RastSpart2 = false;
        BAttack = false;
        AttackSound = false;
    }
}

void BossAttack::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {
    //�v���C���[�̍U�����󂯂���_���[�W
    if (gobjPtr->name == "PAttackHit" && !Hit) {
        if (playerPtr != nullptr) {
            if (playerPtr->P1) {
                Bdamage = playerPtr->AttackPoint;
            }
        }
        if (player2Ptr != nullptr) {
            if (player2Ptr->P2) {
                Bdamage = player2Ptr->AttackPoint;
            }
        }
        if (player3Ptr != nullptr) {
            if (player3Ptr->P3) {
                Bdamage = player3Ptr->AttackPoint;
            }
        }
        BHP -= Bdamage;
        Hit = true;
    }
    //���U���̏���
    if (gobjPtr->name == "PAttackHit+" && !Hit) {
        if (playerPtr != nullptr) {
            if (playerPtr->P1) {
                Bdamage = playerPtr->AttackPointUP;
            }
        }
        if (player2Ptr != nullptr) {
            if (player2Ptr->P2) {
                Bdamage = player2Ptr->AttackPointUP;
            }
        }
        if (player3Ptr != nullptr) {
            if (player3Ptr->P3) {
                Bdamage = player3Ptr->AttackPointUP;
            }
        }
        BHP -= Bdamage;
        Hit = true;
    }
    if (playerPtr != nullptr) {
        if (playerPtr->P1) {
            if (!playerPtr->isAttack2 && !playerPtr->isAttack) {
                Hit = false;
            }
        }
    }
    if (player2Ptr != nullptr) {
        if (player2Ptr->P2) {
            if (!player2Ptr->isAttack2 && !player2Ptr->isAttack) {
                Hit = false;
            }
        }
    }
    if (player3Ptr != nullptr) {
        if (player3Ptr->P3) {
            if (!player3Ptr->isAttack2 && !player3Ptr->isAttack) {
                Hit = false;
            }
        }
    }
}

void Boss::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {

};
void Boss::SetupEffectManager(EffectManager* ptr) {
    effectManagerPtr = ptr;
}