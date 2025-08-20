#pragma once
#include "scene/scene.hpp"

class GameScene final : public Scene
{
public:
	GameScene(SceneManager& manager);

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;

	bool Initialize() noexcept override;
};
