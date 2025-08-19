#pragma once

/// @brief 物理材質データ
struct PhysicsMaterial {
    float static_friction;   /// @brief 静止摩擦係数　(0.0 ~ 1.0)
    float dynamic_friction;  /// @brief 動摩擦係数　　(0.0 ~ 1.0)
    float restitution;       /// @brief 反発係数　　　(0.0 ~ 1.0)

    /// @brief デフォルト材質
    static PhysicsMaterial Default() {
        return { 0.6f, 0.4f, 0.0f };
    }

    /// @brief 滑る材質
    static PhysicsMaterial Slip() {
        return { 0.05f, 0.03f, 0.0f };
    }
};