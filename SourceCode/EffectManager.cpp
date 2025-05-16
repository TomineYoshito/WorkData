#include "EffectManager.h"

EffectManager::EffectManager() {
	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

EffectManager::~EffectManager() {
	effectVec.clear();
	effectVec.shrink_to_fit();
	Effkseer_End();
}

// CSVファイルからエフェクシアアセット情報を読み込む
void EffectManager::LoadCSV(string csv_file_name) {
	ifstream reading_file;
	string filename = "Assets/Effect/CSV/EffectList.csv";
	reading_file.open(filename, std::ios::in);
	string reading_line_buffer;

	while (std::getline(reading_file, reading_line_buffer)) {
		vector<string> elementVec = Common::split(reading_line_buffer, ',');

		// elementVec[0] = ID
		// elementVec[1] = efkefcファイル
		Effect* effectPtr = new Effect();

		effectPtr->effect_id = atoi(elementVec[0].c_str());
		effectPtr->filename = elementVec[1];
		effectPtr->scale = (float)atof(elementVec[2].c_str());

		// エフェクトリソースを読み込む。
		// 読み込む時に大きさを指定する。
		effectPtr->effectResourceHandle = LoadEffekseerEffect(effectPtr->filename.c_str(), effectPtr->scale);
		effectVec.push_back(effectPtr);
	}

	reading_file.close();
}

void EffectManager::Play(int id, VECTOR pos) {
	// エフェクトを再生する。
	playingEffectHandle = PlayEffekseer3DEffect(effectVec[id]->effectResourceHandle);
	// 再生中のエフェクトを移動する。
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
}

void EffectManager::Update() {
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void EffectManager::Draw() {

}