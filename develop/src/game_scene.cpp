#include "misc/assert_dx.hpp"
#include "game_scene.hpp"

//! GameScene Object
#include "skydome.hpp"
#include "player.hpp"

GameScene::GameScene(SceneManager& manager)
	: Scene("game_scene", manager)
{
	auto player = std::make_shared<Player>();
	if (player) {
		Scene::RegisterObject(player, "player");
	}

	auto skydome = std::make_shared<Skydome>();
	if (skydome) {
		Scene::RegisterObject(skydome, "skydome");
	}

	//auto floor = std::make_shared<Floor>(VGet(0.f, 1.f, 0.f), VGet(5.f, 1.f, 5.f));
	//if (floor) {
	//	Scene::RegisterObject(floor, "floor");
	//}
}

void GameScene::CheckSceneState() noexcept
{
	//! TODO: Conditions for Change Scene.
}

bool GameScene::Initialize() noexcept
{	
	//! Easy Camera Implementation.
	static float cam_position = 4.f;
	/*
	if (CheckHitKey(KEY_INPUT_UP)) {
		cam_position++;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		cam_position--;
	}
	*/
	SetCameraPositionAndTarget_UpVecY(VGet(0.f, cam_position, 5.f), VGet(0.f, 0.f, -5.f));

	return Scene::Initialize();
}