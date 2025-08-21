#include <DxLib.h>
#include "app/game_time.hpp"
#include "app/input_manager.hpp"
#include "player.hpp"
#include "misc/collider/collider_info_capsule.hpp"

Player::Player()
    : CharacterObject("player")
    , is_jumping_(false)
    , jump_pressed_(false)
    , count_press_time_(0.f)
    , color_(GetColor(0, 255, 0))
    , state_(State::kNone)
{
    tag_      = ObjectTag::Player;
    model_    = std::make_shared<Modeler> (*this);
    animator_ = std::make_shared<Animator>(*this, model_);
}

Player::~Player()
{
}

void Player::Initialize() noexcept
{
    //! Physics�̒ǉ���
    CharacterObject::Initialize();

    //! Transform
    transform_->SetLocalPosition(VGet(0.f, 0.f, 0.f));
    transform_->SetLocalScale(0.012f);

    //! Rigidbody
    rigidbody_->SetUseGravity(false);
    rigidbody_->SetGrounded(true);

    //! Collider Capsule
    auto capsule_collider = std::dynamic_pointer_cast<ColliderInfoCapsule>(GetColliderInfo());
    if (capsule_collider) {
        auto bottom_point = kPlayerWaist                 + 0.5f;
        auto top_point    = kPlayerHeight - kPlayerWaist + 0.5f;

        capsule_collider->SetCapsule(
            VGet(0.f, bottom_point, 0.f),
            VGet(0.f, top_point   , 0.f),
            kPlayerWaist
        );
    }

    //! Modeler
    if (model_) {
        model_->Initialize();
        model_->SetModelHandle("../resources/model/character/mannequin.mv1");
    }

    //! Animator
    if (animator_) {
        animator_->Initialize();
        animator_->SetAnimationBlendSpeed(3.0f);

        animator_->AddAnimationHandle(static_cast<int>(State::kRun),     "../resources/animation/run_normal.mv1",   "run_normal", 45.0f, TRUE);
        animator_->AddAnimationHandle(static_cast<int>(State::kPaceUp),  "../resources/animation/run_pace_up.mv1",  "pace_up",    30.0f, TRUE);
        animator_->AddAnimationHandle(static_cast<int>(State::kJumping), "../resources/animation/jumping_up.mv1",   "jumping",    40.0f, FALSE);
        animator_->AddAnimationHandle(static_cast<int>(State::kFalling), "../resources/animation/jumping_down.mv1", "falling",    40.0f, FALSE);

        animator_->AttachAnimation(static_cast<int>(State::kRun));
    }
    
    state_ = State::kRun;
}

void Player::Finalize() noexcept
{
    CharacterObject::Finalize();
}

void Player::Update() noexcept
{
    if (!IsActive()) return;
    VECTOR velocity = rigidbody_->GetVelocity();

    //! ���͂Ə�ԍX�V
    UpdateStateFromInput(velocity);

    //! �R���C�_�[�X�V
    GetColliderInfo()->UpdateAABB(
        transform_->GetLocalPosition(),
        transform_->GetWorldMatrix()
    );

    printfDx("velocity.y: %f\n", rigidbody_->GetVelocity().y);

    //! �ړ��ʂ�����
    rigidbody_->SetVelocity(velocity);

    //! �e�R���|�[�l���g�X�V
    animator_ ->Update();
    transform_->UpdateWorldTransform();
    model_    ->SetMatrix(transform_->GetLocalMatrix());
}

void Player::Draw() const noexcept
{
    if (!IsActive()) return;
    model_->Draw();

#ifdef _DEBUG
    VECTOR pos = transform_->GetLocalPosition();

    auto bottom_point = kPlayerWaist;
    auto top_point    = kPlayerHeight - kPlayerWaist;
    DrawCapsule3D(
        VAdd(pos, VGet(0.f, bottom_point, 0.f)),
        VAdd(pos, VGet(0.f, top_point,    0.f)),
        kPlayerWaist,
        8,
        color_,
        GetColor(100, 255, 100),
        false
    );

    if (rigidbody_->IsGrounded()) {
        printfDx("IsGrounded(): TRUE\n");
        printfDx("is_jumping_: %d\n", is_jumping_);
    }
    else {
        printfDx("IsGrounded(): FALSE\n");
        printfDx("is_jumping_: %d\n", is_jumping_);
    }

    std::string str_state = "";
    switch (state_) {
    case Player::State::kNone:    str_state = "none"; break;
    case Player::State::kIdle:    str_state = "idle"; break;
    case Player::State::kRun:     str_state = "run";  break;
    case Player::State::kPaceUp:  str_state = "pace"; break;
    case Player::State::kJumping: str_state = "jump"; break;
    case Player::State::kFalling: str_state = "fall"; break;
    }
    printfDx("current state: %s\n\n", str_state.c_str());
#endif //! �J�v�Z����`��
}

void Player::LastUpdate() noexcept
{
    // ����͓��ɏ����Ȃ�
}

void Player::OnCollide(const CollidableObject& object) noexcept
{
    //! ���Ƃ̏Փ�
    if (object.GetTag() == ObjectTag::Floor) {
        printfDx("On Collide with Floor\n");
    }
    else {
        printfDx("Non Collide\n");
    }
}

//================================================================================================
void Player::UpdateStateFromInput(VECTOR& velo)
{
    auto& input = InputManager::GetInstance();

    if (rigidbody_->IsGrounded() && !is_jumping_)       {
        state_  = State::kRun;
    }

    if (input.IsKeyPressed(KeyCode::B) && !is_jumping_) {
        //! ������^����
        velo.y  = kJumpForce;
        state_  = State::kJumping;
        count_press_time_ = 0.f;
    }

    if (rigidbody_->GetVelocity().y < 0 && is_jumping_) {
        state_  = State::kFalling;
    }

    if (is_jumping_) {
        //! �d�͂�������
        velo.y += gravity_;
    }

    switch (state_) {
    case Player::State::kNone: return;
    case Player::State::kIdle: return;
    case Player::State::kRun:
        animator_->AttachAnimation(static_cast<int>(State::kRun));

        return;

    case Player::State::kPaceUp:
        animator_->AttachAnimation(static_cast<int>(State::kPaceUp));

        return;

    case Player::State::kJumping:
        OnJumping(velo);

        return;

    case Player::State::kFalling:
        OnFalling(velo);

        return;
    }
}

void Player::OnJumping(VECTOR& velo)
{
    rigidbody_->SetGrounded(false);
    is_jumping_ = true;

    //! �A�j���[�V�������A�^�b�`
    animator_->AttachAnimation(static_cast<int>(State::kJumping));

    //! �܂�������Ă���Ȃ�ő�l�܂Ōy���㏸
    if (InputManager::GetInstance().IsKeyHeld(KeyCode::B) && count_press_time_ < kMaxPressCount) {
        count_press_time_ += GameTime::GetUnscaledDeltaTime();
        velo.y            += kJumpSustained;
    }
}

void Player::OnFalling(VECTOR& velo)
{
    //! �A�j���[�V�������A�^�b�`
    animator_->AttachAnimation(static_cast<int>(State::kFalling));

    if (transform_->GetLocalPosition().y < 5.0f) {
        animator_->AttachAnimation(static_cast<int>(State::kRun));
    }

    //! �������I��
    if (transform_->GetLocalPosition().y < 0.5f) {
        rigidbody_->SetGrounded(true);
        is_jumping_ = false;

        //! �኱��Y���̃Y�����C��
        auto& pos = transform_->GetLocalPosition();
        transform_->SetLocalPosition(VGet(pos.x, 0.f, pos.z));

        velo.y = 0.f;
    }
}
