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

#include <ns3/core-module.h>
#include <ns3/applications-module.h>
#include <ns3/drone-server.h>
#include <ns3/drone-client.h>
#include <ns3/drone-scenario-helper.h>

#define DSH DroneScenarioHelper::Get ()
#define CONFIG DSH->GetConfigurator ()

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Scenario");

int main (int argc, char **argv)
{
  DSH->Initialize (argc, argv);

  for (uint32_t i = 0; i < CONFIG->GetDronesN (); ++i)
    {
      Ptr<Application> clientApp = CreateObjectWithAttributes<DroneClient> (
        "Ipv4Address", Ipv4AddressValue (DSH->GetDroneIpv4Address (i, 1)),
        "Ipv4SubnetMask", Ipv4MaskValue ("255.0.0.0"),
        "Duration", DoubleValue (CONFIG->GetDuration ()),
        "DestinationIpv4Address", Ipv4AddressValue (DSH->GetRemoteIpv4Address (0, 1)));
      clientApp->SetStartTime (Seconds (CONFIG->GetDroneApplicationStartTime (i)));
      clientApp->SetStopTime (Seconds (CONFIG->GetDroneApplicationStopTime (i)));
      DSH->SetDroneApplication (i, clientApp);
    }

  // Install DroneServer application on the first remote only
  Ptr<Application> serverApp = CreateObjectWithAttributes<DroneServer> (
    "Ipv4Address", Ipv4AddressValue (DSH->GetRemoteIpv4Address (0, 1)),
    "Ipv4SubnetMask", Ipv4MaskValue ("255.0.0.0"));
  serverApp->SetStartTime (Seconds (CONFIG->GetRemoteApplicationStartTime (0)));
  serverApp->SetStopTime (Seconds (CONFIG->GetRemoteApplicationStopTime (0)));
  DSH->SetRemoteApplication (0, serverApp);

  // Enable traces of the first network only
  DSH->EnableTraces (0);

  DSH->Run ();

  return 0;
}
