#pragma once
#include "scene/scene.hpp"
#include "button.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager);

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;

	bool Initialize()	   noexcept override;
	void Update()		   noexcept override;
	void Draw()		 const noexcept override;

private:
	/// @brief �{�^���̏���
	void ButtonState() noexcept;

private: 
	int		 logo_handle_;
	int		 door_handle_;
	uint16_t button_state_;
	float    change_timer_;

	const int kButtonNum = 2;
};

/// @brief �^�C�g���̃{�^��
namespace title_button {
	/// @brief �X�^�[�g�{�^���������ꂽ
	void StartButtonPressed();

	/// @brief �Q�[����ޏo����
	void ExitButtonPressed();
}
