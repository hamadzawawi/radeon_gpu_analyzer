//=================================================================
// Copyright 2020-2021 Advanced Micro Devices, Inc. All rights reserved.
//=================================================================
#ifndef RGA_RADEONGPUANALYZERCLI_SRC_KC_XML_WRITER_H_
#define RGA_RADEONGPUANALYZERCLI_SRC_KC_XML_WRITER_H_

// XML.
#include "tinyxml2/Include/tinyxml2.h"

// Local.
#include "radeon_gpu_analyzer_cli/kc_utils.h"

class KcXmlWriter
{
public:
    // Generates the header for RGA CLI version info file and writes it to the file specified by "fileName" argument.
    static bool AddVersionInfoHeader(const std::string& filename);

    // Writes the given list of supported target GPUs to the version info file specified by the "fileName" argument.
    static bool AddVersionInfoGPUList(RgaMode mode, const std::set<std::string>& targets, const std::string& filename);

    // Writes the system level version info to the version info file specified by the "fileName" argument.
    static bool AddVersionInfoSystemData(const std::vector<BeVkPhysAdapterInfo>& info, const std::string& filename);

    // Generates the RGA CLI metadata file for OpenCL-Offline mode that contains the list of output
    // files generated by RGA CLI.
    static bool GenerateClSessionMetadataFile(const std::string& filename, const RgFileEntryData& kernel_data,
                                              const RgClOutputMetadata& out_files);

    // Generates the RGA CLI metadata file for Vulkan mode that contains the list of output
    // files generated by RGA CLI.
    static bool GenerateVulkanSessionMetadataFile(const std::string& filename,
                                                  const std::map<std::string, RgVkOutputMetadata>& output_metadata);
};

#endif // RGA_RADEONGPUANALYZERCLI_SRC_KC_XML_WRITER_H_