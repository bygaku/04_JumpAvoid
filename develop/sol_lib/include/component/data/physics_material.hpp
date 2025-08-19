#pragma once

/// @brief �����ގ��f�[�^
struct PhysicsMaterial {
    float static_friction;   /// @brief �Î~���C�W���@(0.0 ~ 1.0)
    float dynamic_friction;  /// @brief �����C�W���@�@(0.0 ~ 1.0)
    float restitution;       /// @brief �����W���@�@�@(0.0 ~ 1.0)

    /// @brief �f�t�H���g�ގ�
    static PhysicsMaterial Default() {
        return { 0.6f, 0.4f, 0.0f };
    }

    /// @brief ����ގ�
    static PhysicsMaterial Slip() {
        return { 0.05f, 0.03f, 0.0f };
    }
};