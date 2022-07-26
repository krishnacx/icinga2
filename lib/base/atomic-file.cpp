/* Icinga 2 | (c) 2022 Icinga GmbH | GPLv2+ */

#include "base/atomic-file.hpp"
#include "base/exception.hpp"
#include "base/utility.hpp"
#include <utility>

#ifdef _WIN32
#	include <windows.h>
#else /* _WIN32 */
#	include <errno.h>
#	include <unistd.h>
#endif /* _WIN32 */

using namespace icinga;

AtomicFile::AtomicFile(String path, int mode) : m_Path(std::move(path))
{
	m_TempFilename = m_Path + ".tmp.XXXXXX";
	int fd;

#ifdef _WIN32
	fd = Utility::MksTemp(&m_TempFilename[0]);
#else /* _WIN32 */
	fd = mkstemp(&m_TempFilename[0]);
#endif /* _WIN32 */

	if (fd < 0) {
		auto error (errno);

		BOOST_THROW_EXCEPTION(posix_error()
			<< boost::errinfo_api_function("mkstemp")
			<< boost::errinfo_errno(error)
			<< boost::errinfo_file_name(m_TempFilename));
	}

	exceptions(failbit | badbit);
	open(boost::iostreams::file_descriptor(fd, boost::iostreams::close_handle));

	if (chmod(m_TempFilename.CStr(), mode) < 0) {
		auto error (errno);

		BOOST_THROW_EXCEPTION(posix_error()
			<< boost::errinfo_api_function("chmod")
			<< boost::errinfo_errno(error)
			<< boost::errinfo_file_name(m_TempFilename));
	}
}

AtomicFile::~AtomicFile()
{
	if (!m_TempFilename.IsEmpty()) {
		try {
			Utility::Remove(m_TempFilename);
		} catch (...) {
			// A destructor must not throw
		}
	}
}

void AtomicFile::Commit()
{
	flush();

	auto h ((*this)->handle());

#ifdef _WIN32
	if (!FlushFileBuffers(h)) {
		auto err (GetLastError());

		BOOST_THROW_EXCEPTION(win32_error()
			<< boost::errinfo_api_function("FlushFileBuffers")
			<< errinfo_win32_error(err)
			<< boost::errinfo_file_name(m_TempFilename));
	}
#else /* _WIN32 */
	if (fsync(h)) {
		auto err (errno);

		BOOST_THROW_EXCEPTION(posix_error()
			<< boost::errinfo_api_function("fsync")
			<< boost::errinfo_errno(err)
			<< boost::errinfo_file_name(m_TempFilename));
	}
#endif /* _WIN32 */

	close();

	Utility::RenameFile(m_TempFilename, m_Path);
	m_TempFilename = "";
}
