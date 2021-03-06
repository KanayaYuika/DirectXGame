#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete sprite_; 
    delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	//X,Y,Z方向のスケーリングを設定
	 worldTransform_.scale_={5.0f,5.0f,5.0f};
	 //X,Y,Z軸周りの回転角を設定
	 worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};
	//X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {
	////スプライトの今の座標を取得
	//XMFLOAT2 position = sprite_->GetPosition();
	////座標を｛２，０｝移動
	//position.x += 2.0f;
	//position.y += 2.0f;
	////移動した座標をスプライトに反映
	//sprite_->SetPosition(position);

	//スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		//音声停止
		audio_->StopWave(voiceHandle_);
	}
	//デバッグテキストの表示
	// debugText_->Print("*aiueo*", 50, 50, 1.0f);
	//変数の値をインクリメント
	//value_++;
	//値を含んだ文字列
	/*std::string strDebug = std::string("Value:") + std::to_string(value_);
	debugText_->Print(strDebug, 50, 50, 1.0f);*/
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);

	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "scale:(%f,%f,%f)", worldTransform_.scale_.x, worldTransform_.scale_.y,
	  worldTransform_.scale_.z);
}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
