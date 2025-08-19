#pragma once
#include "component/transform.hpp"
#include "component/data/physics_material.hpp"

/// @brief 物理計算データ
class Rigidbody final
{
public:
    /// @brief インスタンスを初期化
    Rigidbody(Transform& transform);

    /// @brief インスタンスを解放
    ~Rigidbody();

    /// @brief オブジェクトを初期化
    void Initialize() noexcept;

    /// @brief 向きを取得
    const VECTOR& GetDirection()      const noexcept { return direction_;       }

    /// @brief 移動量を取得
    const VECTOR& GetVelocity()       const noexcept { return velocity_;        }

    /// @brief 質量を取得
    const float&  GetMass()           const noexcept { return mass_;            }
    
    /// @brief 物理材質を取得
    const PhysicsMaterial& 
                  GetMaterial()       const noexcept { return material_;        }

    /// @brief 接地面の法線を取得
    const VECTOR& GetGroundNormal()   const noexcept { return ground_normal_;   }

    /// @brief 重力をかけるオブジェクト判別を取得
    bool          OnGravity()         const noexcept { return use_gravity_;     }

    /// @brief 次フレームの座標を取得
    const VECTOR& GetFuturePosition() const noexcept { return future_pos_;      }

    /// @brief 移動量を設定
    void SetVelocity  (const VECTOR& velocity)        noexcept;

    /// @brief 質量を設定
    void SetMass      (float mass)                    noexcept;

    /// @brief このオブジェクトに重力をかける
    void SetUseGravity(bool gravity)		          noexcept;

    /// @brief 物理材質を設定
    void SetMaterial(const PhysicsMaterial& material) noexcept;

    /// @brief 接地面の法線を設定
    void SetGroundNormal(const VECTOR& normal)        noexcept;

    /// @brief 継続的な力を加える
    void AddForce     (const VECTOR& force_direction) noexcept;

    /// @brief 次フレームの座標を保存
    void SaveFuturePosition(const VECTOR& future_pos) noexcept;

#pragma region 非推奨
    /// @brief 接地状態を設定
    void SetGrounded(bool is_grounded) noexcept { is_grounded_ = is_grounded; }

    /// @brief 接地しているか
    bool IsGrounded()            const noexcept { return is_grounded_;        }
#pragma endregion

private:
    PhysicsMaterial material_;          /// @brief 物理材質
    Transform&      owner_transform_;   /// @brief Transform参照

    VECTOR	        direction_;			/// @brief 向き
    VECTOR	        velocity_;			/// @brief 速度ベクトル
    VECTOR          future_pos_;        /// @brief 次フレームの座標
    bool	        use_gravity_;		/// @brief 重力をかけるオブジェクト
    float           mass_;              /// @brief 質量

    bool            is_grounded_;       /// @brief 接地判定
    VECTOR          ground_normal_;     /// @brief 接地面の法線

};
