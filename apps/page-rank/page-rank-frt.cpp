#include <stdlib.h>

#include <frt.h>
#include <tlp.h>

#include "page-rank.h"

void PageRank(Pid num_partitions, tlp::mmap<const Vid> num_vertices,
              tlp::mmap<const Eid> num_edges, tlp::mmap<VertexAttr> vertices,
              tlp::mmap<const Edge> edges, tlp::mmap<Update> updates) {
  auto instance =
      fpga::Invoke(getenv("BITSTREAM"), num_partitions,
                   fpga::WriteOnly(num_vertices.get(), num_vertices.size()),
                   fpga::WriteOnly(num_edges.get(), num_edges.size()),
                   fpga::ReadWrite(vertices.get(), vertices.size()),
                   fpga::WriteOnly(edges.get(), edges.size()),
                   fpga::ReadWrite(updates.get(), updates.size()));
  LOG(INFO) << "kernel time: " << instance.ComputeTimeSeconds() << "s";
}