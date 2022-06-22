/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#include "base/configuration.hpp"
#include "base/configuration-ti.cpp"
#include "base/exception.hpp"

using namespace icinga;

REGISTER_TYPE(Configuration);

String Configuration::ApiBindHost = []() {
#ifndef _WIN32
	// Automatically fall back to an IPv4 default if socket() tells us that IPv6 is not supported.
	int fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd < 0 && errno == EAFNOSUPPORT) {
		return "0.0.0.0";
	} else if (fd >= 0) {
		close(fd);
	}
#endif /* _WIN32 */

	return "::";
}();

String Configuration::ApiBindPort{"5665"};
bool Configuration::AttachDebugger{false};
String Configuration::CacheDir;
int Configuration::Concurrency{static_cast<int>(std::thread::hardware_concurrency())};
String Configuration::ConfigDir;
String Configuration::DataDir;
String Configuration::EventEngine;
String Configuration::IncludeConfDir;
String Configuration::InitRunDir;
String Configuration::LogDir;
String Configuration::ModAttrPath;
String Configuration::ObjectsPath;
String Configuration::PidPath;
String Configuration::PkgDataDir;
String Configuration::PrefixDir;
String Configuration::ProgramData;
int Configuration::RLimitFiles;
int Configuration::RLimitProcesses;
int Configuration::RLimitStack;
String Configuration::RunAsGroup;
String Configuration::RunAsUser;
String Configuration::SpoolDir;
String Configuration::StatePath;
double Configuration::TlsHandshakeTimeout{10};
String Configuration::VarsPath;
String Configuration::ZonesDir;

/* deprecated */
String Configuration::LocalStateDir;
String Configuration::RunDir;
String Configuration::SysconfDir;

/* internal */
bool Configuration::m_ReadOnly{false};

template<typename T>
void HandleUserWrite(const String& name, T *target, const T& value, bool readOnly)
{
	if (readOnly)
		BOOST_THROW_EXCEPTION(ScriptError("Configuration attribute '" + name + "' is read-only."));

	*target = value;
}

String Configuration::GetApiBindHost() const
{
	return Configuration::ApiBindHost;
}

void Configuration::SetApiBindHost(const String& val, bool, const Value&)
{
	HandleUserWrite("ApiBindHost", &Configuration::ApiBindHost, val, m_ReadOnly);
}

String Configuration::GetApiBindPort() const
{
	return Configuration::ApiBindPort;
}

void Configuration::SetApiBindPort(const String& val, bool, const Value&)
{
	HandleUserWrite("ApiBindPort", &Configuration::ApiBindPort, val, m_ReadOnly);
}

bool Configuration::GetAttachDebugger() const
{
	return Configuration::AttachDebugger;
}

void Configuration::SetAttachDebugger(bool val, bool, const Value&)
{
	HandleUserWrite("AttachDebugger", &Configuration::AttachDebugger, val, m_ReadOnly);
}

String Configuration::GetCacheDir() const
{
	return Configuration::CacheDir;
}

void Configuration::SetCacheDir(const String& val, bool, const Value&)
{
	HandleUserWrite("CacheDir", &Configuration::CacheDir, val, m_ReadOnly);
}

int Configuration::GetConcurrency() const
{
	return Configuration::Concurrency;
}

void Configuration::SetConcurrency(int val, bool, const Value&)
{
	HandleUserWrite("Concurrency", &Configuration::Concurrency, val, m_ReadOnly);
}

String Configuration::GetConfigDir() const
{
	return Configuration::ConfigDir;
}

void Configuration::SetConfigDir(const String& val, bool, const Value&)
{
	HandleUserWrite("ConfigDir", &Configuration::ConfigDir, val, m_ReadOnly);
}

String Configuration::GetDataDir() const
{
	return Configuration::DataDir;
}

void Configuration::SetDataDir(const String& val, bool, const Value&)
{
	HandleUserWrite("DataDir", &Configuration::DataDir, val, m_ReadOnly);
}

String Configuration::GetEventEngine() const
{
	return Configuration::EventEngine;
}

void Configuration::SetEventEngine(const String& val, bool, const Value&)
{
	HandleUserWrite("EventEngine", &Configuration::EventEngine, val, m_ReadOnly);
}

String Configuration::GetIncludeConfDir() const
{
	return Configuration::IncludeConfDir;
}

void Configuration::SetIncludeConfDir(const String& val, bool, const Value&)
{
	HandleUserWrite("IncludeConfDir", &Configuration::IncludeConfDir, val, m_ReadOnly);
}

String Configuration::GetInitRunDir() const
{
	return Configuration::InitRunDir;
}

void Configuration::SetInitRunDir(const String& val, bool, const Value&)
{
	HandleUserWrite("InitRunDir", &Configuration::InitRunDir, val, m_ReadOnly);
}

String Configuration::GetLogDir() const
{
	return Configuration::LogDir;
}

void Configuration::SetLogDir(const String& val, bool, const Value&)
{
	HandleUserWrite("LogDir", &Configuration::LogDir, val, m_ReadOnly);
}

String Configuration::GetModAttrPath() const
{
	return Configuration::ModAttrPath;
}

void Configuration::SetModAttrPath(const String& val, bool, const Value&)
{
	HandleUserWrite("ModAttrPath", &Configuration::ModAttrPath, val, m_ReadOnly);
}

String Configuration::GetObjectsPath() const
{
	return Configuration::ObjectsPath;
}

void Configuration::SetObjectsPath(const String& val, bool, const Value&)
{
	HandleUserWrite("ObjectsPath", &Configuration::ObjectsPath, val, m_ReadOnly);
}

String Configuration::GetPidPath() const
{
	return Configuration::PidPath;
}

void Configuration::SetPidPath(const String& val, bool, const Value&)
{
	HandleUserWrite("PidPath", &Configuration::PidPath, val, m_ReadOnly);
}

String Configuration::GetPkgDataDir() const
{
	return Configuration::PkgDataDir;
}

void Configuration::SetPkgDataDir(const String& val, bool, const Value&)
{
	HandleUserWrite("PkgDataDir", &Configuration::PkgDataDir, val, m_ReadOnly);
}

String Configuration::GetPrefixDir() const
{
	return Configuration::PrefixDir;
}

void Configuration::SetPrefixDir(const String& val, bool, const Value&)
{
	HandleUserWrite("PrefixDir", &Configuration::PrefixDir, val, m_ReadOnly);
}

String Configuration::GetProgramData() const
{
	return Configuration::ProgramData;
}

void Configuration::SetProgramData(const String& val, bool, const Value&)
{
	HandleUserWrite("ProgramData", &Configuration::ProgramData, val, m_ReadOnly);
}

int Configuration::GetRLimitFiles() const
{
	return Configuration::RLimitFiles;
}

void Configuration::SetRLimitFiles(int val, bool, const Value&)
{
	HandleUserWrite("RLimitFiles", &Configuration::RLimitFiles, val, m_ReadOnly);
}

int Configuration::GetRLimitProcesses() const
{
	return RLimitProcesses;
}

void Configuration::SetRLimitProcesses(int val, bool, const Value&)
{
	HandleUserWrite("RLimitProcesses", &Configuration::RLimitProcesses, val, m_ReadOnly);
}

int Configuration::GetRLimitStack() const
{
	return Configuration::RLimitStack;
}

void Configuration::SetRLimitStack(int val, bool, const Value&)
{
	HandleUserWrite("RLimitStack", &Configuration::RLimitStack, val, m_ReadOnly);
}

String Configuration::GetRunAsGroup() const
{
	return Configuration::RunAsGroup;
}

void Configuration::SetRunAsGroup(const String& val, bool, const Value&)
{
	HandleUserWrite("RunAsGroup", &Configuration::RunAsGroup, val, m_ReadOnly);
}

String Configuration::GetRunAsUser() const
{
	return Configuration::RunAsUser;
}

void Configuration::SetRunAsUser(const String& val, bool, const Value&)
{
	HandleUserWrite("RunAsUser", &Configuration::RunAsUser, val, m_ReadOnly);
}

String Configuration::GetSpoolDir() const
{
	return Configuration::SpoolDir;
}

void Configuration::SetSpoolDir(const String& val, bool, const Value&)
{
	HandleUserWrite("SpoolDir", &Configuration::SpoolDir, val, m_ReadOnly);
}

String Configuration::GetStatePath() const
{
	return Configuration::StatePath;
}

void Configuration::SetStatePath(const String& val, bool, const Value&)
{
	HandleUserWrite("StatePath", &Configuration::StatePath, val, m_ReadOnly);
}

double Configuration::GetTlsHandshakeTimeout() const
{
	return Configuration::TlsHandshakeTimeout;
}

void Configuration::SetTlsHandshakeTimeout(double val, bool, const Value&)
{
	HandleUserWrite("TlsHandshakeTimeout", &Configuration::TlsHandshakeTimeout, val, m_ReadOnly);
}

String Configuration::GetVarsPath() const
{
	return Configuration::VarsPath;
}

void Configuration::SetVarsPath(const String& val, bool, const Value&)
{
	HandleUserWrite("VarsPath", &Configuration::VarsPath, val, m_ReadOnly);
}

String Configuration::GetZonesDir() const
{
	return Configuration::ZonesDir;
}

void Configuration::SetZonesDir(const String& val, bool, const Value&)
{
	HandleUserWrite("ZonesDir", &Configuration::ZonesDir, val, m_ReadOnly);
}

String Configuration::GetLocalStateDir() const
{
	return Configuration::LocalStateDir;
}

void Configuration::SetLocalStateDir(const String& val, bool, const Value&)
{
	HandleUserWrite("LocalStateDir", &Configuration::LocalStateDir, val, m_ReadOnly);
}

String Configuration::GetSysconfDir() const
{
	return Configuration::SysconfDir;
}

void Configuration::SetSysconfDir(const String& val, bool, const Value&)
{
	HandleUserWrite("SysconfDir", &Configuration::SysconfDir, val, m_ReadOnly);
}

String Configuration::GetRunDir() const
{
	return Configuration::RunDir;
}

void Configuration::SetRunDir(const String& val, bool, const Value&)
{
	HandleUserWrite("RunDir", &Configuration::RunDir, val, m_ReadOnly);
}

bool Configuration::GetReadOnly()
{
	return m_ReadOnly;
}

void Configuration::SetReadOnly(bool readOnly)
{
	m_ReadOnly = readOnly;
}
