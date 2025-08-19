#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "scene/scene.hpp"
#include "app/scene_manager.hpp"
#include "component/physics.hpp"

#include "title.hpp"
#include "stage.hpp"

bool SceneManager::instantiated_ = false;

SceneManager::SceneManager()
{
	ASSERT(instantiated_ == 0, "already instantiated.");
	instantiated_ = true;
	Physics::Create();

	scenes_[SceneTag::Title]  = std::make_shared<TitleScene>(*this);
	scenes_[SceneTag::Stage]  = std::make_shared<StageScene>(*this);

	current_scene_ = scenes_[SceneTag::Title];
	current_scene_->Initialize();
}

SceneManager::~SceneManager()
{
	/// すべてのシーンを削除
	for (auto& scene : scenes_) {
		scene.second->Finalize();
	}

	Physics::Destroy();
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

#ifdef _DEBUG
void SceneManager::Debug() noexcept
{
	auto length_half = 100.f;
	DrawLine3D(VGet(0, 0, 0), VGet(length_half, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, length_half, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, length_half), GetColor(0, 0, 255));

	current_scene_->Debug();
}
#endif 

void SceneManager::LastUpdate() noexcept
{
	current_scene_->LastUpdate();
	current_scene_->CheckSceneState();
}

void SceneManager::ChangeScene(const SceneTag& tag)
{
	if (current_scene_->GetTag() == tag) return;

	//! シーン移動前初期化
	bool succeed = false;
	try {
		succeed = scenes_[tag]->Initialize();
	}
	catch (...) {
		ASSERT(succeed, "scene initialize failure.");
	}

	current_scene_ = scenes_[tag];
}

std::shared_ptr<Scene> SceneManager::GetScene(const SceneTag& tag) const noexcept
{
	if (!scenes_.count(tag)) {
		return nullptr;
	}
	
	return scenes_.at(tag);
}
