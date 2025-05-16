#include "GameObject.h"

void GameObject::Transform(int handle) {
    // XYZの回転行列を作る
    MATRIX matrix;
    MATRIX matRotX = MGetRotX(rotation.x);  // X軸回転行列を作る
    MATRIX matRotY = MGetRotY(rotation.y);  // Y軸回転行列を作る
    MATRIX matRotZ = MGetRotZ(rotation.z);  // Z軸回転行列を作る

    // ①Z軸回転行列ｘY軸回転行列
    matrix = MMult(matRotZ, matRotY);

    // ②ZY軸回転行列ｘX軸回転行列
    matrix = MMult(matrix, matRotX);

    // ③拡縮の行列を作る
    MATRIX matScl = MGetScale(VGet(0.2f, 0.2f, 0.2f));

    // ④拡縮行列ｘXYZ回転行列
    matrix = MMult(matScl, matrix);

    // ⑤XYZ回転拡縮行列ｘ平行移動行列
    MATRIX matTrans = MGetTranslate(trans);
    matrix = MMult(matrix, matTrans);

    // 最終的な座標変換行列をモデルに適用
    MV1SetMatrix(handle, matrix);

    // 現在の回転マトリクスを使って前向きベクトルを作る
    MATRIX rotMatrix = MGetIdent();
    rotMatrix.m[0][0] = matrix.m[0][0]; rotMatrix.m[0][1] = matrix.m[0][1]; rotMatrix.m[0][2] = matrix.m[0][2];
    rotMatrix.m[1][0] = matrix.m[1][0]; rotMatrix.m[1][1] = matrix.m[1][1]; rotMatrix.m[1][2] = matrix.m[1][2];
    rotMatrix.m[2][0] = matrix.m[2][0]; rotMatrix.m[2][1] = matrix.m[2][1]; rotMatrix.m[2][2] = matrix.m[2][2];
    forward = VNorm(VTransform(VGet(0.f, 0.f, -1.f), rotMatrix));
}
