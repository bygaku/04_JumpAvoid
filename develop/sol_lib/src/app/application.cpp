#include <DxLib.h>
#include "app/application.hpp"
#include "misc/data/screen_data.hpp"
#include "misc/json_loader.hpp"

//! ツール・ライブラリ管理
#include "app/setting/window_config.hpp"
#include "app/game_time.hpp"
#include "app/input_manager.hpp"
#include "app/scene_manager.hpp"
#include "component/physics.hpp"

//! シーン追加
#include "title_scene.hpp"
#include "game_scene.hpp"
#include "result_scene.hpp"

bool Application::instantiated_ = false;

Application::Application() noexcept
    : is_initialized_(false)
    , game_time_(nullptr)
    , scene_manager_(nullptr)
{
    instantiated_ = true;
}

Application::~Application() noexcept
{
    instantiated_ = false;
}

bool Application::Initialize() noexcept
{
    if (is_initialized_)  return true;

    JsonLoader::Create();

    if (!InitializeDx())  return false;
    if (!InitializeAdx()) return false;

    game_time_      = std::make_unique<GameTime>();
    game_time_->SetFpsTarget(60);

    scene_manager_ = std::make_unique<SceneManager>();
    scene_manager_->RegisterScene<TitleScene>("title_scene");
    scene_manager_->RegisterScene<GameScene>("game_scene");
    scene_manager_->RegisterScene<ResultScene>("result_scene");

    InputManager::Create();
    Physics::Create();

    is_initialized_ = true;
    return true;
}

void Application::Terminate() noexcept
{
    if (!is_initialized_) return;

    if (scene_manager_) {
        scene_manager_.reset();
    }

    if (game_time_) {
        game_time_.reset();
    }

    if (adx_manager_) {
        adx_manager_.reset();
    }

    InputManager::Destroy();
    Physics::Destroy();
    JsonLoader::Destroy();
    DxLib_End();

    is_initialized_ = false;
}

bool Application::IsRunning() const noexcept
{
    return !ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE);
}

void Application::Run() noexcept
{
    game_time_->IncrementFrame();
    InputManager::GetInstance().Update();
    ClearDrawScreen();

    Debug();
    scene_manager_->Update();
    scene_manager_->Draw();
    scene_manager_->LastUpdate();       //! FIXME: 呼ぶ回数を変えてもいいかも

    game_time_->SleepForNextFrame();
    ScreenFlip();
}

void Application::Debug() const noexcept
{
    InputManager::GetInstance().Debug();
    printfDx("application: %.2f\n", game_time_->GetBootTimer());
    scene_manager_->Debug();
}

bool Application::InitializeDx()
{
    auto config_ = WindowConfig{};

    try {
#ifdef _DEBUG
        SetOutApplicationLogValidFlag(TRUE);
#else
        SetOutApplicationLogValidFlag(FALSE);
#endif
        SetWindowText(config_.project_name.c_str());
        ChangeWindowMode(screen::kWindowMode);
        SetGraphMode(screen::kWidth, screen::kHeight, screen::kColorBit);

        //if (config_.is_full_screen) {
        //    int default_width, default_height, default_color_bit;
        //    GetDefaultState(&default_width, &default_height, &default_color_bit);
        //    SetWindowSize(default_width, default_height);
        //}

        SetUseDirect3DVersion(DX_DIRECT3D_11);
        SetUseDirectInputFlag(false);
        SetUseXInputFlag(true);

        if (DxLib_Init() == -1)  return false;

        SetDrawScreen(DX_SCREEN_BACK);

        SetUseZBuffer3D(true);
        SetWriteZBuffer3D(true);
        SetUseBackCulling(true);

        SetFogEnable(true);
        SetFogMode(DX_FOGMODE_LINEAR);
        SetFogColor(255, 200, 150);
        SetFogStartEnd(200.0f, 1000.0f);
        SetFogDensity(0.05f);

        return true;
    }
    catch (...) {
        return false;
    }
}

bool Application::InitializeAdx()
{
    try {
        adx_manager_ = std::make_unique<audio::AdxManager>();

        if (!adx_manager_->Initialize()) {
            adx_manager_.reset();
            return false;
        }

        //! プレイヤーとボイスプール作成
        adx_manager_->CreateVoicePool();
        adx_manager_->CreatePlayer();

        return true;
    }
    catch (...) {
        adx_manager_.reset();
        return false;
    }
}
