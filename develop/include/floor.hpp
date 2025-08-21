#pragma once
#include "object/collidable_object.hpp"
#include "component/modeler.hpp"

/// @brief 床オブジェクト（静的）
class Floor final : public CollidableObject
{
public:
    /// @brief インスタンスを初期化
    Floor(const VECTOR& position, const VECTOR& size);

    /// @brief インスタンスを解放
    ~Floor() override;

    /// @brief オブジェクトを初期化
    void Initialize()   noexcept override;

    /// @brief オブジェクト破棄処理
    void Finalize()     noexcept override;

    /// @brief オブジェクトを更新
    void Update()       noexcept override;

    /// @brief オブジェクトを描画
    void Draw()   const noexcept override;

    /// @brief オブジェクトの最終更新
    void LastUpdate()   noexcept override;

    /// @brief 衝突判定
    void OnCollide(const CollidableObject& object) noexcept override;

private:
    VECTOR size_;                       /// @brief 床のサイズ
    int    texture_;                    /// @brief テクスチャハンドル

    std::shared_ptr<Modeler> model_;    /// @brief モデル管理コンポーネント

};