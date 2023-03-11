/*
======  https://github.com/cpp11nullptr/lsignal ========================
The MIT License (MIT)

Copyright (c) 2015 Ievgen Polyvanyi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef LSIGNAL_H
#define LSIGNAL_H

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace lSignal
{
	template<int>
	struct placeholder_lSignal
	{
	};
}

namespace std
{
	// custom std::placeholders

	template<int N>
	struct is_placeholder<lSignal::placeholder_lSignal<N>>
		: integral_constant<int, N+1>
	{
	};
}

namespace lSignal
{
	// std::integer_sequence for C++11

	template<int... Ns>
	struct int_sequence
	{
	};

	template<int N, int... Ns>
	struct make_int_sequence
		: make_int_sequence<N-1, N-1, Ns...>
	{
	};

	template<int... Ns>
	struct make_int_sequence<0, Ns...>
		: int_sequence<Ns...>
	{
	};

	// Connection

	struct Connection_data
	{
		bool locked;
	};

	struct Connection_cleaner
	{
		std::function<void(std::shared_ptr<Connection_data>)> deleter;
		std::shared_ptr<Connection_data> data;
	};

	class Connection
	{
		template<typename>
		friend class Signal;

	public:
		Connection(std::shared_ptr<Connection_data>&& data);
		virtual ~Connection();

		bool is_locked() const;
		void set_lock(const bool lock);

		void DisConnect();

	private:
		std::shared_ptr<Connection_data> _data;
		std::vector<Connection_cleaner> _cleaners;

	};

	inline Connection::Connection(std::shared_ptr<Connection_data>&& data)
		: _data(std::move(data))
	{
	}

	inline Connection::~Connection()
	{
	}

	inline bool Connection::is_locked() const
	{
		return _data->locked;
	}

	inline void Connection::set_lock(const bool lock)
	{
		_data->locked = lock;
	}

	inline void Connection::DisConnect()
	{
		decltype(_cleaners) cleaners = _cleaners;

		for (auto iter = cleaners.cbegin(); iter != cleaners.cend(); ++iter)
		{
			const Connection_cleaner& cleaner = *iter;

			cleaner.deleter(cleaner.data);
		}
	}

	// Slot

	class Slot
		: public Connection
	{
	public:
		Slot();
		~Slot() override;

	};

	inline Slot::Slot()
		: Connection(std::shared_ptr<Connection_data>())
	{
	}

	inline Slot::~Slot()
	{
		DisConnect();
	}

	// Signal

	template<typename>
	class Signal;

	template<typename R, typename... Args>
	class Signal<R(Args...)>
	{
	public:
		using result_type = R;
		using callback_type = std::function<R(Args...)>;

		Signal();
		~Signal();

		Signal(const Signal& rhs);
		Signal& operator= (const Signal& rhs);

		Signal(Signal&& rhs) = default;
		Signal& operator= (Signal&& rhs) = default;

		bool is_locked() const;
		void set_lock(const bool lock);

		void Connect(Signal *sg);
		void DisConnect(Signal *sg);

		Connection Connect(const callback_type& fn, Slot *owner = nullptr);
		Connection Connect(callback_type&& fn, Slot *owner = nullptr);

		template<typename T, typename U>
		Connection Connect(T *p, const U& fn, Slot *owner = nullptr);

		void DisConnect(const Connection& Connection);
		void DisConnect(Slot *owner);

		void DisConnect_all();

		R operator() (Args... args);

		template<typename T>
		R operator() (Args... args, const T& agg);

	private:
		struct joint
		{
			callback_type callback;
			std::shared_ptr<Connection_data> Connection;
			Slot *owner;
		};

		mutable std::mutex _mutex;
		bool _locked;

		std::list<joint> _callbacks;

		Signal *_parent;
		std::list<Signal*> _children;

		template<typename T, typename U, int... Ns>
		callback_type construct_mem_fn(const T& fn, U *p, int_sequence<Ns...>) const;

		void copy_callbacks(const std::list<joint>& callbacks);

		std::shared_ptr<Connection_data> create_Connection(callback_type&& fn, Slot *owner);
		void destroy_Connection(std::shared_ptr<Connection_data> Connection);

		Connection prepare_Connection(Connection&& conn);

	};

	template<typename R, typename... Args>
	Signal<R(Args...)>::Signal()
		: _locked(false), _parent(nullptr)
	{
	}

	template<typename R, typename... Args>
	Signal<R(Args...)>::~Signal()
	{
		std::lock_guard<std::mutex> locker(_mutex);

		for (auto iter = _callbacks.begin(); iter != _callbacks.end(); ++iter)
		{
			const joint& jnt = *iter;

			if (jnt.owner != nullptr)
			{
				jnt.owner->_data = nullptr;
				jnt.owner->_cleaners.clear();
			}
		}

		if (_parent != nullptr)
		{
			_parent->_children.remove(this);
		}

		for (Signal *sig : _children)
		{
			sig->_parent = nullptr;
		}
	}

	template<typename R, typename... Args>
	Signal<R(Args...)>::Signal(const Signal& rhs)
		: _locked(rhs._locked)
	{
		std::unique_lock<std::mutex> lock_own(_mutex, std::defer_lock);
		std::unique_lock<std::mutex> lock_rhs(rhs._mutex, std::defer_lock);

		std::lock(lock_own, lock_rhs);

		copy_callbacks(rhs._callbacks);
	}

	template<typename R, typename... Args>
	Signal<R(Args...)>& Signal<R(Args...)>::operator= (const Signal& rhs)
	{
		std::unique_lock<std::mutex> lock_own(_mutex, std::defer_lock);
		std::unique_lock<std::mutex> lock_rhs(rhs._mutex, std::defer_lock);

		std::lock(lock_own, lock_rhs);

		_locked = rhs._locked;

		copy_callbacks(rhs._callbacks);

		return *this;
	}

	template<typename R, typename... Args>
	bool Signal<R(Args...)>::is_locked() const
	{
		return _locked;
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::set_lock(const bool lock)
	{
		_locked = lock;
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::Connect(Signal *sg)
	{
		std::lock_guard<std::mutex> locker_own(_mutex);
		std::lock_guard<std::mutex> locker_sg(sg->_mutex);

		if (_parent == sg)
		{
			return;
		}

		auto iter = std::find(_children.cbegin(), _children.cend(), sg);

		if (iter == _children.cend())
		{
			sg->_parent = this;

			_children.push_back(std::move(sg));
		}
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::DisConnect(Signal *sg)
	{
		std::lock_guard<std::mutex> locker(_mutex);

		_children.remove(sg);
	}

	template<typename R, typename... Args>
	Connection Signal<R(Args...)>::Connect(const callback_type& fn, Slot *owner)
	{
		return create_Connection(static_cast<callback_type>(fn), owner);
	}

	template<typename R, typename... Args>
	Connection Signal<R(Args...)>::Connect(callback_type&& fn, Slot *owner)
	{
		return create_Connection(std::move(fn), owner);
	}

	template<typename R, typename... Args>
	template<typename T, typename U>
	Connection Signal<R(Args...)>::Connect(T *p, const U& fn, Slot *owner)
	{
		auto mem_fn = std::move(construct_mem_fn(fn, p, make_int_sequence<sizeof...(Args)>{}));

		return create_Connection(std::move(mem_fn), owner);
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::DisConnect(const Connection& Connection)
	{
		destroy_Connection(Connection._data);
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::DisConnect(Slot *owner)
	{
		if (owner != nullptr)
		{
			destroy_Connection(owner->_data);
		}
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::DisConnect_all()
	{
		std::lock_guard<std::mutex> locker(_mutex);

		for (const auto& jnt : _callbacks)
		{
			if (jnt.owner != nullptr)
			{
				jnt.owner->_data = nullptr;
				jnt.owner->_cleaners.clear();
			}
		}
		_callbacks.clear();
		for (auto sig : _children)
		{
			if (sig->_parent == this) // should be an assert
			{
				sig->_parent = nullptr;
			}
		}
		_children.clear();
	}

	template<typename R, typename... Args>
	R Signal<R(Args...)>::operator() (Args... args)
	{
		std::lock_guard<std::mutex> locker(_mutex);

		if (!_locked)
		{
			for (Signal *sig : _children)
			{
				sig->operator()(std::forward<Args>(args)...);
			}

			for (auto iter = _callbacks.cbegin(); iter != _callbacks.cend(); ++iter)
			{
				const joint& jnt = *iter;

				if (!jnt.Connection->locked)
				{
					if (std::next(iter, 1) == _callbacks.cend())
					{
						return jnt.callback(std::forward<Args>(args)...);
					}

					jnt.callback(std::forward<Args>(args)...);
				}
			}
		}

		return R();
	}

	template<typename R, typename... Args>
	template<typename T>
	R Signal<R(Args...)>::operator() (Args... args, const T& agg)
	{
		std::vector<R> result;

		std::lock_guard<std::mutex> locker(_mutex);

		if (!_locked)
		{
			for (Signal *sig : _children)
			{
				sig->operator()(std::forward<Args>(args)...);
			}

			result.reserve(_callbacks.size());

			for (auto iter = _callbacks.cbegin(); iter != _callbacks.cend(); ++iter)
			{
				const joint& jnt = *iter;

				if (!jnt.Connection->locked)
				{
					result.push_back(std::move(jnt.callback(std::forward<Args>(args)...)));
				}
			}
		}

		return agg(std::move(result));
	}

	template<typename R, typename... Args>
	template<typename T, typename U, int... Ns>
	typename Signal<R(Args...)>::callback_type Signal<R(Args...)>::construct_mem_fn(const T& fn, U *p, int_sequence<Ns...>) const
	{
		return std::bind(fn, p, placeholder_lSignal<Ns>{}...);
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::copy_callbacks(const std::list<joint>& callbacks)
	{
		for (auto iter = callbacks.begin(); iter != callbacks.end(); ++iter)
		{
			const joint& jn = *iter;

			if (jn.owner == nullptr)
			{
				joint jnt;

				jnt.callback = jn.callback;
				jnt.Connection = jn.Connection;
				jnt.owner = nullptr;

				_callbacks.push_back(std::move(jnt));
			}
		}
	}

	template<typename R, typename... Args>
	std::shared_ptr<Connection_data> Signal<R(Args...)>::create_Connection(callback_type&& fn, Slot *owner)
	{
		std::shared_ptr<Connection_data> Connection = std::make_shared<Connection_data>();

		if (owner != nullptr)
		{
			auto deleter = [this](std::shared_ptr<Connection_data> Connection)
			{
				destroy_Connection(Connection);
			};

			Connection_cleaner cleaner;

			cleaner.deleter = deleter;
			cleaner.data = Connection;

			owner->_data = Connection;
			owner->_cleaners.emplace_back(cleaner);
		}

		joint jnt;

		jnt.callback = std::move(fn);
		jnt.Connection = Connection;
		jnt.owner = owner;

		std::lock_guard<std::mutex> locker(_mutex);

		_callbacks.push_back(std::move(jnt));

		return Connection;
	}

	template<typename R, typename... Args>
	void Signal<R(Args...)>::destroy_Connection(std::shared_ptr<Connection_data> Connection)
	{
		std::lock_guard<std::mutex> locker(_mutex);

		for (auto iter = _callbacks.begin(); iter != _callbacks.end(); ++iter)
		{
			const joint& jnt = *iter;

			if (jnt.Connection == Connection)
			{
				if (jnt.owner != nullptr)
				{
					jnt.owner->_data = nullptr;
					jnt.owner->_cleaners.clear();
				}

				_callbacks.erase(iter);

				break;
			}
		}
	}
}

#endif // LSIGNAL_H
