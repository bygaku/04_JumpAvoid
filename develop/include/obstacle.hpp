#pragma once
#include "object/collidable_object.hpp"
#include "component/modeler.hpp"

/// @brief 障害物
class Obstacle : public CollidableObject
{
public:
    /// @brief コンストラクタ
    Obstacle(const std::string& name);

    /// @brief デストラクタ
    ~Obstacle() override;

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

    /// @brief 座標初期化
    void ResetStartPosition();

    /// @brief 画面外に出たか判定
    /// @return 画面外なら true
    bool IsOutOfScreen() const noexcept;

    /// @brief 移動速度を設定
    void SetMoveSpeed(float speed) noexcept { move_speed_ = speed; }
private:
    static constexpr float kInitalizeSpeed = 5.f;
    static constexpr float kDespawnZ       = 5.0f;
    static constexpr float kSpawnZ         = -20.0f;

private:
    unsigned int             color_; /// @brief カプセルの色: DEBUG用
    std::shared_ptr<Modeler> model_;           ///< モデル管理

    float move_speed_;               ///< 移動速度（z方向）
};