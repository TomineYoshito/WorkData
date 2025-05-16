#pragma once
#include "Dxlib.h"
#include <string>
#include <vector>

using namespace std;

typedef struct _CollisionRect {
	VECTOR min_pos;
	VECTOR max_pos;
} CollisionRect;

typedef struct _CollisionNode {
	int frameIndex;		// �t���[���C���f�b�N�X
	string name;		// �m�[�h�i�t���[���j��
	bool visible;		// �R���W�������肷�邵�Ȃ��iTRUE�F�R���W�������肷��AFALSE�F�R���W�������肵�Ȃ��j
	bool static_node;	// ���W�ϊ��𖈃t���[�����{���邩�ǂ����iTRUE:���W�ϊ��͂P�x�����AFALSE�F���t���[�����W�ϊ��j
} CollisionNode;

class Collision
{
private:
	string nodeName;
	int modelHandle;
	vector<CollisionNode*> colFrameVec;
	vector<CollisionNode*> colWallFrameVec;

	void traverse(int parent_index, string root_name);

public:
	Collision();
	~Collision();

	void SetName(string node_name) {
		nodeName = node_name;
	}

	bool CheckAABB_AABB(CollisionRect rect1, CollisionRect rect2);
	bool CheckRectPoint(CollisionRect rect, VECTOR pos);
	bool CheckRaycast(VECTOR startPos, VECTOR forward, const float ray_cast_length, VECTOR* hitPos);

	void SetModelHandle(int handle);

	static void DrawRect(CollisionRect rect1) {
		float x = rect1.max_pos.x - rect1.min_pos.x;
		float y = rect1.max_pos.y - rect1.min_pos.y;
		float z = rect1.max_pos.z - rect1.min_pos.z;

		VECTOR pos1 = VGet(rect1.min_pos.x + x, rect1.min_pos.y, rect1.min_pos.z);
		VECTOR pos2 = VGet(rect1.min_pos.x, rect1.min_pos.y + y, rect1.min_pos.z);

		DrawLine3D(rect1.min_pos, rect1.max_pos, GetColor(255, 0, 0));	// �΂ߐ�
		DrawLine3D(rect1.min_pos, pos1, GetColor(255, 0, 0));			// ������
		DrawLine3D(pos2, rect1.max_pos, GetColor(255, 0, 0));			// �㕔��

		pos1 = VGet(rect1.min_pos.x, rect1.min_pos.y + y, rect1.min_pos.z);
		DrawLine3D(rect1.min_pos, pos1, GetColor(255, 0, 0));			// ������

		pos2 = VGet(rect1.max_pos.x, rect1.max_pos.y - y, rect1.min_pos.z);
		DrawLine3D(pos2, rect1.max_pos, GetColor(255, 0, 0));			// �E����
	}
};
