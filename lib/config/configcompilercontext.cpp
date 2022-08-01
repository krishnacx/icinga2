/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#include "config/configcompilercontext.hpp"
#include "base/singleton.hpp"
#include "base/json.hpp"
#include "base/netstring.hpp"
#include "base/exception.hpp"
#include "base/application.hpp"
#include "base/utility.hpp"

using namespace icinga;

ConfigCompilerContext *ConfigCompilerContext::GetInstance()
{
	return Singleton<ConfigCompilerContext>::GetInstance();
}

void ConfigCompilerContext::OpenObjectsFile(const String& filename)
{
	try {
		m_ObjectsFP = new AtomicFile(filename, 0600);
	} catch (const std::exception& ex) {
		Log(LogCritical, "cli", "Could not create temporary objects file: " + DiagnosticInformation(ex, false));
		Application::Exit(1);
	}
}

void ConfigCompilerContext::WriteObject(const Dictionary::Ptr& object)
{
	if (!m_ObjectsFP)
		return;

	String json = JsonEncode(object);

	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		NetString::WriteStringToStream(*m_ObjectsFP, json);
	}
}

void ConfigCompilerContext::CancelObjectsFile()
{
	delete m_ObjectsFP;
	m_ObjectsFP = nullptr;
}

void ConfigCompilerContext::FinishObjectsFile()
{
	m_ObjectsFP->Commit();

	delete m_ObjectsFP;
	m_ObjectsFP = nullptr;
}

