#pragma once
#include "scene/scene.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager);

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;
};
