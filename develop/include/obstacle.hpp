#pragma once
#include "object/collidable_object.hpp"

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

private:
    static constexpr float kSpeed = 24.f;

private:
    unsigned int color_; /// @brief カプセルの色: DEBUG用

};