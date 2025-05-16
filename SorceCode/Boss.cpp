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

    BHP = 30000.f; //ボスのHP
    MAXHP = 30000.f;

    anim_id = 2;
    animatorPtr = new Animator();
    //animatorPtr->Load(animInfo);
    animatorPtr->Load(MHandle, animInfodoragon);
    animatorPtr->Play(MHandle, anim_id);
    isAttack = false;
    end = false;
    dead = false;

    Sound1 = LoadSoundMem("Assets/sound/大砲1.mp3");
    Sound2 = LoadSoundMem("Assets/sound/ゴブリンの鳴き声1.mp3");
    Sound3 = LoadSoundMem("Assets/sound/打撃6.mp3");

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
        //最後のコライダーの名前を武器にする
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

    //当たり判定をコリジョンマネージャーに登録
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
        // 止まる
        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 4000.f) {
            actStatus = ACT_STATUS_CHASE;
        }
    }
    else if (actStatus == ACT_STATUS_CHASE && !animatorPtr->GetAttackFrag())
    {
        // 追いかける
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
        //ボスのHPが一定を超えると攻撃速度が上がる
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
        // target_posに数値が設定されたら、以降を処理する

        // ⑥
        // プレイヤーと敵のワールド座標より、方向ベクトルを作る
        // ベクトルの作り方を覚えよう
        // 
        // ベクトル同士の引き算で（大きさを含んだ）方向ベクトルが得られる
        // 但し、引く時のベクトルの順番を間違えると想定していた方向とは逆方向ベクトルになってしまうので注意
        // この場合、target_pos（プレイヤー）の座標へ向けて移動させたいので、target_posがベクトルの終点になる
        // 始点は自分（この場合、敵）のワールド座標であるpositionが始点ベクトルになる
        // 終点－始点で終点方向への方向ベクトルが得られる。
        // 始点－終点にすると逆方向ベクトルとなる。
        // 
        // target_posとpositionの引き算の順番を入れ替えたら、どんな動きになるか確認してみよう！
        VECTOR chase_dir;
        chase_dir.x = targetPos.x - trans.x;
        chase_dir.y = targetPos.y - trans.y;
        chase_dir.z = targetPos.z - trans.z;

        // ⑦
        // chase_dirは、まだ大きさを持つベクトルで、このままではベクトルの大きさが大きすぎて
        // 一瞬でプレイヤーに追いついてしまい方向ベクトルとしては使えないので、
        // chase_dirを正規化（Normalize）して単位ベクトル（Unit Vector）化する
        // 試しに、VNormで正規化しない（コメントアウト）で実行結果を確認してみると意味がわかるよ！
        chase_dir = VNorm(chase_dir);

        // ⑧
        // chase_dirは、xyzの値が、1～-1の単位ベクトルになったので、
        // 敵モデルをchase_dirの方向へ進むようにxz座標に加算する。
        // 1.0fはベクトルの大きさで、1.0fを大きくすると敵が速く移動し、小さくすると遅く移動する
        if (BHP > BRast) {
            trans.x += chase_dir.x * 15.0f;
            trans.z += chase_dir.z * 15.0f;
        }
        //ボスのHP一定を超えると移動速度が上がる
        else {
            trans.x += chase_dir.x * 30.0f;
            trans.z += chase_dir.z * 30.0f;
        }
        // ⑨
        // atan2fを使って、敵とプレイヤーの傾きより、敵モデルを向ける角度を求めY軸の回転値とする。
        // 回転値はラジアンをあらわず
        // 下のコードをコメントアウトしたらどうなるか確認してみよう
        rotation.y = atan2f(trans.x - targetPos.x, trans.z - targetPos.z);
    }
}

void Boss::SetTargetPos(VECTOR pos)
{
    // 追いかけるターゲットのワールド座標をセット④
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
        DrawBox(1930 - damage2, 10, 1930, 60, GetColor(255, 0, 0), TRUE); //HPの表示
    }
    //ボス攻撃時の音
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

    //ボスのHPが一定を超えるとアニメーションが入る
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

    //ボスのHPが0になったら死ぬ
    if (BHP <= 0 && !dead) {
        MV1DeleteModel(MHandle);
        dead = true;
        end = true;
    }
    //死んだ後の処理
    if (end && dead) {
        WaitTimer(1500);
        effectManagerPtr->Play(3, trans);
        if (!SoundFlag) {
            PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
            SoundFlag = true;
        }
        end = false;
    }

    //壁に入らないようにする
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

    // トランスフォームマトリクスを作る
    Transform(MHandle);

    MV1DrawModel(MHandle);

    transform_collider();

    Status();

    // アニメーション再生更新
    if (!animatorPtr->Update())
    {
        // アニメーション再生が終了しニュートラルへ戻った
        SoundFlag = false;
        RastSpart2 = false;
        BAttack = false;
        AttackSound = false;
    }
}

void BossAttack::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {
    //プレイヤーの攻撃を受けたらダメージ
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
    //強攻撃の処理
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