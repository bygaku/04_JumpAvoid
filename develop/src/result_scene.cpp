#include "misc/assert_dx.hpp"
#include "app/input_manager.hpp"
#include "result_scene.hpp"

//! TitleObject
#include "skydome.hpp"
#include "button.hpp"

ResultScene::ResultScene(SceneManager& manager)
	: Scene("result_scene", manager)
{
}

void ResultScene::CheckSceneState() noexcept
{
	auto& input = InputManager::GetInstance();

	if (input.IsKeyPressed(KeyCode::Space)) {
		Scene::ChangeScene("title_scene");
	}
}

void ResultScene::Update() noexcept
{
	Scene::Update();
}

void ResultScene::Draw() const noexcept
{
	Scene::Draw();
}
