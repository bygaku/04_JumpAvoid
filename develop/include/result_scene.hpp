#pragma once
#include "scene/scene.hpp"

class ResultScene final : public Scene
{
public:
	ResultScene(SceneManager& manager);

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;

	void Update()		   noexcept override;
	void Draw()		 const noexcept override;
};
