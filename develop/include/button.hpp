#pragma once
#include <cstdint>
#include <functional>
#include "object/game_object.hpp"

/// @remark 命名
using ON_CLICK_CALLBACK = std::function<void()>;
using ON_HOVER_CALLBACK = std::function<void()>;

class Button : public GameObject
{
public:
	/// @brief インスタンス初期化
	Button(const std::string& name, Vector2D_I pos, Vector2D_I size);

	/// @brief インスタンス解放
	~Button() override = default;

    /// @brief 初期化
    void Initialize() noexcept override;

    /// @brief 破棄処理
    void Finalize()   noexcept override;

    /// @brief 更新
    void Update()	  noexcept override;

    /// @brief 描画
    void Draw() const noexcept override;

    /// @brief描画後更新
    void LastUpdate() noexcept override;

    /// @brief ボタンがクリックされたとき
    void OnClick()    noexcept;

    /// @brief ボタンがホバー中か
    void OnHover()     noexcept;

    /// @brief ホバー状態の設定
    void SetHover(bool is_hover = true) noexcept;

    /// @brief ホバー状態を取得
    bool IsHover() const noexcept { return is_hover_; };

    /// @brief クリック時の処理を登録
    void RegisterOnClickProcess(const ON_CLICK_CALLBACK& process) noexcept;

    /// @brief ホバー時の処理を登録
    void RegisterOnHoverProcess(const ON_HOVER_CALLBACK& process) noexcept;

private:
    Vector2D_I center_pos_;         /// @brief 中央座標
    Vector2D_I size_;               /// @brief サイズ

    ON_CLICK_CALLBACK on_click_;    /// @brief クリック処理関数
    ON_HOVER_CALLBACK on_hover_;    /// @brief ホバー処理関数
    bool              is_click_;    /// @brief ボタンクリック
    bool              is_hover_;    /// @brief ボタンセレクト
};
