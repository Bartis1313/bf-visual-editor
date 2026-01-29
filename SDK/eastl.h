#pragma once

#include <cstdint>

namespace eastl
{
	template <typename T>
	class vector
	{
	public:
		size_t size() const
		{
			return (reinterpret_cast<uintptr_t>(m_End) - reinterpret_cast<uintptr_t>(m_FirstElement)) / sizeof(T);
		}

		T& at(size_t index)
		{
			return m_FirstElement[index];
		}

		const T& at(size_t index) const
		{
			return m_FirstElement[index];
		}

		T& operator[](size_t index)
		{
			return m_FirstElement[index];
		}

		const T& operator[](size_t index) const
		{
			return m_FirstElement[index];
		}

		T* begin() { return m_FirstElement; }
		const T* begin() const { return m_FirstElement; }

		T* end() { return m_End; }
		const T* end() const { return m_End; }

		bool empty() const { return size() == 0; }

		size_t capacity() const
		{
			return (reinterpret_cast<uintptr_t>(m_ArrayBound) - reinterpret_cast<uintptr_t>(m_FirstElement)) / sizeof(T);
		}

	private:
		T* m_FirstElement = nullptr;
		T* m_End = nullptr;
		T* m_ArrayBound = nullptr;
		void* vftable = nullptr;
	};

	struct string
	{
		char const* begin;
		char const* end;
		char const* capacity;
		void* allocator_arena;

		auto c_str()
		{
			return begin;
		}
	};
}