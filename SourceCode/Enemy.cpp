#include "Enemy.h"

static vector <AnimationInfo> animInfoslime = {
    { 0, "Hit" ,false},
    { 1, "idle" ,true},
    { 2, "attack" ,false},
    { 3, "walk" ,false},
};

Enemy::Enemy() {
    MHandle = MV1LoadModel("Assets/stump-chum/source/Slime.mv1");
    trans = VGet(-1000.0f, 100.0f, -100.f);
    rotation = VGet(0.f, 3.15f, 0.f);
    MV1SetScale(MHandle, VGet(1.0f, 1.0f, 1.0f));
    status = Enemy::STATUS_LIVE;
    actStatus = 0;

    colliderType = Collider::CAPSULE_TYPE;
    StartPos = trans;
    EndPos = trans;
    EndPos.y += 55.f;
    radius = 30.f;


    anim_id = 1;
    animatorPtr = new Animator();
    animatorPtr->Load(MHandle, animInfoslime);
    //animatorPtr->Load(MHandle, animInfowolf, 7);
    animatorPtr->Play(MHandle, anim_id);

    animatorPtr->SetMonster(2);

    EHP = 100.f;
    GetEXP = 200;
}

Enemy::~Enemy() {
    MV1DeleteModel(MHandle);
    delete animatorPtr;         // Animator破棄
}

void Enemy::Move() {
    anim_id = 2;
    animatorPtr->Play(MHandle, anim_id);
}

void Enemy::EnemyCollision()
{
    typedef struct _AttackCollider {
        string startNodeName;
        string endNodeName;
    } AttackCollider;

    vector<AttackCollider> attackCollider = {
        { "Bone.002" ,"Bone.002" },
    };

    for (int i = 0; i < attackCollider.size(); i++) {
        EnemyAttack* attackPtr = new EnemyAttack();
        attackPtr->SetPlayer(GetPlayer());
        attackPtr->SetPlayer2(GetPlayer2());
        attackPtr->SetPlayer3(GetPlayer3());
        attackPtr->name = "EAttack";
        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        colliderPtr->colliderType = Collider::CAPSULE_TYPE;
        attackPtr->frameIndices[0] = MV1SearchFrame(MHandle, attackCollider[i].startNodeName.c_str());
        attackPtr->frameIndices[1] = MV1SearchFrame(MHandle, attackCollider[i].endNodeName.c_str());
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        colliderPtr->radius = 33.f;
        colliderPtr->SetGameObject(attackPtr);

        attackVec.push_back(colliderPtr);
    }

}

void Enemy::transform_collider()
{
    for (int i = 0; i < attackVec.size(); i++) {
        EnemyAttack* attackPtr = (EnemyAttack*)attackVec[i];

        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        MATRIX matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

        if (EAttack) {
            attackPtr->name = "EAttack";
        }
        else {
            attackPtr->name = "Enemy";
        }
    }
}

void Enemy::SetupCollision(CollisionManager* managerPtr) {
    collisionManagerPtr = managerPtr;

    //当たり判定をコリジョンマネージャーに登録
    vector<Collider*>colliderVec = GetCollider();
    for (int i = 0; i < colliderVec.size(); i++) {
        collisionManagerPtr->Add(colliderVec[i]);
    }
}

void Enemy::Think()
{
    if (dead) {
        return;
    }
    if (actStatus == ACT_STATUS_STOP) {
        // 止まる
        animatorPtr->Play(MHandle, 1);
        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 4000.f) {
            actStatus = ACT_STATUS_CHASE;
        }
    }
    else if (actStatus == ACT_STATUS_CHASE && !animatorPtr->GetAttackFrag()) {
        // 追いかける
        animatorPtr->Play(MHandle, 3);
        targetChase();

        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist >= 10000.f) {
            actStatus = ACT_STATUS_STOP;
        }
        if (dist <= 100.f) {
            actStatus = ACT_STATUS_ATTCK;
        }
    }
    else if (actStatus == ACT_STATUS_ATTCK) {
        EAttack = true;
        animatorPtr->Play(MHandle, 2);
        float dist = sqrtf(powf((targetPos.x - trans.x), 2) + powf((targetPos.z - trans.z), 2));
        if (dist <= 2000.f && dist >= 100.f) {
            actStatus = ACT_STATUS_CHASE;
        }
    }
    if (EAttack) {
        targetChase();
    }
}

void Enemy::targetChase() {
    if (targetPos.x != 0.0f && targetPos.z != 0.0f || EAttack) {
        // target_posに数値が設定されたら、以降を処理する

        // ⑥
        // プレイヤーと敵のワールド座標より、方向ベクトルを作る
        
        VECTOR chase_dir;
        chase_dir.x = targetPos.x - trans.x;
        chase_dir.y = targetPos.y - trans.y;
        chase_dir.z = targetPos.z - trans.z;

        
        chase_dir = VNorm(chase_dir);

        
        if (EAttack) {
            trans.x += chase_dir.x * 3.5f;
            trans.z += chase_dir.z * 3.5f;
        }
        else {
            trans.x += chase_dir.x * 5.0f;
            trans.z += chase_dir.z * 5.0f;
        }
       
        rotation.y = atan2f(trans.x - targetPos.x, trans.z - targetPos.z);
    }
}

void Enemy::Status() {

    if (EHP <= 0 && !dead) {
        animatorPtr->Play(MHandle, 0);
        deadanim = true;
        dead = true;
    }
    if (dead && !deadanim) {
        MV1DeleteModel(MHandle);
    }

    if (dead && !EXP) {
        if (playerPtr != nullptr) {
            playerPtr->EXP += GetEXP;
            EXP = true;
        }
        if (player2Ptr != nullptr) {
            player2Ptr->EXP += GetEXP;
            EXP = true;
        }
        if (player3Ptr != nullptr) {
            player3Ptr->EXP += GetEXP;
            EXP = true;
        }
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

void Enemy::Update() {

    //Move();

    Think();

    // トランスフォームマトリクスを作る
    Transform(MHandle);

    transform_collider();

    Status();

    if (!animatorPtr->Update())
    {
        EAttack = false;
        deadanim = false;
    }

    MV1DrawModel(MHandle);
    
    // ※：ポイント　カプセルの始点と終点座標を更新
    if (!dead) {
        StartPos = trans;
        StartPos.y += 20;
        EndPos = StartPos;
        EndPos.y += 30.f;
    }

}

void EnemyAttack::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {

}


// コリジョン判定でヒットした時に呼び出されるコールバック関数
void Enemy::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr)
{
    //プレイヤーの攻撃を受けたらダメージ
    if (gobjPtr->name == "PAttackHit" && !Hit) {
        if (playerPtr != nullptr) {
            if (playerPtr->P1) {
                Edamage = playerPtr->AttackPoint;
            }
        }
        if (player2Ptr != nullptr) {
            if (player2Ptr->P2) {
                Edamage = player2Ptr->AttackPoint;
            }
        }
        if (player3Ptr != nullptr) {
            if (player3Ptr->P3) {
                Edamage = player3Ptr->AttackPoint;
            }
        }
        EHP -= Edamage;
        Hit = true;
    }
    //強攻撃の処理
    if (gobjPtr->name == "PAttackHit+" && !Hit) {
        if (playerPtr != nullptr) {
            if (playerPtr->P1) {
                Edamage = playerPtr->AttackPointUP;
            }
        }
        if (player2Ptr != nullptr) {
            if (player2Ptr->P2) {
                Edamage = player2Ptr->AttackPointUP;
            }
        }
        if (player3Ptr != nullptr) {
            if (player3Ptr->P3) {
                Edamage = player3Ptr->AttackPointUP;
            }
        }
        EHP -= Edamage;
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