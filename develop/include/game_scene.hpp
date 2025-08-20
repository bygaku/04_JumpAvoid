#pragma once
#include "scene/scene.hpp"

class GameScene final : public Scene
{
public:
	GameScene(SceneManager& manager);

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;

	bool Initialize() noexcept override;
};
