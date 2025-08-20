#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "object/game_object.hpp"

class SceneManager;

class Scene abstract
{
public:
    /// @brief 禁止コンストラクタ
    Scene() = delete;

    /// @brief 各パラメータを指定してインスタンスを初期化
    Scene(const std::string& tag, SceneManager& manager) noexcept
        : manager_(manager)
        , tag_(tag)
        , time_(0.0f)
    {
    }

    /// @brief インスタンスを解放
    virtual	~Scene();

    /// @brief 初期化
    virtual bool Initialize();

    /// @brief 破棄処理
    virtual void Finalize();

    /// @brief 毎フレーム更新
    virtual void Update()		   noexcept;

    /// @brief 毎フレーム描画
    virtual void Draw()		 const noexcept;

    /// @brief 毎フレームデバッグ描画
    virtual void Debug()	 const noexcept;

    /// @brief 描画後更新
    virtual void LastUpdate()	   noexcept;

    /// @brief シーンの状態を確認
    virtual void CheckSceneState()														 noexcept;

    /// @brief シーンにオブジェクトを登録
    void RegisterObject(std::shared_ptr<GameObject> object, const std::string& name)     noexcept;

    /// @brief 登録されたオブジェクトを取得
    std::shared_ptr<GameObject> GetSceneObject(const std::string& name)				     noexcept;

    /// @brief すべてのシーンに適応するオブジェクトを追加
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string name)		 noexcept;

    /// @brief すべてのシーンに適応するオブジェクトを取得
    std::unordered_map<std::string, std::shared_ptr<GameObject>> GetDontDestroyObjects() noexcept;

    const std::string& GetTag() const noexcept { return tag_;  }

protected:
    SceneManager& manager_;
    
    /// @brief シーンを切り替え
    void ChangeScene(const std::string& tag) noexcept;

    /// @brief シーンのタイムを取得
    const float& GetSceneTime() const noexcept { return time_; }

private:
    std::string tag_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> objects_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> dont_destroy_objects_;

    float time_;

};
