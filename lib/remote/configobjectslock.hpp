/* Icinga 2 | (c) 2022 Icinga GmbH | GPLv2+ */

#ifndef CONFIGOBJECTSLOCK_H
#define CONFIGOBJECTSLOCK_H

#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>

namespace icinga
{

/**
 * Waits until all ConfigObjects*Lock-s have vanished.
 * For its lifetime disallows ConfigObjectsSharedLock-s.
 * Keep an instance alive while no other one shall write config objects!
 *
 * @ingroup remote
 */
class ConfigObjectsExclusiveLock
{
public:
	ConfigObjectsExclusiveLock();

private:
	boost::interprocess::scoped_lock<boost::interprocess::interprocess_sharable_mutex> m_Lock;
};

/**
 * Keep an instance alive while writing config objects and being not the only one!
 * Check OwnsLock() for whether allowed to write!
 *
 * @ingroup remote
 */
class ConfigObjectsSharedLock
{
public:
	ConfigObjectsSharedLock();

	inline bool OwnsLock() const
	{
		return m_Lock.owns();
	}

private:
	boost::interprocess::sharable_lock<boost::interprocess::interprocess_sharable_mutex> m_Lock;
};

}

#endif /* CONFIGOBJECTSLOCK_H */
