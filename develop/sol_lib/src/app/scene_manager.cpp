#include "misc/assert_dx.hpp"
#include "app/scene_manager.hpp"

bool SceneManager::instantiated_ = false;

SceneManager::SceneManager()
{
	ASSERT(instantiated_ == 0, "already instantiated.");
	instantiated_ = true;
}

SceneManager::~SceneManager()
{
	for (auto& scene : scenes_) {
		scene.second->Finalize();
	}

	instantiated_ = false;
}

void SceneManager::Update() noexcept
{
	current_scene_->Update();
}

void SceneManager::Draw() noexcept
{
	current_scene_->Draw();
}

void SceneManager::Debug() noexcept
{
#ifdef _DEBUG
	current_scene_->Debug();
	auto length_half = 100.f;
	DrawLine3D(VGet(0, 0, 0), VGet(length_half, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, length_half, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, length_half), GetColor(0, 0, 255));
#endif 
}

void SceneManager::LastUpdate() noexcept
{
	current_scene_->LastUpdate();
	current_scene_->CheckSceneState();
}

void SceneManager::ChangeScene(const std::string& tag)
{
	if (current_scene_->GetTag() == tag) return;

	//! ƒV[ƒ“ˆÚ“®‘O‰Šú‰»
	bool succeed = false;
	try {
		succeed = scenes_[tag]->Initialize();
	}
	catch (...) {
		ASSERT(succeed, "scene initialize failure.");
	}

	current_scene_ = scenes_[tag];
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& tag) const noexcept
{
	if (!scenes_.count(tag)) {
		return nullptr;
	}
	
	return scenes_.at(tag);
}
