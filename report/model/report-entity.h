/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019 The IoD_Sim Authors.
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
 *
 * Authors: Giovanni Grieco <giovanni.grieco@poliba.it>
 */
#ifndef REPORT_ENTITY_H
#define REPORT_ENTITY_H
#include <vector>

#include <ns3/ipv4.h>
#include <ns3/mobility-model.h>
#include <ns3/node.h>
#include <ns3/object.h>

#include <libxml/xmlwriter.h>

#include "report-data-stats.h"
#include "report-location.h"
#include "report-protocol-stack.h"
#include "report-transfer.h"

namespace ns3 {

/**
 * Generic object to address common report functionalities between IoD_Sim
 * entities, like Drone and ZSP.
 * Reporting in IoD_Sim consists in building and using the following monitors:
 *  - trajectory
 *  - network stats
 *  - traffic (Rx and Tx)
 *  - and eventual cumulative statistics that can be derived
 */
class ReportEntity : public Object
{
public:
  /**
   * Register the type using ns-3 TypeId System.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  /**
   * Default constructor
   */
  ReportEntity ();
  /**
   * Default destructor
   */
  ~ReportEntity ();

  /**
   * Write Entity report data to a XML file with a given handler
   *
   * \param handle the XML handler to write data on
   */
  void Write (xmlTextWriterPtr handle);
protected:
  /**
   * Write internal interface
   *
   * \param handle the XML handler to write data on
   */
  virtual void DoWrite (xmlTextWriterPtr handle);

  /**
   * Initialize probe to get trajectory data
   */
  virtual void DoInitializeTrajectoryMonitor ();

  /**
   * Explore a given entity object and build an abstraction of
   * its network stacks
   */
  virtual void DoInitializeNetworkStacks ();

  /**
   * Callback to monitor entity trajectory
   *
   * \params mobility the mobility model of the entity to inspect
   */
  virtual void DoMonitorTrajectory (const Ptr<const MobilityModel> mobility);

  /**
   * Callback to monitor incoming IPv4 packets
   *
   * \param packet    the incoming IPv4 payload
   * \param ipv4      the IPv4 header
   * \param interface drone interface ID
   */
  void DoMonitorRxTraffic (Ptr<const Packet> packet,
                           Ptr<Ipv4> ipv4,
                           uint32_t interface);

  /**
   * Callback to monitor outgoing IPv4 packets
   *
   * \param packet    the incoming IPv4 payload
   * \param ipv4      the IPv4 header
   * \param interface drone interface ID
   */
  void DoMonitorTxTraffic (Ptr<const Packet> packet,
                           Ptr<Ipv4> ipv4,
                           uint32_t interface);

  /**
   * Helper to check if NetDevice is a WifiNetDevice
   *
   * \param device the network device to inspect
   */
  static bool IsWifiNetDevice (Ptr<NetDevice> device);

  /**
   * Helper to get interface registered IP Address
   *
   * \param drone the drone to inspect
   * \return a tuple with the IPv4 address and its Network Mask
   */
  const std::tuple<const std::string, const std::string> GetIpv4Address ();

  /// cumulative Stats in Rx
  std::vector<Ptr<ReportDataStats>> m_cumulativeDataRx;
  /// cumulative Stats in Tx
  std::vector<Ptr<ReportDataStats>> m_cumulativeDataTx;
  /// monitored traffix Rx
  std::vector<Ptr<ReportTransfer>> m_dataRx;
  /// monitored traffic Tx
  std::vector<Ptr<ReportTransfer>> m_dataTx;

  uint32_t m_reference; /// Entity UID of reference
private:
  /**
   * Object internal initialization
   */
  void DoInitialize ();

  /**
   * Initialize Data Stats accumulators
   */
  void DoInitializeDataStats ();

  /**
   * Helper to initialize traffic monitor for both Rx and Tx directions
   */
  inline void DoInitializeTrafficMonitors ();

  /**
   * Initialize traffic monitor for a given direction relative to the
   * observed node
   *
   * \param direction the direction of the traffic flow
   */
  void DoInitializeTrafficMonitor (TransferDirection direction);
};

} // namespace ns3

#endif /* REPORT_ENTITY_H */
