/* Icinga 2 | (c) 2022 Icinga GmbH | GPLv2+ */

#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include <boost/interprocess/anonymous_shared_memory.hpp>
#include <utility>

namespace icinga
{

/**
 * Type-safe memory shared across fork(2).
 *
 * @ingroup base
 */
template<class T>
class SharedMemory
{
public:
	template<class... Args>
	SharedMemory(Args&&... args) : m_Memory(boost::interprocess::anonymous_shared_memory(sizeof(T)))
	{
		new(GetAddress()) T(std::forward<Args>(args)...);
	}

	SharedMemory(const SharedMemory&) = delete;
	SharedMemory(SharedMemory&&) = delete;
	SharedMemory& operator=(const SharedMemory&) = delete;
	SharedMemory& operator=(SharedMemory&&) = delete;

	inline T& Get() const
	{
		return *GetAddress();
	}

private:
	inline T* GetAddress() const
	{
		return (T*)m_Memory.get_address();
	}

	boost::interprocess::mapped_region m_Memory;
};

}

#endif /* SHARED_MEMORY_HPP */
