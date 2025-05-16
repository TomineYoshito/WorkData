#include "Player3.h"

static vector <AnimationInfo> animInfowolf = {
    { 0, "attack" ,false},
    { 1, "attack2" ,false},
    { 2, "damage" ,false},
    { 3, "dead" ,false},
    { 4, "idle" ,false},
    { 5, "run" ,false},
    { 6, "walk" ,false},
};


Player3::Player3() {
    MHandle = MV1LoadModel("Assets/wolf.mv1");
    trans = VGet(320.0f, 100.0f, 550.f);
    rotation = VGet(0.f, 3.15f, 0.f);
    scale = VGet(1.f, 1.f, 1.f);

    anim_id = 4;
    animatorPtr = new Animator();
    //animatorPtr->Load(animInfo);
    animatorPtr->Load(MHandle, animInfowolf,7);
    animatorPtr->Play(MHandle, anim_id);

    HP = 500.f;
    MAXHP = 500.f;

    //前向きベクトル
    forward = VGet(-1.f, 0.f, -1.f);

    cameraPosition = VGet(320.f, 800.f, 500.f);
    cameraVAngle = -1000.0f;
    cameraHAngle = -90.0f;

    colliderType = Collider::CAPSULE_TYPE;
    StartPos = trans;
    EndPos = trans;
    EndPos.y += 30.f;
    radius = 30.f;

    // マウスの位置をセット
    SetMousePoint(990, 1000);

    IsWallHit = false;
    isAttack = false;
    isAttack2 = false;
    Step = false;
    CoolTime = false;
    command = false;
    notAttack = false;

    Cr = GetColor(255, 0, 0);        // 図形の色の設定
    Cr2 = GetColor(255, 255, 255);   // 文字の色の設定
    Cr3 = GetColor(0, 128, 0);       // HPの色の設定
    Cr4 = GetColor(255, 255, 0);     // 経験値ゲージの色の設定

    Sound1 = LoadSoundMem("Assets/sound/打撃1.mp3");
    Sound2 = LoadSoundMem("Assets/sound/踏み込む.mp3");
    Sound3 = LoadSoundMem("Assets/sound/目をパチパチ.mp3");

    GHandle2 = LoadGraph("Assets/24451082.png");
    GHandle3 = LoadGraph("Assets/穴掘り.png");
    GHandle4 = LoadGraph("Assets/24683181.png");
    GHandle5 = LoadGraph("Assets/24842771.png");


    AttackCollision();
}

Player3::~Player3() {
    MV1DeleteModel(MHandle);
    delete animatorPtr;         // Animator破棄
}

void Player3::Move() {
    GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
    if (dead) {       //死んだら動けない
        return;
    }

    work = false;
    work2 = false;
    if (!PAD_key) {
        //プレイヤーの移動
        if (CheckHitKey(KEY_INPUT_A) && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            work2 = true;
            key = true;
            trans.x += sinf(rotation.y + DX_PI_F / 2) * 5.f;
            trans.z += cosf(rotation.y + DX_PI_F / 2) * 5.f;
            anim_id = 5;
            animatorPtr->Play(MHandle, anim_id);
        }
        else if (CheckHitKey(KEY_INPUT_D) && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            work2 = true;
            key = true;
            trans.x -= sinf(rotation.y + DX_PI_F / 2) * 5.f;
            trans.z -= cosf(rotation.y + DX_PI_F / 2) * 5.f;
            anim_id = 5;
            animatorPtr->Play(MHandle, anim_id);
        }
        else if (CheckHitKey(KEY_INPUT_W) && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            //speed = 20;
            work = true;
            key = true;
            isAttack = false;
            Step = false;
            trans.x -= sinf(rotation.y) * speed;
            trans.z -= cosf(rotation.y) * speed;
            if (!IsJamp) {
                anim_id = 5;
                animatorPtr->Play(MHandle, anim_id);
            }
        }
        else if (CheckHitKey(KEY_INPUT_S) && !isAttack && !IsWallHit && !isAttack2 && !notAttack) {
            work2 = true;
            //speed = 20;
            key = true;
            trans.x += sinf(rotation.y) * speed;
            trans.z += cosf(rotation.y) * speed;
            anim_id = 5;
            animatorPtr->Play(MHandle, anim_id);
        }
        else if (!isAttack && !Step && !IsJamp && !isAttack2 && !notAttack) {
            animatorPtr->SetPlaySpeed(1);
            anim_id = 4;
            animatorPtr->Play(MHandle, anim_id);
        }
    }

    if (!key) {
        if (PAD.ThumbLX < -10000.0f && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            work2 = true;
            PAD_key = true;
            trans.x += sinf(rotation.y + DX_PI_F / 2) * speed/2;
            trans.z += cosf(rotation.y + DX_PI_F / 2) * speed/2;
            if (!PAD.ThumbLY < 10000.0f || !PAD.ThumbLY > -10000.0f) {
                animatorPtr->SetPlaySpeed(1);
                anim_id = 5;
                animatorPtr->Play(MHandle, anim_id);
            }
        }

        if (PAD.ThumbLX > 10000.0f && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            work2 = true;
            PAD_key = true;
            trans.x -= sinf(rotation.y + DX_PI_F / 2) * speed/2;
            trans.z -= cosf(rotation.y + DX_PI_F / 2) * speed/2;
            if (PAD.ThumbLY < 10000.0f || PAD.ThumbLY > -10000.0f) {
                animatorPtr->SetPlaySpeed(1);
                anim_id = 5;
                animatorPtr->Play(MHandle, anim_id);
            }
        }
        if (PAD.ThumbLY > 10000.0f && !IsWallHit && !isAttack && !Step && !isAttack2 && !notAttack) {
            work = true;
            PAD_key = true;
            //speed = 20;
            isAttack = false;
            Step = false;
            trans.x -= sinf(rotation.y) * speed;
            trans.z -= cosf(rotation.y) * speed;
            if (!IsJamp) {
                animatorPtr->SetPlaySpeed(1);
                anim_id = 5;
                animatorPtr->Play(MHandle, anim_id);
            }
        }
        if (PAD.ThumbLY < -10000.0f && !isAttack && !IsWallHit && !isAttack2 && !notAttack) {
            work2 = true;
            PAD_key = true;
            //speed = 20;
            trans.x += sinf(rotation.y) * speed;
            trans.z += cosf(rotation.y) * speed;
            animatorPtr->SetPlaySpeed(1);
            anim_id = 5;
            animatorPtr->Play(MHandle, anim_id);
        }

        if (!isAttack && !Step && !IsJamp && !isAttack2 && !notAttack && !work && !work2) {
            animatorPtr->SetPlaySpeed(1);
            anim_id = 4;
            animatorPtr->Play(MHandle, anim_id);
        }
    }

    //ダッシュ
    if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 && !work2) {
        //anim_id = 5;
        speed = 50 + speedUP;
    }
    if (PAD.Buttons[9] && !work2) {
        //anim_id = 5;
        speed = 50 + speedUP;
    }
    else {
        //anim_id = 3;
        speed = 30 + speedUP;
    }

    //ジャンプ
    if (!IsJamp && CheckHitKey(KEY_INPUT_SPACE)) {
        animatorPtr->SetPlaySpeed(0.3);
        anim_id = 5;
        animatorPtr->Play(MHandle, anim_id);
        IsJamp = true;
        JampPower = JUMP_POWER;
        GroundPos = trans;
        IsGroundHit = false;
    }
    if (!IsJamp && PAD.Buttons[12]) {
        animatorPtr->SetPlaySpeed(0.3);
        anim_id = 5;
        animatorPtr->Play(MHandle, anim_id);
        IsJamp = true;
        JampPower = JUMP_POWER;
        GroundPos = trans;
        IsGroundHit = false;
    }
    //else if (!IsJamp) {
    //    animatorPtr->SetPlaySpeed(1);
    //}

    //弱攻撃
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !isAttack2 && !notAttack && !command) {
        anim_id = 0;
        isAttack = true;
        animatorPtr->Play(MHandle, anim_id);
        if (!SoundFlag) {
            PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
            SoundFlag = true;
        }
    }
    if (PAD.Buttons[13] && !isAttack2 && !notAttack && !command) {
        anim_id = 0;
        isAttack = true;
        animatorPtr->SetPlaySpeed(1 + AnimSpeed);
        animatorPtr->Play(MHandle, anim_id);
        if (!SoundFlag) {
            PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
            SoundFlag = true;
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


    //if (CheckHitKey(KEY_INPUT_LSHIFT) && !Step && !CoolTime) {
    //    Step = true;
    //    CoolTime = true;
    //}
    ////アニメーション再生中動く
    //if (Step) {
    //    animatorPtr->SetPlaySpeed(2);
    //    trans.x -= sinf(rotation.y) * 20;
    //    trans.z -= cosf(rotation.y) * 20;
    //    anim_id = 5;
    //    animatorPtr->Play(MHandle, anim_id);
    //}
    ////クールタイムの設定
    //if (CoolTime) {
    //    if (Time < 150) {
    //        Time++;
    //    }
    //    else {
    //        CoolTime = false;
    //        Time = 0;
    //    }
    //}

}

void Player3::Jamp() {
    // プレイヤーをジャンプさせる
// ジャンプをする処理
    if (IsJamp == true) {
        // ジャンプ力を重力分減算する
        JampPower -= GRAVITY;

        // Y軸の移動ベクトルにジャンプ力を設定
        trans.y += JampPower;
        if (trans.y <= GroundPos.y) {
            IsJamp = false;
            trans.y = GroundPos.y;
            IsGroundHit = true;
        }
    }

    else {
        // ジャンプ中ではない
        trans.y -= 10.f;
        if (trans.y <= GroundPos.y) {
            trans.y = GroundPos.y;

            IsJamp = false;
            JampPower = 0.f;
            IsGroundHit = true;
        }
    }

}

void Player3::Attack() {

    GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
    if (command2 || dead) {       //技中と死んだときは選択画面を消す
        return;
    }
    if (!PAD_key) {
        //技の選択画面を表示する
        if (CheckHitKey(KEY_INPUT_E)) {
            key = true;
            command = true;                            //技選択中はコマンドをtrueにする
            //DrawBox(660, 100, 1320, 400, Cr, TRUE);    // 四角形を描画
            DrawExtendGraph(600, 50, 1400, 450, GHandle5, TRUE);
            DrawString(880, 230, "強攻撃", Cr2);        // 文字列の描画
            DrawExtendGraph(1100, 200, 1300, 350, GHandle2, TRUE);
            //DrawBox(0, 400, 660, 700, Cr, TRUE);
            DrawExtendGraph(0, 400, 750, 800, GHandle5, TRUE);
            DrawString(280, 520, "潜る", Cr2);
            DrawFormatString(180, 460, Cr2, "残り使用回数%d", SPoint);
            DrawExtendGraph(500, 550, 650, 650, GHandle3, TRUE);
            //DrawBox(1320, 400, 1980, 700, Cr, TRUE);
            DrawExtendGraph(1250, 400, 1980, 800, GHandle5, TRUE);
            DrawString(1550, 520, "回復", Cr2);
            DrawFormatString(1450, 460, Cr2, "残り使用回数%d", SPoint2);
            DrawExtendGraph(1750, 550, 1900, 650, GHandle4, TRUE);
            //DrawBox(660, 600, 1320, 850, Cr, TRUE);
            //DrawString(930, 700, "戻る", Cr2);
        }
        else {
            command = false;
        }
    }
    if (GetMouseInput() & MOUSE_INPUT_LEFT) {
        GetMousePoint(&MouseX, &MouseY);        // カーソルの位置を取得

        //わざ１
        if (MouseX >= 660 && MouseX < 1320 && MouseY >= 100 && MouseY < 400) {
            animatorPtr->SetPlaySpeed(1);
            anim_id = 1;
            animatorPtr->Play(MHandle, anim_id);
            isAttack2 = true;
            command2 = true;
            effect1 = true;
            if (!SoundFlag) {
                PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
                SoundFlag = true;
            }
        }

        //わざ２
        else if (MouseX >= 0 && MouseX < 660 && MouseY >= 400 && MouseY < 700 && SPoint > 0) {
            anim_id = 1;
            animatorPtr->Play(MHandle, anim_id);
            isAttack2 = true;
            command2 = true;
            SPoint -= 1;
            if (!SoundFlag2) {
                PlaySoundMem(Sound2, DX_PLAYTYPE_BACK);
                SoundFlag2 = true;
            }
        }
        //わざ３
        else if (MouseX >= 1320 && MouseX < 1980 && MouseY >= 400 && MouseY < 700 && SPoint2 > 0 && HP <= 400) {
            anim_id = 1;
            animatorPtr->Play(MHandle, anim_id);
            notAttack = true;
            command2 = true;
            HP += 100;
            effectManagerPtr->Play(7, trans);
            SPoint2 -= 1;
            if (!SoundFlag3) {
                PlaySoundMem(Sound3, DX_PLAYTYPE_BACK);
                SoundFlag3 = true;
            }
        }
        /*else if (MouseX >= 660 && MouseX < 1320 && MouseY >= 600 && MouseY < 850) {
            isAttack2 = false;
        }*/
    }
    if (!key) {
        if (PAD.Buttons[8]) {
            PAD_key = true;
            command = true;                            //技選択中はコマンドをtrueにする
            //DrawBox(660, 100, 1320, 400, Cr, TRUE);    // 四角形を描画
            DrawExtendGraph(600, 50, 1400, 450, GHandle5, TRUE);
            DrawString(900, 180, "強攻撃", Cr2);        // 文字列の描画
            DrawString(880, 230, "Ｙボタン", Cr2);
            DrawExtendGraph(1100, 200, 1300, 350, GHandle2, TRUE);
            //DrawBox(0, 400, 660, 700, Cr, TRUE);
            DrawExtendGraph(0, 400, 750, 800, GHandle5, TRUE);
            DrawString(200, 520, "ダッシュ攻撃", Cr2);
            DrawString(230, 570, "Ｘボタン", Cr2);
            //DrawFormatString(180, 460, Cr2, "残り使用回数%d", SPoint);
            //DrawExtendGraph(500, 550, 650, 650, GHandle3, TRUE);
            //DrawBox(1320, 400, 1980, 700, Cr, TRUE);
            DrawExtendGraph(1250, 400, 1980, 800, GHandle5, TRUE);
            DrawString(1500, 520, "速度強化", Cr2);
            DrawString(1500, 570, "Ｂボタン", Cr2);
            //DrawFormatString(1450, 460, Cr2, "残り使用回数%d", SPoint2);
            //DrawExtendGraph(1750, 550, 1900, 650, GHandle4, TRUE);
            //DrawBox(660, 600, 1320, 850, Cr, TRUE);
            //DrawString(930, 700, "戻る", Cr2);

            //わざ１
            if (PAD.Buttons[15]) {
                animatorPtr->SetPlaySpeed(1 + AnimSpeed);
                anim_id = 1;
                animatorPtr->Play(MHandle, anim_id);
                isAttack2 = true;
                command2 = true;
                skill1 = true;
                effect1 = true;
                if (!SoundFlag) {
                    PlaySoundMem(Sound1, DX_PLAYTYPE_BACK);
                    SoundFlag = true;
                }
            }

            //わざ２
            else if (PAD.Buttons[14] && Level >= 3) {
                isAttack2 = true;
                command2 = true;
                skill2 = true;
                dash = true;
                /*if (!SoundFlag2) {
                    PlaySoundMem(Sound2, DX_PLAYTYPE_BACK);
                    SoundFlag2 = true;
                }*/
            }
            //わざ３
            else if (PAD.Buttons[13] && SPoint2 > 0 && !speedup && Level >= 5) {
                anim_id = 2;
                animatorPtr->Play(MHandle, anim_id);
                notAttack = true;
                command2 = true;
                skill3 = true;
                speedup = true;
                HP -= 10;
                speedUP = 30;
                AnimSpeed = 1;
                //effectManagerPtr->Play(7, trans);
                //SPoint2 -= 1;
                /*if (!SoundFlag3) {
                    PlaySoundMem(Sound3, DX_PLAYTYPE_BACK);
                    SoundFlag3 = true;
                }*/
            }
        }
        else {
            command = false;
        }
    }
}

void Player3::Camera() {

    GetJoypadXInputState(DX_INPUT_PAD1, &PAD);
    if (command || dead) {       //技選択中はカメラの動きを止める
        return;
    }

    if (!PAD_key) {
        // マウスの位置を取得
        GetMousePoint(&MouseX, &MouseY);
        // マウスの操作距離を測る
        X = 990 - MouseX;

        // マウスでカメラの移動
        cameraHAngle += X / 5;
        cameraVAngle = -MouseY / 5;
        //マウスカーソルの位置をリセットする
        SetMousePoint(990, MouseY);

        if (MouseY <= 900) {              //上下の上限の設定
            SetMousePoint(990, 900);
        }
        if (MouseY > 1000) {
            SetMousePoint(990, 1000);
        }

        if (!CheckHitKey(KEY_INPUT_LSHIFT)) {
            rotation.y = -cameraHAngle / 57.3;   //マウスでキャラを回転させる
        }
    }

    if (!key) {
        if (PAD.ThumbRX > 128) {
            cameraHAngle -= (PAD.ThumbRX / 32767.0f * 2.5);
        }
        if (PAD.ThumbRX < 0) {
            cameraHAngle += (PAD.ThumbRX / -32767.0f * 2.5);
        }

        if (PAD.ThumbRY > 128) {
            PAD_key = true;
            cameraVAngle += (PAD.ThumbRY / 32767.0f);
        }
        if (PAD.ThumbRY < 0) {
            PAD_key = true;
            cameraVAngle -= (PAD.ThumbRY / -32767.0f);
        }

        if (cameraVAngle > -180) {
            cameraVAngle = -180;
        }
        if (cameraVAngle < -200) {
            cameraVAngle = -200;
        }
        if (!PAD.Buttons[7]) {
            rotation.y = -cameraHAngle / 57.3;   //マウスでキャラを回転させる
        }
    }
};

void Player3::Status() {
    if (dead) {
        return;
    }


    if (skill1) {
        DrawExtendGraph(600, 50, 1400, 450, GHandle5, TRUE);
        DrawString(900, 180, "強攻撃", Cr2);        // 文字列の描画
        DrawString(880, 230, "Ｙボタン", Cr2);
        DrawExtendGraph(1100, 200, 1300, 350, GHandle2, TRUE);
    }

    if (skill2) {
        DrawExtendGraph(0, 400, 750, 800, GHandle5, TRUE);
        DrawString(200, 520, "ダッシュ攻撃", Cr2);
        DrawString(230, 570, "Ｘボタン", Cr2);
    }

    if (skill3) {
        DrawExtendGraph(1250, 400, 1980, 800, GHandle5, TRUE);
        DrawString(1500, 520, "速度強化", Cr2);
        DrawString(1500, 570, "Ｂボタン", Cr2);
    }


    damage = HP / MAXHP * 100;
    damage2 = damage * 6;
    GHandle = LoadGraph("Assets/icon/WolfIcon.png");
    DrawBox(130, 10, 750, 60, Cr2, TRUE); //HPの枠表示
    DrawBox(140, 20, 140 + damage2, 50, Cr3, TRUE); //HPの表示
    if (HP >= 100) {
        DrawBox(760, 10, 1000, 60, Cr2, TRUE);
    }
    else {
        DrawBox(760, 10, 975, 60, Cr2, TRUE);
    }
    DrawFormatString(760, 10, Cr3, "HP%.0f/%.0f", HP, MAXHP);
    DrawBox(140, 60, 560, 110, GetColor(100,100,100), TRUE);//経験値ゲージの枠表示
    DrawBox(150, 70, 150 + EXP, 100, Cr4, TRUE);//経験値ゲージの表示
    if (EXP >= 100) {
        DrawBox(570, 60, 910, 110, GetColor(100, 100, 100), TRUE);
    }
    else {
        DrawBox(570, 60, 860, 110, GetColor(100, 100, 100), TRUE);
    }
    DrawFormatString(570, 60, Cr4, "経験値%d/400", EXP);
    //DrawCircle(70, 70, 70, Cr, TRUE);
    // 読みこんだグラフィックを拡大描画
    DrawExtendGraph(-10, -15, 150, 150, GHandle, TRUE);// アイコンを描画
    DrawFormatString(140, 110, Cr3, "Lv%d", Level);//レベルの表示
    if (EXP >= 400) {
        EXP = 0;
        Level++;
    }

    if (Level > Level2) {
        AttackPoint += 10;
        AttackPointUP += 10;
        HP = MAXHP;
    }

    Level2 = Level;

    if(Level < 3 && command && !command2) {
        DrawLine(35, 425, 720, 775, Cr);
        DrawLine(690, 450, 60, 750, Cr);
        DrawString(230, 650, "LV3で解放", GetColor(255, 0, 0));
    }

    if (Level < 5 && command && !command2) {
        DrawLine(1280, 425, 1980, 800, Cr);
        DrawLine(1920, 450, 1310, 750, Cr);
        DrawString(1500, 650, "LV5で解放", GetColor(255, 0, 0));
    }

    //HPが0になると死亡する
    if (HP <= 0 && !dead) {
        anim_id = 3;
        animatorPtr->Play(MHandle, anim_id);
        dead = true;
    }
    if (Hit) {
        Hitcnt++;
    }
    if (Hitcnt > 100) {
        Hit = false;
        Hitcnt = 0;
    }

    if (EHit) {
        EHitcnt++;
    }
    if (EHitcnt > 50) {
        EHit = false;
        EHitcnt = 0;
    }

    if (effect1) {
        Effectcnt++;
    }

    if (effect2) {
        Effectcnt++;
    }

    if (Effectcnt > 25 && effect2) {
        effectManagerPtr->Play(4, trans);
        effect2 = false;
        Effectcnt = 0;
    }

    if (Effectcnt > 25 && effect1) {
        effectManagerPtr->Play(4, trans);
        effect1 = false;
        Effectcnt = 0;
    }
    if (dash) {
        trans.x -= sinf(rotation.y) * 50;
        trans.z -= cosf(rotation.y) * 50;
        animatorPtr->SetPlaySpeed(5);
        anim_id = 1;
        animatorPtr->Play(MHandle, anim_id);
        //animatorPtr->SetPlaySpeed(1);
    }
    if (dash && IsWallHit) {
        trans.x += sinf(rotation.y) * 50;
        trans.z += cosf(rotation.y) * 50;
    }
    if (speedup) {
        PowerUpcnt++;
    }
    if (PowerUpcnt > 1000) {
        speedup = false;
        PowerUpcnt = 0;
        speedUP = 0;
        AnimSpeed = 0;
    }
}
void Player3::AttackCollision() {

    typedef struct _AttackCollider {
        string startNodeName;
        string endNodeName;
    } AttackCollider;

    vector<AttackCollider> attackCollider = {
        { "RigJaw" ,"RigJaw" },
    };

    for (int i = 0; i < attackCollider.size(); i++) {
        Player3Attack* attackPtr = new Player3Attack();
        attackPtr->name = "PAttack";
        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        colliderPtr->colliderType = Collider::CAPSULE_TYPE;
        attackPtr->frameIndices[0] = MV1SearchFrame(MHandle, attackCollider[i].startNodeName.c_str());
        attackPtr->frameIndices[1] = MV1SearchFrame(MHandle, attackCollider[i].endNodeName.c_str());
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);
        colliderPtr->radius = 30.f;
        colliderPtr->SetGameObject(attackPtr);

        attackVec.push_back(colliderPtr);
    }
}


void Player3::transform_collider() {
    for (int i = 0; i < attackVec.size(); i++) {
        Player3Attack* attackPtr = (Player3Attack*)attackVec[i];

        CapsuleCollider* colliderPtr = (CapsuleCollider*)attackPtr;
        MATRIX matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[0]);
        colliderPtr->StartPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

        matrix = MV1GetFrameLocalWorldMatrix(MHandle, attackPtr->frameIndices[1]);
        colliderPtr->EndPos = VGet(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

        //攻撃の時にコライダーの名前を変える
        if (isAttack && !command || isAttack2) {
            attackPtr->name = "PAttackHit";
            if (anim_id == 1) {
                attackPtr->name = "PAttackHit+";
            }
        }
        //コライダーの名前を戻す
        else {
            attackPtr->name = "PAttack";
        }
    }
}

void Player3::SetupCollision(CollisionManager* managerPtr) {
    collisionManagerPtr = managerPtr;

    //当たり判定をコリジョンマネージャーに登録
    vector<Collider*>colliderVec = GetCollider();
    for (int i = 0; i < colliderVec.size(); i++) {
        collisionManagerPtr->Add(colliderVec[i]);
    }
}

void Player3::Update() {
    
    Move();
    Jamp();
    Attack();
    Status();
    Camera();
    transform_collider();

    VECTOR tempPosition1;
    VECTOR tempPosition2;
    VECTOR cameraLookAtPosition;

    // 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
    cameraLookAtPosition = trans;
    cameraLookAtPosition.y += 60.f;

    // カメラの位置はカメラの水平角度と垂直角度から算出
    {
        float sinParam;
        float cosParam;

        // 最初に垂直角度を反映した位置を算出
        sinParam = sinf(cameraVAngle / 180.0f * DX_PI_F);
        cosParam = cosf(cameraVAngle / 180.0f * DX_PI_F);
        tempPosition1.x = 0.0f;
        tempPosition1.y = sinParam * 350.f;
        tempPosition1.z = -cosParam * 350.f;

        // 次に水平角度を反映した位置を算出
        sinParam = sinf(cameraHAngle / 180.0f * DX_PI_F);
        cosParam = cosf(cameraHAngle / 180.0f * DX_PI_F);
        tempPosition2.x = cosParam * tempPosition1.x - sinParam * tempPosition1.z *1.1;
        tempPosition2.y = tempPosition1.y * 1.3;
        tempPosition2.z = sinParam * tempPosition1.x + cosParam * tempPosition1.z * 1.1;
    }

    // 算出した座標に注視点の位置を加算したものがカメラの位置
    cameraPosition = VAdd(tempPosition2, cameraLookAtPosition);

    // カメラの設定に反映する
    SetCameraPositionAndTarget_UpVecY(cameraPosition, VGet(trans.x, trans.y + 200, trans.z));


#if 0
#else
    // トランスフォームマトリクスを作る
    Transform(MHandle);
#endif

    MV1DrawModel(MHandle);

    // アニメーション再生更新
    if (!animatorPtr->Update()) {
        // アニメーション再生が終了しニュートラルへ戻った
        isAttack = false;
        isAttack2 = false;
        Step = false;
        command2 = false;
        notAttack = false;
        SoundFlag = false;
        SoundFlag2 = false;
        SoundFlag3 = false;
        dash = false;
    }
    P3 = true;

    if (skill1 || skill2 || skill3) {
        showcnt++;
    }

    if (showcnt >= 30) {
        skill1 = false;
        skill2 = false;
        skill3 = false;
        showcnt = 0;
    }

    // ※：ポイント　カプセルの始点と終点座標を更新
    StartPos = trans;
    EndPos = StartPos;
    EndPos.y += 60.f;

}

void Player3::OnCollisionHit(Collider* colliderPtr, GameObject* gobjPtr) {
    //ボスの攻撃を食らうとダメージを受ける
    if (gobjPtr->name == "Weapon" && !Hit) {
        HP -= 50;
        Hit = true;
    }

    if (gobjPtr->name == "Boss" && !IsWallHit) {
        VECTOR pos = VGet(-forward.x * 5.f, 0.f, -forward.z * 5.f);
        trans = VAdd(trans, pos);
    }

    if (gobjPtr->name == "EAttack") {
        VECTOR pos = VGet(-forward.x * 5.f, 0.f, -forward.z * 5.f);
        trans = VAdd(trans, pos);
        if (!EHit) {
            HP -= 15;
            EHit = true;
        }
    }
}

void Player3::SetupEffectManager(EffectManager* ptr) {
    effectManagerPtr = ptr;
}

