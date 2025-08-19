#pragma once
#include <functional>
#include <map>

/// @brief 状態管理インターフェイス
/// @tparam StateType 状態の種類
template <typename StateType> 
class StateMachine
{
public:
	using STATE_DELEGATE = std::function<void(void)>;

private:
	/// @brief 状態管理
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

	StateType							  current_state_;	/// @brief 現在の状態
	std::map<StateType, StateDelegateSet> state_funcs_;		/// @brief 状態関数ポインタ

	bool is_initialized_ = false;

public:
	/// @brief 状態登録
	void RegisterState(StateType state, STATE_DELEGATE update, STATE_DELEGATE exit)
	{
		//! state が配列内に見つからなければ back を行う
		bool found = std::find(state_funcs_.begin(), state_funcs_.end(), state) != state_funcs_.end();
		if (found) {
			ASSERT(found, "the specified StateType is already registered.");
		}

		StateDelegateSet set{};
		set.state  = state;
		set.update = update;
		state_funcs_.emplace(state, set);
	}

	/// @brief 状態を設定
	void SetState(StateType state)
	{
		if (current_state_ != state) return;
		current_state_ = state;
	}

	/// @brief 現在の状態のみ更新
	void Update()
	{
		stateFuncMap[current_state_].update();
	}

	// 現在の状態の取得
	StateType GetCurrentState() const { return current_state_; }
};

