#include <DxLib.h>
#include "misc/assert_dx.hpp"
#include "app/game_time.hpp"
#include "app/scene_manager.hpp"
#include "app/input_manager.hpp"
#include "component/physics.hpp"
#include "scene/scene.hpp"

Scene::~Scene()
{
}

bool Scene::Initialize()
{
    time_ = 0.0f;

    try {
        for (auto&& obj : objects_) {
            obj.second->Initialize();
        }

        for (auto&& obj : objects_) {
            obj.second->Activate();
        }
    }
    catch (...) {
        return false;
    }

    return true;
}

void Scene::Finalize()
{
    for (auto&& obj : objects_) {
        obj.second->Finalize();
    }
}

void Scene::Update() noexcept
{
    time_ += GameTime::GetDeltaTime();

    for (auto&& obj : objects_) {
        if (!obj.second->IsActive()) {
            continue;
        }

        obj.second->Update();
    }

    Physics::GetInstance().Update();
}

void Scene::Draw() const noexcept
{
    for (auto&& obj : objects_) {
        if (!obj.second->IsActive()) {
            continue;
        }

        obj.second->Draw();
    }
}

void Scene::Debug() const noexcept
{
#ifdef _DEBUG
    std::string str = "";
    switch (tag_) {
    case SceneTag::Title:
        str = "TitleScene";     break;
    case SceneTag::Stage:
        str = "StageScene";     break;
    case SceneTag::Result:
        str = "ResultScene";    break;
    }

    printfDx("\n%s Time: %.1f", str.c_str(), time_);
    printfDx("\n");
    printfDx("パッド接続数：%d\n", GetJoypadNum());
    printfDx("\n");

    for (auto&& obj : objects_) {
        auto& name = obj.second->GetName();
        auto& pos  = obj.second->GetTransform()->GetWorldPosition();

        if (CheckHitKey(KEY_INPUT_TAB)) {
            DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetForward(), 5.0f)), 0x0000ff);
            DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetRight(),   5.0f)), 0xff0000);
            DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetUp(),	   5.0f)), 0x00ff00);
        }

        printfDx("%s... x: %.2f, y: %.2f, z: %.2f. \n", name.c_str(), pos.x, pos.y, pos.z);
    }
    printfDx("\n");

    auto spot = GetLightPositionHandle(DX_LIGHTTYPE_SPOT);
    printfDx("SPOT:		{ %.2f, %.2f, %.2f }\n", spot.x, spot.y, spot.z);

    auto point = GetLightPositionHandle(DX_LIGHTTYPE_POINT);
    printfDx("POINT:	   { %.2f, %.2f, %.2f }\n", point.x, point.y, point.z);

    auto direct = GetLightPositionHandle(DX_LIGHTTYPE_DIRECTIONAL);
    printfDx("DIRECTIONAL: { %.2f, %.2f, %.2f }\n", direct.x, direct.y, direct.z);

#endif // DEBUG
}

void Scene::LastUpdate() noexcept
{
    for (auto&& obj : objects_) {
        obj.second->LastUpdate();
    }
}

void Scene::CheckSceneState() noexcept
{
    /// no processing on base class.
}

void Scene::RegisterObject(std::shared_ptr<GameObject> object, const std::string& name) noexcept
{
    objects_[name] = object;
}

std::shared_ptr<GameObject> Scene::GetSceneObject(const std::string& name) noexcept
{
    return std::shared_ptr<GameObject>();
}

void Scene::DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string name) noexcept
{
    dont_destroy_objects_[name] = object;
}

std::unordered_map<std::string, std::shared_ptr<GameObject>> Scene::GetDontDestroyObjects() noexcept
{
    return dont_destroy_objects_;
}

void Scene::ChangeScene(const SceneTag& tag) noexcept
{
    manager_.ChangeScene(tag);
}
