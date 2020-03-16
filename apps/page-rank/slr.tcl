# partitioning constraints generated by tlpp
# modify only if you know what you are doing
puts "applying partitioning constraints generated by tlpp"
add_cells_to_pblock [get_pblocks pblock_dynamic_SLR0] [get_cells -hierarchical -regex {
  (.*/)?EdgeMem_0
  (.*/)?edges_0__m_axi
  (.*/)?ProcElem_0
  (.*/)?UpdateMem_0
  (.*/)?updates_0__m_axi
  (.*/)?UpdateHandler_0
  (.*/)?PageRank_control_s_axi_U
  (.*/)?edge_req_0
  (.*/)?edge_resp_0
  (.*/)?num_updates_0/inst\\[0]\\.unit
  (.*/)?num_updates_0/inst\\[1]\\.unit
  (.*/)?task_req_0/inst\\[2]\\.unit
  (.*/)?task_req_0/inst\\[3]\\.unit
  (.*/)?task_resp_0/inst\\[0]\\.unit
  (.*/)?task_resp_0/inst\\[1]\\.unit
  (.*/)?update_config_0/inst\\[2]\\.unit
  (.*/)?update_config_0/inst\\[3]\\.unit
  (.*/)?update_mm2pe_0
  (.*/)?update_pe2mm_0
  (.*/)?update_phase_0/inst\\[2]\\.unit
  (.*/)?update_phase_0/inst\\[3]\\.unit
  (.*/)?update_read_addr_0
  (.*/)?update_read_data_0
  (.*/)?update_req_0
  (.*/)?update_write_addr_0
  (.*/)?update_write_data_0
  (.*/)?vertex_mm2pe_0/inst\\[3]\\.unit
  (.*/)?vertex_pe2mm_0/inst\\[0]\\.unit
  (.*/)?vertex_req_0/inst\\[0]\\.unit
}]
add_cells_to_pblock [get_pblocks pblock_dynamic_SLR1] [get_cells -hierarchical -regex {
  (.*/)?EdgeMem_1
  (.*/)?edges_1__m_axi
  (.*/)?ProcElem_1
  (.*/)?UpdateMem_1
  (.*/)?updates_1__m_axi
  (.*/)?UpdateHandler_1
  (.*/)?Control_0
  (.*/)?edge_req_1
  (.*/)?edge_resp_1
  (.*/)?num_updates_0/inst\\[2]\\.unit
  (.*/)?num_updates_0/inst\\[3]\\.unit
  (.*/)?num_updates_1
  (.*/)?num_updates_2/inst\\[2]\\.unit
  (.*/)?num_updates_2/inst\\[3]\\.unit
  (.*/)?num_updates_3/inst\\[3]\\.unit
  (.*/)?task_req_0/inst\\[0]\\.unit
  (.*/)?task_req_0/inst\\[1]\\.unit
  (.*/)?task_req_1
  (.*/)?task_req_2/inst\\[0]\\.unit
  (.*/)?task_req_2/inst\\[1]\\.unit
  (.*/)?task_req_3/inst\\[0]\\.unit
  (.*/)?task_resp_0/inst\\[2]\\.unit
  (.*/)?task_resp_0/inst\\[3]\\.unit
  (.*/)?task_resp_1
  (.*/)?task_resp_2/inst\\[2]\\.unit
  (.*/)?task_resp_2/inst\\[3]\\.unit
  (.*/)?task_resp_3/inst\\[3]\\.unit
  (.*/)?update_config_0/inst\\[0]\\.unit
  (.*/)?update_config_0/inst\\[1]\\.unit
  (.*/)?update_config_1
  (.*/)?update_config_2/inst\\[0]\\.unit
  (.*/)?update_config_2/inst\\[1]\\.unit
  (.*/)?update_config_3/inst\\[0]\\.unit
  (.*/)?update_mm2pe_1
  (.*/)?update_pe2mm_1
  (.*/)?update_phase_0/inst\\[0]\\.unit
  (.*/)?update_phase_0/inst\\[1]\\.unit
  (.*/)?update_phase_1
  (.*/)?update_phase_2/inst\\[0]\\.unit
  (.*/)?update_phase_2/inst\\[1]\\.unit
  (.*/)?update_phase_3/inst\\[0]\\.unit
  (.*/)?update_read_addr_1
  (.*/)?update_read_data_1
  (.*/)?update_req_1
  (.*/)?update_write_addr_1
  (.*/)?update_write_data_1
  (.*/)?vertex_mm2pe_0/inst\\[1]\\.unit
  (.*/)?vertex_mm2pe_0/inst\\[2]\\.unit
  (.*/)?vertex_mm2pe_1/inst\\[2]\\.unit
  (.*/)?vertex_mm2pe_1/inst\\[3]\\.unit
  (.*/)?vertex_pe2mm_0/inst\\[1]\\.unit
  (.*/)?vertex_pe2mm_0/inst\\[2]\\.unit
  (.*/)?vertex_pe2mm_1/inst\\[0]\\.unit
  (.*/)?vertex_pe2mm_1/inst\\[1]\\.unit
  (.*/)?vertex_req_0/inst\\[1]\\.unit
  (.*/)?vertex_req_0/inst\\[2]\\.unit
  (.*/)?vertex_req_1/inst\\[0]\\.unit
  (.*/)?vertex_req_1/inst\\[1]\\.unit
}]
add_cells_to_pblock [get_pblocks pblock_dynamic_SLR2] [get_cells -hierarchical -regex {
  (.*/)?EdgeMem_2
  (.*/)?edges_2__m_axi
  (.*/)?ProcElem_2
  (.*/)?UpdateMem_2
  (.*/)?updates_2__m_axi
  (.*/)?UpdateHandler_2
  (.*/)?VertexMem_0
  (.*/)?vertices__m_axi
  (.*/)?edge_req_2
  (.*/)?edge_resp_2
  (.*/)?num_updates_2/inst\\[0]\\.unit
  (.*/)?num_updates_2/inst\\[1]\\.unit
  (.*/)?num_updates_3/inst\\[1]\\.unit
  (.*/)?num_updates_3/inst\\[2]\\.unit
  (.*/)?task_req_2/inst\\[2]\\.unit
  (.*/)?task_req_2/inst\\[3]\\.unit
  (.*/)?task_req_3/inst\\[1]\\.unit
  (.*/)?task_req_3/inst\\[2]\\.unit
  (.*/)?task_resp_2/inst\\[0]\\.unit
  (.*/)?task_resp_2/inst\\[1]\\.unit
  (.*/)?task_resp_3/inst\\[1]\\.unit
  (.*/)?task_resp_3/inst\\[2]\\.unit
  (.*/)?update_config_2/inst\\[2]\\.unit
  (.*/)?update_config_2/inst\\[3]\\.unit
  (.*/)?update_config_3/inst\\[1]\\.unit
  (.*/)?update_config_3/inst\\[2]\\.unit
  (.*/)?update_mm2pe_2
  (.*/)?update_pe2mm_2
  (.*/)?update_phase_2/inst\\[2]\\.unit
  (.*/)?update_phase_2/inst\\[3]\\.unit
  (.*/)?update_phase_3/inst\\[1]\\.unit
  (.*/)?update_phase_3/inst\\[2]\\.unit
  (.*/)?update_read_addr_2
  (.*/)?update_read_data_2
  (.*/)?update_req_2
  (.*/)?update_write_addr_2
  (.*/)?update_write_data_2
  (.*/)?vertex_mm2pe_0/inst\\[0]\\.unit
  (.*/)?vertex_mm2pe_1/inst\\[0]\\.unit
  (.*/)?vertex_mm2pe_1/inst\\[1]\\.unit
  (.*/)?vertex_mm2pe_2
  (.*/)?vertex_mm2pe_3/inst\\[0]\\.unit
  (.*/)?vertex_mm2pe_3/inst\\[1]\\.unit
  (.*/)?vertex_pe2mm_0/inst\\[3]\\.unit
  (.*/)?vertex_pe2mm_1/inst\\[2]\\.unit
  (.*/)?vertex_pe2mm_1/inst\\[3]\\.unit
  (.*/)?vertex_pe2mm_2
  (.*/)?vertex_pe2mm_3/inst\\[2]\\.unit
  (.*/)?vertex_pe2mm_3/inst\\[3]\\.unit
  (.*/)?vertex_req_0/inst\\[3]\\.unit
  (.*/)?vertex_req_1/inst\\[2]\\.unit
  (.*/)?vertex_req_1/inst\\[3]\\.unit
  (.*/)?vertex_req_2
  (.*/)?vertex_req_3/inst\\[2]\\.unit
  (.*/)?vertex_req_3/inst\\[3]\\.unit
}]
add_cells_to_pblock [get_pblocks pblock_dynamic_SLR3] [get_cells -hierarchical -regex {
  (.*/)?EdgeMem_3
  (.*/)?edges_3__m_axi
  (.*/)?ProcElem_3
  (.*/)?UpdateMem_3
  (.*/)?updates_3__m_axi
  (.*/)?UpdateHandler_3
  (.*/)?edge_req_3
  (.*/)?edge_resp_3
  (.*/)?num_updates_3/inst\\[0]\\.unit
  (.*/)?task_req_3/inst\\[3]\\.unit
  (.*/)?task_resp_3/inst\\[0]\\.unit
  (.*/)?update_config_3/inst\\[3]\\.unit
  (.*/)?update_mm2pe_3
  (.*/)?update_pe2mm_3
  (.*/)?update_phase_3/inst\\[3]\\.unit
  (.*/)?update_read_addr_3
  (.*/)?update_read_data_3
  (.*/)?update_req_3
  (.*/)?update_write_addr_3
  (.*/)?update_write_data_3
  (.*/)?vertex_mm2pe_3/inst\\[2]\\.unit
  (.*/)?vertex_mm2pe_3/inst\\[3]\\.unit
  (.*/)?vertex_pe2mm_3/inst\\[0]\\.unit
  (.*/)?vertex_pe2mm_3/inst\\[1]\\.unit
  (.*/)?vertex_req_3/inst\\[0]\\.unit
  (.*/)?vertex_req_3/inst\\[1]\\.unit
}]
