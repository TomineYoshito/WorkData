#include "EffectManager.h"

EffectManager::EffectManager() {
	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

EffectManager::~EffectManager() {
	effectVec.clear();
	effectVec.shrink_to_fit();
	Effkseer_End();
}

// CSV�t�@�C������G�t�F�N�V�A�A�Z�b�g����ǂݍ���
void EffectManager::LoadCSV(string csv_file_name) {
	ifstream reading_file;
	string filename = "Assets/Effect/CSV/EffectList.csv";
	reading_file.open(filename, std::ios::in);
	string reading_line_buffer;

	while (std::getline(reading_file, reading_line_buffer)) {
		vector<string> elementVec = Common::split(reading_line_buffer, ',');

		// elementVec[0] = ID
		// elementVec[1] = efkefc�t�@�C��
		Effect* effectPtr = new Effect();

		effectPtr->effect_id = atoi(elementVec[0].c_str());
		effectPtr->filename = elementVec[1];
		effectPtr->scale = (float)atof(elementVec[2].c_str());

		// �G�t�F�N�g���\�[�X��ǂݍ��ށB
		// �ǂݍ��ގ��ɑ傫�����w�肷��B
		effectPtr->effectResourceHandle = LoadEffekseerEffect(effectPtr->filename.c_str(), effectPtr->scale);
		effectVec.push_back(effectPtr);
	}

	reading_file.close();
}

void EffectManager::Play(int id, VECTOR pos) {
	// �G�t�F�N�g���Đ�����B
	playingEffectHandle = PlayEffekseer3DEffect(effectVec[id]->effectResourceHandle);
	// �Đ����̃G�t�F�N�g���ړ�����B
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
}

void EffectManager::Update() {
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

void EffectManager::Draw() {

}