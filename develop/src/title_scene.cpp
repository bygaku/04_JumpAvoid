#include "misc/assert_dx.hpp"
#include "app/game_time.hpp"
#include "app/input_manager.hpp"
#include "title_scene.hpp"

//! TitleObject
#include "skydome.hpp"
#include "button.hpp"
#include "misc/data/screen_data.hpp"

TitleScene::TitleScene(SceneManager& manager)
	: Scene("title_scene", manager)
	, logo_handle_(-1)
	, door_handle_(-1)
	, button_state_(0x0001)
	, change_timer_(0.f)
{
	auto skydome = std::make_shared<Skydome>();
	if (skydome) {
		auto& transform = skydome->GetTransform();
		transform->SetLocalScale(0.1f);
		Scene::RegisterObject(skydome, "skydome");
	}

	logo_handle_ = LoadGraph("../resources/icon/logo.png");
	door_handle_ = LoadGraph("../resources/texture/door.png");

	int margin = 100;
	Vector2D_I size		(500, 50);
	Vector2D_I pos_start(screen::kHalfWidth, screen::kHeight - margin - size.y_);
	Vector2D_I pos_exit (screen::kHalfWidth, screen::kHeight - margin + size.y_);
	auto start_button = std::make_shared<Button>("start_button", pos_start, size);
	auto exit_button  = std::make_shared<Button>("exit_button",  pos_exit,  size);

	if (start_button && exit_button) {
		start_button->RegisterOnClickProcess(title_button::StartButtonPressed);

		Scene::RegisterObject(start_button, "start");
		Scene::RegisterObject(exit_button,  "exit");
	}

	//! Easy Camera Implementation.
	SetCameraNearFar(3.0f, 1000.f);
	SetCameraPositionAndTarget_UpVecY(VGet(0.f, 0.f, -1.f), VGet(0.f, 0.f, 1.f));
}

void TitleScene::CheckSceneState() noexcept
{
	auto& input = InputManager::GetInstance();
	auto  start = std::dynamic_pointer_cast<Button>(Scene::GetSceneObject("start"));
	auto  exit  = std::dynamic_pointer_cast<Button>(Scene::GetSceneObject("exit" ));

	if (!input.IsKeyPressed(KeyCode::Space)) return;
	if (start->IsHover()) {
		start->OnClick();

		if (change_timer_ < 3.f) return;
		Scene::ChangeScene("game_scene");
	}

	if (exit->IsHover()) {
		exit->OnClick();
		Scene::ChangeScene("result_scene");
	}
}

bool TitleScene::Initialize() noexcept
{
	button_state_ = 0x0001;
	change_timer_ = 0.f;

	return Scene::Initialize();
}

void TitleScene::Update() noexcept
{
	//! オブジェクトの取得 =======================
	auto skydome = Scene::GetSceneObject("skydome");
	if (skydome) {
		auto& transform = skydome->GetTransform();
		transform->Rotate(Quaternion::FromEulerDegrees(0.f, 0.01f, 0.f));
	}

	auto start	 = Scene::GetSceneObject("start");
	auto exit	 = Scene::GetSceneObject("exit");


	//! ボタンの更新 ============================
	ButtonState();

	if (button_state_ & (1 << 0)) /* Start Select */ {
		std::dynamic_pointer_cast<Button>(start)->SetHover();
		std::dynamic_pointer_cast<Button>(exit )->SetHover(false);
	}

	if (button_state_ & (1 << 1)) /* Exit  Select */ {
		std::dynamic_pointer_cast<Button>(exit )->SetHover();
		std::dynamic_pointer_cast<Button>(start)->SetHover(false);
	}
	
	//! シーンオブジェクト更新処理 ================
	Scene::Update();

	//! 描画 ===================================
	skydome	->Draw();
	start	->Draw();
	exit	->Draw();
}

void TitleScene::Draw() const noexcept
{
	DrawExtendGraph(screen::kHalfWidth - 300, screen::kHalfHeight - 380,
					screen::kHalfWidth + 300, screen::kHalfHeight + 220,
					logo_handle_, true);

	DrawExtendGraph(0, 0, screen::kWidth, screen::kHeight, door_handle_, true);
}

void TitleScene::ButtonState() noexcept
{
	auto&		input = InputManager::GetInstance();
	
	if (input.IsKeyPressed(KeyCode::Down) || input.IsKeyPressed(KeyCode::Up)) {
		button_state_ <<= 1;
		//! ボタンが kButtonNum を指したとき
		if (button_state_ & (1 << kButtonNum)) {
			button_state_ = 0x0001;
		}
	}
}

void title_button::StartButtonPressed()
{
	int alpha = 255;
	for (size_t i = 255; i > 0; i--)
	{
		alpha = i;
		SetDrawBright(alpha, alpha, alpha);
	}
}

void title_button::ExitButtonPressed()
{
}
