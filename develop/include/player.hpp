#pragma once
#include "object/character_object.hpp"

/// @brief プレイヤー
class Player : public CharacterObject
{
private:
    /// @brief プレイヤー状態
    enum class State
    {
        kNone,
        kIdle,
        kRun,
        kPaceUp,
        kJumping,
        kFalling,
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
    /// @brief ジャンプによる上昇処理
    void OnJumping(VECTOR& velo);

    /// @brief 下降処理
    void OnFalling(VECTOR& velo);

    /// @brief 入力によるステートの更新
    void UpdateStateFromInput(VECTOR& velo);

private:
    static constexpr float kJumpForce       = 24.f;     /// @brief ジャンプ初速力 (km/h)
    static constexpr float kJumpSustained   = 0.7f;     /// @brief ジャンプ滞空力 (km/frame)
    static constexpr float kMaxPressCount   = 0.5f;     /// @brief ジャンプ入力最大値 (seconds)
    static constexpr float kPlayerHeight    = 1.7f;     /// @brief 身長
    static constexpr float kPlayerWaist     = 0.3f;     /// @brief 横幅

private:
    State        state_;

    bool         is_jumping_;                           /// @brief ジャンプ中か
    bool         jump_pressed_;                         /// @brief ジャンプ入力
    float        count_press_time_;                     /// @brief 入力の時間をカウント

    unsigned int color_;                                /// @brief カプセルの色: DEBUG用
    float        gravity_ = -0.98f;
};