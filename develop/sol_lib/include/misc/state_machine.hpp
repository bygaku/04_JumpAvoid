#pragma once
#include <functional>
#include <map>

/// @brief ��ԊǗ��C���^�[�t�F�C�X
/// @tparam StateType ��Ԃ̎��
template <typename StateType> 
class StateMachine
{
public:
	using STATE_DELEGATE = std::function<void(void)>;

private:
	/// @brief ��ԊǗ�
	struct StateDelegateSet
	{
	public:
		StateType	   state;
		STATE_DELEGATE update;

		StateDelegateSet()
			: state(StateType()) 
		{
		}
	};

	StateType							  current_state_;	/// @brief ���݂̏��
	std::map<StateType, StateDelegateSet> state_funcs_;		/// @brief ��Ԋ֐��|�C���^

	bool is_initialized_ = false;

public:
	/// @brief ��ԓo�^
	void RegisterState(StateType state, STATE_DELEGATE update, STATE_DELEGATE exit)
	{
		//! state ���z����Ɍ�����Ȃ���� back ���s��
		bool found = std::find(state_funcs_.begin(), state_funcs_.end(), state) != state_funcs_.end();
		if (found) {
			ASSERT(found, "the specified StateType is already registered.");
		}

		StateDelegateSet set{};
		set.state  = state;
		set.update = update;
		state_funcs_.emplace(state, set);
	}

	/// @brief ��Ԃ�ݒ�
	void SetState(StateType state)
	{
		if (current_state_ != state) return;
		current_state_ = state;
	}

	/// @brief ���݂̏�Ԃ̂ݍX�V
	void Update()
	{
		stateFuncMap[current_state_].update();
	}

	// ���݂̏�Ԃ̎擾
	StateType GetCurrentState() const { return current_state_; }
};

