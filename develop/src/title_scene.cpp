#include "misc/assert_dx.hpp"
#include "app/input_manager.hpp"
#include "title_scene.hpp"

//! TitleObject
#include "skydome.hpp"

TitleScene::TitleScene(SceneManager& manager)
	: Scene("title_scene", manager)
{
	auto skydome = std::make_shared<Skydome>();
	if (skydome) {
		Scene::RegisterObject(skydome, "skydome");
	}

	//! Easy Camera Implementation.
	SetCameraNearFar(3.0f, 2000.f);
}

void TitleScene::CheckSceneState() noexcept
{
	auto& input = InputManager::GetInstance();

	if (input.IsKeyPressed(KeyCode::Space)) {
		Scene::ChangeScene("game_scene");
	}
}
