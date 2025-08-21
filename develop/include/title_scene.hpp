#pragma once
#include "scene/scene.hpp"
#include "button.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager);

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;

	bool Initialize()	   noexcept override;
	void Update()		   noexcept override;
	void Draw()		 const noexcept override;

private:
	/// @brief ボタンの条件
	void ButtonState() noexcept;

private: 
	int		 logo_handle_;
	int		 door_handle_;
	uint16_t button_state_;
	float    change_timer_;

	const int kButtonNum = 2;
};

/// @brief タイトルのボタン
namespace title_button {
	/// @brief スタートボタンが押された
	void StartButtonPressed();

	/// @brief ゲームを退出する
	void ExitButtonPressed();
}
