#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "object/game_object.hpp"

class SceneManager;

class Scene abstract
{
public:
    /// @brief �֎~�R���X�g���N�^
    Scene() = delete;

    /// @brief �e�p�����[�^���w�肵�ăC���X�^���X��������
    Scene(const std::string& tag, SceneManager& manager) noexcept
        : manager_(manager)
        , tag_(tag)
        , time_(0.0f)
    {
    }

    /// @brief �C���X�^���X�����
    virtual	~Scene();

    /// @brief ������
    virtual bool Initialize();

    /// @brief �j������
    virtual void Finalize();

    /// @brief ���t���[���X�V
    virtual void Update()		   noexcept;

    /// @brief ���t���[���`��
    virtual void Draw()		 const noexcept;

    /// @brief ���t���[���f�o�b�O�`��
    virtual void Debug()	 const noexcept;

    /// @brief �`���X�V
    virtual void LastUpdate()	   noexcept;

    /// @brief �V�[���̏�Ԃ��m�F
    virtual void CheckSceneState()														 noexcept;

    /// @brief �V�[���ɃI�u�W�F�N�g��o�^
    void RegisterObject(std::shared_ptr<GameObject> object, const std::string& name)     noexcept;

    /// @brief �o�^���ꂽ�I�u�W�F�N�g���擾
    std::shared_ptr<GameObject> GetSceneObject(const std::string& name)				     noexcept;

    /// @brief ���ׂẴV�[���ɓK������I�u�W�F�N�g��ǉ�
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string name)		 noexcept;

    /// @brief ���ׂẴV�[���ɓK������I�u�W�F�N�g���擾
    std::unordered_map<std::string, std::shared_ptr<GameObject>> GetDontDestroyObjects() noexcept;

    const std::string& GetTag() const noexcept { return tag_;  }

protected:
    SceneManager& manager_;
    
    /// @brief �V�[����؂�ւ�
    void ChangeScene(const std::string& tag) noexcept;

    /// @brief �V�[���̃^�C�����擾
    const float& GetSceneTime() const noexcept { return time_; }

private:
    std::string tag_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> objects_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> dont_destroy_objects_;

    float time_;

};
