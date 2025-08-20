#pragma once
#include "object/character_object.hpp"

/// @brief プレイヤー
class Player : public CharacterObject
{
private:
    /// @brief アニメーション種類
    enum class Animation
    {
        kIdle,
        kRun,
        kPaceUp,
        kJumpUp,
        kJumpDown,
    };

public:
    /// @brief コンストラクタ
    Player();

    /// @brief デストラクタ
    ~Player() override;

    /// @brief 初期化
    void Initialize()   noexcept override;

    /// @brief 破棄
    void Finalize()     noexcept override;

    /// @brief 更新
    void Update()       noexcept override;

    /// @brief 描画
    void Draw()   const noexcept override;

    /// @brief 最終更新
    void LastUpdate()   noexcept override;

    /// @brief 衝突判定
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    /// @brief ジャンプ処理
    void HandleJump();

    /// @brief 入力処理
    void UpdateInput();

private:
    static constexpr float kJumpInitialPower = 2.f;    /// @brief ジャンプ初速
    static constexpr float kPlayerHeight     = 1.7f;    /// @brief 身長
    static constexpr float kPlayerWaist      = 0.3f;    /// @brief 横幅

private:
    bool         is_jumping_;                           /// @brief ジャンプ中か
    bool         jump_input_;                           /// @brief ジャンプ入力
    unsigned int color_;                                /// @brief カプセルの色: DEBUG用

};