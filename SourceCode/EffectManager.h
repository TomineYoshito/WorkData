#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "Common.h"
#include "EffekseerForDXLib.h"

using namespace std;

typedef struct _Effect {
	int effect_id;
	string filename;
	float scale;
	int effectResourceHandle;
} Effect;

class EffectManager : public GameObject {
public:
	EffectManager();
	~EffectManager();

	void Update();
	void Draw();
	void Play(int id, VECTOR pos);
	void LoadCSV(string csv_file_name);

private:
	int effectResourceHandle;
	int playingEffectHandle;

	vector<Effect*> effectVec;
};


