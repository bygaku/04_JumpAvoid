#include "misc/assert_dx.hpp"
#include "app/input_manager.hpp"
#include "game_scene.hpp"

//! GameScene Object
#include "skydome.hpp"
#include "obstacle.hpp"
#include "player.hpp"
#include "floor.hpp"

GameScene::GameScene(SceneManager& manager)
	: Scene("game_scene", manager)
{
	auto player = std::make_shared<Player>();
	if (player) {
		Scene::RegisterObject(player, "player");
	}

	auto floor = std::make_shared<Floor>(VGet(0.f, -1.f, 0.f), VGet(3.5f, 1.f, 5.f));
	if (floor) {
		Scene::RegisterObject(floor, "floor");
	}

	auto skydome = std::make_shared<Skydome>();
	if (skydome) {
		auto& transform = skydome->GetTransform();
		transform->SetLocalScale(0.05f);
		Scene::RegisterObject(skydome, "skydome");
	}

	auto obstacle = std::make_shared<Obstacle>("spike");
	if (obstacle) {
		Scene::RegisterObject(obstacle, "spike");
	}
}

void GameScene::CheckSceneState() noexcept
{
	auto& input = InputManager::GetInstance();

	if (input.IsKeyPressed(KeyCode::Space)) {
		Scene::ChangeScene("game_scene");
	}
	if (input.IsKeyPressed(KeyCode::Enter)) {
		Scene::ChangeScene("result_scene");
	}
}

bool GameScene::Initialize() noexcept
{	
	//! Easy Camera Implementation.
	static float cam_position = 4.f;
	SetCameraPositionAndTarget_UpVecY(VGet(-10.f, cam_position, -7.f), VGet(10.f, 4.f, -7.f));

	return Scene::Initialize();
}

void GameScene::Update() noexcept
{
	auto skydome = Scene::GetSceneObject("skydome");
	if (skydome) {
		auto& transform = skydome->GetTransform();
		transform->Rotate(Quaternion::FromEulerDegrees(0.f, 0.01f, 0.f));
	}

	//auto obstacle = Scene::GetSceneObject("spike");
	//if (obstacle) {
	//	auto& transform = obstacle->GetTransform();
	//	transform->Rotate(Quaternion::FromEulerDegrees(0.01f, 0.01f, 0.01f));
	//}

	Scene::Update();
}
