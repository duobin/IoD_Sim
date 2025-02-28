set(source_files
  model/drone-control-layer.cc
  model/ipv4-layer.cc
  model/protocol-layer.cc
  model/report-container.cc
  model/report-data-stats.cc
  model/report-drone.cc
  model/report-entity.cc
  model/report-location.cc
  model/report-protocol-stack.cc
  model/report-simulation.cc
  model/report-transfer.cc
  model/report-zsp.cc
  model/report.cc
  model/simulation-duration.cc
  model/transfer-direction.cc
  model/wifi-inspector.cc
  model/wifi-mac-layer.cc
  model/wifi-phy-layer.cc
  helper/report-helper.cc
  model/report-peripheral.cc
  model/report-world.cc
  model/report-remote.cc
  model/lte-ue-phy-layer.cc
)

set(header_files
  model/drone-control-layer.h
  model/ipv4-layer.h
  model/protocol-layer.h
  model/report-container.h
  model/report-data-stats.h
  model/report-default-iterator.h
  model/report-drone.h
  model/report-entity.h
  model/report-location.h
  model/report-protocol-stack.h
  model/report-simulation.h
  model/report-transfer.h
  model/report-zsp.h
  model/report.h
  model/simulation-duration.h
  model/transfer-direction.h
  model/wifi-inspector.h
  model/wifi-mac-layer.h
  model/wifi-phy-layer.h
  helper/report-helper.h
  model/report-peripheral.h
  model/report-world.h
  model/report-remote.h
  model/lte-ue-phy-layer.h
)

include(FindPkgConfig)

if (${PKG_CONFIG_FOUND})
  pkg_check_modules(THIRD_PARTY libxml-2.0 REQUIRED)

  build_lib(
    LIBNAME report
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    LIBRARIES_TO_LINK ${libcore}
  )
endif()
