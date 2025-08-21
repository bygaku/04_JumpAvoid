#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "misc/math/math.hpp"
#include "app/game_time.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_all.hpp"

using namespace dx_math;

Physics* Physics::instance_ = nullptr;

Physics& Physics::GetInstance()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    return *instance_;
}

void Physics::Create()
{
    ASSERT(DxLib_IsInit() != 0, "DxLib isn't initialize.");
    ASSERT(instance_ == nullptr, "instance has been created.");
    instance_ = new Physics();
}

void Physics::Destroy()
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");
    delete instance_;
}

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::Entry(CollidableObject* collidable)
{	
    ASSERT(instance_, "haven't been created instance.");

    //! collidable ���z����Ɍ������ back ���s��
    bool found = std::find(collidables_.begin(), collidables_.end(), collidable) != collidables_.end();
    if (found) {
        ASSERT(found, "the specified object is already registered.");
        return;
    }
    
    collidables_.emplace_back(collidable);
}

void Physics::Exit(CollidableObject* collidable)
{
    //! collidable ���z����Ɍ������ erase ���s��
    auto count = std::erase_if(collidables_, 
                              [collidable](CollidableObject* target) { 
                              return target == collidable; }
    );

    if (count <= 0) {
        ASSERT(count == 0, "the specified object isn't registered.");
    }
}

void Physics::Update() noexcept
{
    /* HACK: EARLY RETURN;
    *  if (GameTime::GetTimeScaler() < 0.01f) return;
    */
    auto dt = GameTime::GetDeltaTime();
    if (dt < 0.0001f) return;

    //! �ړ�����
    for (auto& object : collidables_) {
        if (!object->IsActive()) continue;
        if (object->GetColliderInfo()->GetCategory() == ColliderInfo::Category::kStatic) continue;

        //! Rigidbody 
        auto rd	= object->GetRigidbody();
        if (!rd) continue;

        VECTOR position = object->GetTransform()->GetLocalPosition();
        VECTOR velocity = rd->GetVelocity();

        //! �d�͂� Velocity �ɉ�����
        if (rd->OnGravity()) {
            //! Gravity: 1.f m/s^2
            velocity.y += kGravity * dt;

            if (!rd->IsGrounded()) {
                //! ���R�����̏I�[�����x
                velocity.y = std::clamp(velocity.y, kMaxFreefallSpeed, 0.f);
            }
        }

        //! ���C��������
        //if (rd->IsGrounded()) {
        //    ApplyFriction(rd, dt);
        //    velocity = rd->GetVelocity();
        //}

        rd->SaveFuturePosition(position + velocity * dt);
    }

    //! �Փ˃`�F�b�N
    auto on_collide_info = CheckCollide();

    //! ���W�C��
    FixPosition();

    //! �Փ˒ʒm
    for (auto& object : on_collide_info) {
        object.owner->OnCollide(*object.collider);
    }
}

std::vector<Physics::OnCollideInfo> Physics::CheckCollide() const noexcept
{
    std::vector<OnCollideInfo> on_collide_info;
    bool	do_check = true;
    int		count	 =	  0;	/// @brief �`�F�b�N��

    //! �Փ˒ʒm�A���W�C��
    while (do_check) {
        do_check = false;
        ++count;

        /// HACK: Player����̋������m�F, �œK�����ׂ�
        //! �I�u�W�F�N�g��������
        for (size_t i = 0; i < collidables_.size(); ++i) {
            for (size_t j = i + 1; j < collidables_.size(); ++j) {
                auto* obj_a = collidables_[i];
                auto* obj_b = collidables_[j];

                //! �ǂ�����Î~�I�u�W�F�N�g�̏ꍇ, ���肵�Ȃ�
                if (obj_a->GetWeight() == CollidableObject::Weight::Static &&
                    obj_b->GetWeight() == CollidableObject::Weight::Static) {
                    continue;
                }

                //! �Փˏ���
                if (IsCollide(obj_a, obj_b)) {
                    auto weight_a = obj_a->GetWeight();
                    auto weight_b = obj_b->GetWeight();

                    CollidableObject* higher = obj_a;		/// @brief �d���F��
                    CollidableObject* lower = obj_b;		/// @brief �d���F��

                    bool are_trigger = obj_a->GetColliderInfo()->IsTrigger() || obj_b->GetColliderInfo()->IsTrigger();

                    //! �g���K�[�łȂ���Ύ��ڕW�ʒu�C��
                    if (!are_trigger) {
                        if (weight_a < weight_b) {
                            higher = obj_b;
                            lower = obj_a;
                        }

                        FixFuturePosition(higher, lower);
                    }

                    /// @note ���x���Ă΂��\�����l��, ��U�x�������p.
                    bool has_higher_info = false;
                    bool has_lower_info = false;

                    //! �Փ˒ʒm���̍X�V
                    for (const auto& object : on_collide_info) {
                        //! ���ɒʒm���X�g�Ɋ܂܂�Ă���
                        if (object.owner == higher) has_higher_info = true;
                        if (object.owner == lower)  has_lower_info = true;
                    }

                    if (!has_higher_info) {
                        on_collide_info.push_back({ higher, lower });
                    }
                    if (!has_lower_info) {
                        on_collide_info.push_back({ lower, higher });
                    }

                    //! ��x�ł��q�b�g+�␳������Փ˔���ƕ␳���Ȃ���
                    if (!are_trigger) do_check = true;// �Е����g���K�[�Ȃ�q�b�g�Ƃ�Ȃ����Ȃ�
                }
            }
        }

        //! ����ȏ�͔�����s��Ȃ�
        if (count > kMaxHitCheck && do_check) {
#if _DEBUG
            printfDx("Loop exceeded the maximum number of evaluations.\n");
#endif
            break;
        }
    }
    return on_collide_info;
}

bool Physics::IsCollide(const CollidableObject* obj_a, const CollidableObject* obj_b) const noexcept
{
    //! �܂��J�e�S���Ńt�B���^�����O
    if (!obj_a->GetColliderInfo()->CanCollideWith(obj_b->GetColliderInfo().get())) {
        return false;
    }

    //! �X���[�^�O�̃`�F�b�N
    if (obj_a->IsThroughTarget(obj_b) || obj_b->IsThroughTarget(obj_a)) {
        return false;
    }

    //! AABB�ő�܂��Ȕ���
    const AABB& aabb_a = obj_a->GetColliderInfo()->GetAABB();
    const AABB& aabb_b = obj_b->GetColliderInfo()->GetAABB();

    if (!aabb_a.Intersects(aabb_b)) {
        return false;
    }

    //! �ڍׂȏՓ˔���
    VECTOR hit_point;
    VECTOR hit_normal;
    float penetration;

    auto rb_a = obj_a->GetRigidbody();
    auto rb_b = obj_b->GetRigidbody();

    //! ���t���[���̈ʒu�Ŕ���
    VECTOR pos_a = rb_a ? rb_a->GetFuturePosition() : obj_a->GetTransform()->GetLocalPosition();
    VECTOR pos_b = rb_b ? rb_b->GetFuturePosition() : obj_b->GetTransform()->GetLocalPosition();

    return obj_a->GetColliderInfo()->TestCollision(
        obj_b->GetColliderInfo().get(),
        pos_a,
        pos_b,
        hit_point,
        hit_normal,
        penetration
    );
}

void Physics::FixFuturePosition(CollidableObject* higher, CollidableObject* lower) const noexcept
{
    //! �����蔻��̎�ʂ��ƂɏC�����@��ύX
    auto higher_type = higher->GetColliderInfo()->GetType();
    auto lower_type  =  lower->GetColliderInfo()->GetType();
    auto higher_rd	 = higher->GetRigidbody();
    auto lower_rd	 =  lower->GetRigidbody();

    //! �ÓI�I�u�W�F�N�g��Rigidbody�������Ȃ��\��
    if (!lower_rd) return;

    //! �ڍׂȏՓˏ����擾
    VECTOR hit_point;
    VECTOR hit_normal;
    float  penetration;

    VECTOR pos_higher = higher_rd ? higher_rd->GetFuturePosition() : higher->GetTransform()->GetLocalPosition();
    VECTOR pos_lower = lower_rd->GetFuturePosition();

    bool hit = higher->GetColliderInfo()->TestCollision(
        lower->GetColliderInfo().get(),
        pos_higher,
        pos_lower,
        hit_point,
        hit_normal,
        penetration
    );

    if (!hit) return;

    //! �d���ɉ����������o���䗦���v�Z
    float weight_ratio = 1.0f;
    if (higher->GetWeight() != CollidableObject::Weight::Static) {
        float mass_higher = higher_rd ? higher_rd->GetMass() : 1000000.0f;  // �ÓI�͔��ɏd��
        float mass_lower = lower_rd->GetMass();
        weight_ratio = mass_higher / (mass_higher + mass_lower);
    }

    //! �����o���x�N�g��
    VECTOR push_vector = VScale(hit_normal, penetration);

    //! �Ⴂ�d���̃I�u�W�F�N�g�������o��
    VECTOR new_pos = pos_lower + VScale(push_vector, weight_ratio);
    lower_rd->SaveFuturePosition(new_pos);

    //! �����d���̃I�u�W�F�N�g�����������߂��i�ÓI�łȂ��ꍇ�j
    if (higher_rd && higher->GetWeight() != CollidableObject::Weight::Static) {
        VECTOR higher_new_pos = pos_higher - VScale(push_vector, 1.0f - weight_ratio);
        higher_rd->SaveFuturePosition(higher_new_pos);
    }

    //! �ڒn����
    //! �@����������ŁA�������Ɉړ����Ă����ꍇ�͐ڒn
    if (hit_normal.y > 0.7f && lower_rd->GetVelocity().y < 0) {
        lower_rd->SetGrounded(true);

        //! Y�����̑��x�����Z�b�g
        VECTOR vel = lower_rd->GetVelocity();
        vel.y = 0.0f;
        lower_rd->SetVelocity(vel);
    }
}

void Physics::FixPosition() noexcept
{
    for (auto& object : collidables_) {
        auto rd = object->GetRigidbody();
        if (!rd) continue;

        //! �ʒu�̍������瑬�x���t�Z
        VECTOR position_diff = rd->GetFuturePosition() - object->GetTransform()->GetLocalPosition();

        float dt = GameTime::GetDeltaTime();
        if (dt > 0.0001f) {
            //! �ړ��ʍX�V
            VECTOR fix_velocity = position_diff * (1.0f / dt);
            rd->SetVelocity(fix_velocity);
        }

        //! ���W�X�V
        object->GetTransform()->SetLocalPosition(rd->GetFuturePosition());
    }
}

void Physics::ApplyGravity() noexcept
{
}

void Physics::ApplyFriction(const std::shared_ptr<Rigidbody>& rb, float delta_time) noexcept
{
    VECTOR velocity		 = rb->GetVelocity();
    VECTOR ground_normal = rb->GetGroundNormal();

    //! �n�ʂɕ��s�ȑ��x�������v�Z
    auto normal_dot_velocity = VDot(velocity, ground_normal);
    auto velocity_parallel   = velocity - (ground_normal * normal_dot_velocity);
    auto speed				 = VSize(velocity_parallel);
    if (speed < 0.001f) return;

    //! ���C�͂̌v�Z
    auto& material			   = rb->GetMaterial();
    auto  friction_coefficient = material.dynamic_friction;

    //! �Ö��C�̔���
    if (speed < 0.1f) {
        friction_coefficient = material.static_friction;
    }

    //! ���C�� = �� * N�i�����R�́j
    auto normal_force   = rb->GetMass() * fabs(kGravity);
    auto friction_force = friction_coefficient * normal_force;

    //! ���C�ɂ�錸��
    auto friction_direction = VNorm(velocity_parallel) * -1.f;
    auto friction_accel     = friction_direction * (friction_force / rb->GetMass());

    //! ���x���X�V�i���C�Ŋ��S�Ɏ~�܂�Ȃ��悤�ɐ����j
    auto new_velocity_parallel = velocity_parallel + (friction_accel * delta_time);

    //! ���C�ŋt�����ɂȂ����ꍇ�͒�~
    if (VDot(new_velocity_parallel, velocity_parallel) < 0) {
        new_velocity_parallel = VGet(0.f, 0.f, 0.f);
    }

    //! ���������ƍ������đ��x��ݒ�
    VECTOR velocity_normal = ground_normal * normal_dot_velocity;
    rb->SetVelocity(new_velocity_parallel + velocity_normal);
}
