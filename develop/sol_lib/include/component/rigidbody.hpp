#pragma once
#include "component/transform.hpp"
#include "component/data/physics_material.hpp"

/// @brief �����v�Z�f�[�^
class Rigidbody final
{
public:
    /// @brief �C���X�^���X��������
    Rigidbody(Transform& transform);

    /// @brief �C���X�^���X�����
    ~Rigidbody();

    /// @brief �I�u�W�F�N�g��������
    void Initialize() noexcept;

    /// @brief �������擾
    const VECTOR& GetDirection()      const noexcept { return direction_;       }

    /// @brief �ړ��ʂ��擾
    const VECTOR& GetVelocity()       const noexcept { return velocity_;        }

    /// @brief ���ʂ��擾
    const float&  GetMass()           const noexcept { return mass_;            }
    
    /// @brief �����ގ����擾
    const PhysicsMaterial& 
                  GetMaterial()       const noexcept { return material_;        }

    /// @brief �ڒn�ʂ̖@�����擾
    const VECTOR& GetGroundNormal()   const noexcept { return ground_normal_;   }

    /// @brief �d�͂�������I�u�W�F�N�g���ʂ��擾
    bool          OnGravity()         const noexcept { return use_gravity_;     }

    /// @brief ���t���[���̍��W���擾
    const VECTOR& GetFuturePosition() const noexcept { return future_pos_;      }

    /// @brief �ړ��ʂ�ݒ�
    void SetVelocity  (const VECTOR& velocity)        noexcept;

    /// @brief ���ʂ�ݒ�
    void SetMass      (float mass)                    noexcept;

    /// @brief ���̃I�u�W�F�N�g�ɏd�͂�������
    void SetUseGravity(bool gravity)		          noexcept;

    /// @brief �����ގ���ݒ�
    void SetMaterial(const PhysicsMaterial& material) noexcept;

    /// @brief �ڒn�ʂ̖@����ݒ�
    void SetGroundNormal(const VECTOR& normal)        noexcept;

    /// @brief �p���I�ȗ͂�������
    void AddForce     (const VECTOR& force_direction) noexcept;

    /// @brief ���t���[���̍��W��ۑ�
    void SaveFuturePosition(const VECTOR& future_pos) noexcept;

#pragma region �񐄏�
    /// @brief �ڒn��Ԃ�ݒ�
    void SetGrounded(bool is_grounded) noexcept { is_grounded_ = is_grounded; }

    /// @brief �ڒn���Ă��邩
    bool IsGrounded()            const noexcept { return is_grounded_;        }
#pragma endregion

private:
    PhysicsMaterial material_;          /// @brief �����ގ�
    Transform&      owner_transform_;   /// @brief Transform�Q��

    VECTOR	        direction_;			/// @brief ����
    VECTOR	        velocity_;			/// @brief ���x�x�N�g��
    VECTOR          future_pos_;        /// @brief ���t���[���̍��W
    bool	        use_gravity_;		/// @brief �d�͂�������I�u�W�F�N�g
    float           mass_;              /// @brief ����

    bool            is_grounded_;       /// @brief �ڒn����
    VECTOR          ground_normal_;     /// @brief �ڒn�ʂ̖@��

};
