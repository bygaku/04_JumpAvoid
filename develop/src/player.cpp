#include <DxLib.h>
#include "app/game_time.hpp"
#include "app/input_manager.hpp"
#include "player.hpp"
#include "component/physics.hpp"
#include "misc/collider/collider_info_capsule.hpp"

Player::Player()
    : CharacterObject("player")
    , is_jumping_(false)
    , jump_input_(false)
    , color_(GetColor(0, 255, 0))
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
    //! Physicsの追加等
    CharacterObject::Initialize();

    //! Transform
    transform_->SetLocalPosition(VGet(0.f, 0.f, 0.f));
    transform_->SetLocalScale(0.01f);

    //! Rigidbody
    rigidbody_->SetUseGravity(false);

    //! Collider Capsule
    auto capsule_collider = std::dynamic_pointer_cast<ColliderInfoCapsule>(GetColliderInfo());
    if (capsule_collider) {
        auto bottom_point = kPlayerWaist;
        auto top_point    = kPlayerHeight - kPlayerWaist;

        capsule_collider->SetCapsule(
            VGet(0.f, bottom_point, 0.f),
            VGet(0.f, top_point,    0.f),
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

        animator_->AddAnimationHandle(static_cast<int>(Animation::kRun),      "../resources/animation/run_normal.mv1",   "run_normal",   40.0f, TRUE);
        animator_->AddAnimationHandle(static_cast<int>(Animation::kPaceUp),   "../resources/animation/run_pace_up.mv1",  "pace_up",      30.0f, TRUE);
        animator_->AddAnimationHandle(static_cast<int>(Animation::kJumpDown), "../resources/animation/jumping_down.mv1", "jumping_down", 20.0f, FALSE);
        animator_->AddAnimationHandle(static_cast<int>(Animation::kJumpUp),   "../resources/animation/jumping_up.mv1",   "jumping_up",   20.0f, FALSE);

        animator_->AttachAnimation(static_cast<int>(Animation::kRun));
    }
}

void Player::Finalize() noexcept
{
    CharacterObject::Finalize();
}

void Player::Update() noexcept
{
    if (!IsActive()) return;

    UpdateInput();

    HandleJump();

    GetColliderInfo()->UpdateAABB(
        transform_->GetLocalPosition(),
        transform_->GetWorldMatrix()
    );

    animator_->Update();
    model_->SetMatrix(transform_->GetLocalMatrix());
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
        printfDx("Player->IsGrounded():TRUE\n");
    }
    else {
        printfDx("Player->IsGrounded():FALSE\n");
    }
#endif //! カプセルを描画
}

void Player::LastUpdate() noexcept
{
    // 今回は特に処理なし
}

void Player::OnCollide(const CollidableObject& object) noexcept
{
    //! 床との衝突
    if (object.GetTag() == ObjectTag::Floor) {
        printfDx("On Collide with Floor\n");
    }
    else
    {
        printfDx("Non Collide\n");
    }
}

void Player::UpdateInput()
{
    auto& input = InputManager::GetInstance();
    jump_input_ = input.IsKeyPressed(KeyCode::B);
}

void Player::HandleJump()
{
    //! ジャンプ可能時処理
    if (jump_input_) {
        rigidbody_->SetGrounded(false);
        is_jumping_ = true;

        //! 初速を与える
        VECTOR vel = rigidbody_->GetVelocity();
        vel.y      = kJumpInitialPower;
        rigidbody_->SetVelocity(vel);

        //! ジャンプ開始時の色変更
        color_ = 0xff0000;
    }
    else {
        rigidbody_->SetGrounded(true);
    }

    //! 下降中はジャンプ終了
    if (is_jumping_ && rigidbody_->GetVelocity().y < 0) {
        is_jumping_ = false;
    }

    //! 色の更新
    if (!rigidbody_->IsGrounded()) {
        if (!is_jumping_) {
            color_ = 0x0000ff;  // 落下中
        }
    }
    else {
        color_ = 0x00ff00;  // 接地中
    }
}