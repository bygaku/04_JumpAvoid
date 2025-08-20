#pragma once
#include "scene/scene.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager);

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;
};
