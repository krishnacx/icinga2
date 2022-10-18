/* Icinga 2 | (c) 2022 Icinga GmbH | GPLv2+ */

#include "base/string.hpp"
#include "config/applyrule.hpp"
#include "config/expression.hpp"
#include <utility>
#include <vector>

using namespace icinga;

const std::vector<ApplyRule::Ptr>& ApplyRule::GetTargetedHostRules(const Type::Ptr& sourceType, const Type::Ptr& targetType, const String& host)
{
	auto perSourceType (m_Rules.find(sourceType.get()));

	if (perSourceType != m_Rules.end()) {
		auto perTargetType (perSourceType->second.find(targetType.get()));

		if (perTargetType != perSourceType->second.end()) {
			auto perHost (perTargetType->second.Targeted.find(host));

			if (perHost != perTargetType->second.Targeted.end()) {
				return perHost->second.ForHost;
			}
		}
	}

	static const std::vector<ApplyRule::Ptr> noRules;
	return noRules;
}

const std::vector<ApplyRule::Ptr>& ApplyRule::GetTargetedServiceRules(const Type::Ptr& sourceType, const Type::Ptr& targetType, const String& host, const String& service)
{
	auto perSourceType (m_Rules.find(sourceType.get()));

	if (perSourceType != m_Rules.end()) {
		auto perTargetType (perSourceType->second.find(targetType.get()));

		if (perTargetType != perSourceType->second.end()) {
			auto perHost (perTargetType->second.Targeted.find(host));

			if (perHost != perTargetType->second.Targeted.end()) {
				auto perService (perHost->second.ForServices.find(service));

				if (perService != perHost->second.ForServices.end()) {
					return perService->second;
				}
			}
		}
	}

	static const std::vector<ApplyRule::Ptr> noRules;
	return noRules;
}

bool ApplyRule::AddTargetedRule(ApplyRule& rule, const String& sourceType, ApplyRule::PerTypes& rules)
{
	if (rule.m_TargetType == "Host" || (rule.m_TargetType == "" && sourceType == "Service")) {
		std::vector<const String *> hosts;

		if (GetTargetHosts(rule.m_Filter.get(), hosts)) {
			ApplyRule::Ptr sharedRule = new ApplyRule(std::move(rule));

			for (auto host : hosts) {
				rules.Targeted[*host].ForHost.emplace_back(sharedRule);
			}

			return true;
		}
	} else if (rule.m_TargetType == "Service") {
		std::vector<std::pair<const String *, const String *>> services;

		if (GetTargetServices(rule.m_Filter.get(), services)) {
			ApplyRule::Ptr sharedRule = new ApplyRule(std::move(rule));

			for (auto service : services) {
				rules.Targeted[*service.first].ForServices[*service.second].emplace_back(sharedRule);
			}

			return true;
		}
	}

	return false;
}

bool ApplyRule::GetTargetHosts(Expression* assignFilter, std::vector<const String *>& hosts)
{
	auto lor (dynamic_cast<LogicalOrExpression*>(assignFilter));

	if (lor) {
		return GetTargetHosts(lor->GetOperand1().get(), hosts)
			&& GetTargetHosts(lor->GetOperand2().get(), hosts);
	}

	auto name (GetComparedName(assignFilter, "host"));

	if (name) {
		hosts.emplace_back(name);
		return true;
	}

	return false;
}

bool ApplyRule::GetTargetServices(Expression* assignFilter, std::vector<std::pair<const String *, const String *>>& services)
{
	auto lor (dynamic_cast<LogicalOrExpression*>(assignFilter));

	if (lor) {
		return GetTargetServices(lor->GetOperand1().get(), services)
			&& GetTargetServices(lor->GetOperand2().get(), services);
	}

	auto service (GetTargetService(assignFilter));

	if (service.first) {
		services.emplace_back(service);
		return true;
	}

	return false;
}

std::pair<const String *, const String *> ApplyRule::GetTargetService(Expression* assignFilter)
{
	auto land (dynamic_cast<LogicalAndExpression*>(assignFilter));

	if (!land) {
		return {nullptr, nullptr};
	}

	auto op1 (land->GetOperand1().get());
	auto op2 (land->GetOperand2().get());

	{
		auto host (GetComparedName(op1, "host"));

		if (host) {
			auto service (GetComparedName(op2, "service"));

			if (!service) {
				return {nullptr, nullptr};
			}

			return {host, service};
		}
	}

	auto service (GetComparedName(op1, "service"));

	if (!service) {
		return {nullptr, nullptr};
	}

	auto host (GetComparedName(op2, "host"));

	if (!host) {
		return {nullptr, nullptr};
	}

	return {host, service};
}

const String * ApplyRule::GetComparedName(Expression* assignFilter, const char * lcType)
{
	auto eq (dynamic_cast<EqualExpression*>(assignFilter));

	if (!eq) {
		return nullptr;
	}

	auto op1 (eq->GetOperand1().get());
	auto op2 (eq->GetOperand2().get());

	if (IsNameIndexer(op1, lcType)) {
		return GetLiteralStringValue(op2);
	}

	if (IsNameIndexer(op2, lcType)) {
		return GetLiteralStringValue(op1);
	}

	return nullptr;
}

bool ApplyRule::IsNameIndexer(Expression* exp, const char * lcType)
{
	auto ixr (dynamic_cast<IndexerExpression*>(exp));

	if (!ixr) {
		return false;
	}

	auto var (dynamic_cast<VariableExpression*>(ixr->GetOperand1().get()));

	if (!var || var->GetVariable() != lcType) {
		return false;
	}

	auto val (GetLiteralStringValue(ixr->GetOperand2().get()));

	return val && *val == "name";
}

const String * ApplyRule::GetLiteralStringValue(Expression* exp)
{
	auto lit (dynamic_cast<LiteralExpression*>(exp));

	if (!lit) {
		return nullptr;
	}

	auto& val (lit->GetValue());

	if (!val.IsString()) {
		return nullptr;
	}

	return &val.Get<String>();
}