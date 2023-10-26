#include "config/DirectiveTypeTraits.hpp"

namespace config {

DirectiveTypeTraitsBase::DirectiveTypeTraitsBase(std::string const& name)
    : m_name(name), m_valid(false) {}

DirectiveTypeTraitsBase::~DirectiveTypeTraitsBase(void) {}

bool DirectiveTypeTraitsBase::isValid(void) const { return (m_valid); }

std::string DirectiveTypeTraitsBase::getName(void) const { return (m_name); }

bool DirectiveTypeTraitsBase::isBlockDirective(void) const { return (false); }

bool DirectiveTypeTraitsBase::isRouteDirective(void) const { return (false); }

bool DirectiveTypeTraitsBase::isGlobalDirective(void) const { return (false); }

void DirectiveTypeTraitsBase::set(std::vector<ServerOpts>&) const {}

void DirectiveTypeTraitsBase::set(ServerOpts&) const {}

void DirectiveTypeTraitsBase::set(LocationOpts&) const {}

} // namespace config