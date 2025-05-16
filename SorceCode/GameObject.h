#pragma once
#include "Dxlib.h"
#include <math.h>
#include <string>
#include <vector>

using namespace std;

class GameObject
{
public:
	VECTOR trans;
	VECTOR rotation;
	VECTOR scale;
    VECTOR BoxPos1;
    VECTOR BoxPos2;
    bool visible;
    string name;
    string tag_name;

    GameObject() {
        visible = true;
    }

    ~GameObject() {}

    void Transform(int handle);

    MATRIX GetMatrix() {
        return matrix;
    }


    VECTOR GetPos() {
        return trans;
    }

    VECTOR GetForward() {
        return forward;
    }

    void SetVisible(bool flag) {
        visible = flag;
    }

    virtual void Update() = 0;  // 仮想関数。派生クラスで必ず実装しないといけない

protected:
    MATRIX matrix;
    VECTOR forward;
};

