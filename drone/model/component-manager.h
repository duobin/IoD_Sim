/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018-2021 The IoD_Sim Authors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <ns3/singleton.h>


/// Use this as the only interface for ComponentManager to register a component at the end of a method before returning.
#define NS_OBJECT_REGISTER_COMPONENT() ComponentManager::Get()->RegisterComponent((uintptr_t)(void*)this, __FUNCTION__)
/// Use this as the only interface for ComponentManager to require a component at the beginning of a method.
#define NS_OBJECT_REQUIRE_COMPONENT(param) ComponentManager::Get()->RequireComponent((uintptr_t)(void*)this, __FUNCTION__, param)

namespace ns3
{

/**
 * \brief Allows methods of an object to require other methods to be called before
 *        by requiring the needed components and then registering itself as such for
 *        other methods. Each object has its own components and the ComponentManager
 *        automatically keeps records of them for each object indipendently.
 */
class ComponentManager : public Singleton<ComponentManager>
{
public:
  /**
   * \brief Adds the caller function to the set of components of the
   *        proprietary object.
   *        Use with `NS_OBJECT_REGISTER_COMPONENT()`
   * \param object The pointer to the caller object. The component will be added to this object's set.
   *               The macro uses `(uintptr_t)(void*)this` for that.
   * \param comp The component name to register.
   *             The macro uses builtin `__FUNCTION__` for that
   */
  void RegisterComponent(uintptr_t object, std::string comp);

  /**
   * \brief Asks for a method of the same object to be called before the caller method.
   *        If it's not been called stops the program.
   *        Use with `NS_OBJECT_REGISTER_COMPONENT("RequiredMethodName")`
   * \param object The pointer to the caller object. The component will be searched inside this object's set.
   *               The macro uses `(uintptr_t)(void*)this` for that.
   * \param caller The name of the caller function that is requiring the component.
   *               The macro uses builtin `__FUNCTION__` for that.
   * \param comp The component name to search.
   *             This should be explicitly passed even inside the macro.
   */
  void RequireComponent(uintptr_t object, std::string caller, std::string comp);

private:
  std::unordered_map<uintptr_t, std::unordered_set<std::string>> m_components;
};

} // namespace ns3

#endif // COMPONENT_MANAGER_H