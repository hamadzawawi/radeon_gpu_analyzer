//=================================================================
// Copyright 2020 Advanced Micro Devices, Inc. All rights reserved.
//=================================================================
#ifndef RGA_RADEONGPUANALYZERCLI_SRC_KC_CLI_COMMANDER_LIGHTNING_H_
#define RGA_RADEONGPUANALYZERCLI_SRC_KC_CLI_COMMANDER_LIGHTNING_H_

// C++.
#include <string>
#include <set>
#include <memory>
#include <unordered_map>

// Local.
#include "radeon_gpu_analyzer_cli/kc_cli_commander.h"

// Backend.
#include "source/radeon_gpu_analyzer_backend/be_program_builder_lightning.h"

using namespace std;

// Commander interface for compiling with the Lightning Compiler (LC).
class KcCLICommanderLightning : public KcCliCommander
{
public:
    KcCLICommanderLightning() = default;
    ~KcCLICommanderLightning() = default;

    // Print the driver version.
    void Version(Config& config, LoggingCallbackFunction callback) override;

    // Print list of supported devices.
    virtual bool PrintAsicList(const Config&) override;

    // Parse the source file and extract list of entry points (for example, kernels for OpenCL).
    // Dump the extracted entry points to stdout.
    virtual bool ListEntries(const Config& config, LoggingCallbackFunction callback) override;

    // Actual implementation of "ListEntries" virtual function.
    // This function is also used by legacy cl mode.
    static bool ListEntriesOpenclOffline(const Config& config, LoggingCallbackFunction callback);

    // Perform compilation steps.
    void RunCompileCommands(const Config& config, LoggingCallbackFunction callback) override;

    // Perform post-compile actions.
    bool RunPostCompileSteps(const Config& config) override;

    // Initialize the Commander object.
    beStatus  Init(const Config& config, LoggingCallbackFunction log_callback);

    // Generates OpenCL-Offline "version info" data and writes it to the file specified by "fileName".
    // The data will be appended to the existing content of the file.
    static bool GenerateOpenclOfflineVersionInfo(const std::string& fileName);

    // Get the list of names of supported targets in DeviceInfo format.
    static bool GetSupportedTargets(std::set<std::string>& targets, bool printCmd = false);

private:
    // Map  device or kernel name <--> kernel ISA text.
    typedef std::unordered_map<std::string, std::string>  IsaMap;

    // Identify the devices requested by user.
    bool  InitRequestedAsicListLC(const Config& config);

    // Perform requested compilation.
    bool  Compile(const Config& config);

    // Perform OpenCL compilation.
    beStatus  CompileOpenCL(const Config& config, const OpenCLOptions& ocl_options);

    // Disassemble binary file.
    // The disassembled ISA text are be divided into per-kernel parts and stored in separate files.
    // The names of ISA files are generated based on provided user ISA file name.
    beStatus DisassembleBinary(const std::string& bin_filename, const std::string& user_isa_filename, const std::string& clang_device, const std::string& rga_device,
                                const std::string& kernel, bool line_numbers, std::string& error_text);

    // Parse ISA files and generate separate files that contain parsed ISA in CSV format.
    bool  ParseIsaFilesToCSV(bool add_line_numbers);

    // Add the device name to the output file name provided in "outFileName" string.
    beStatus  AdjustBinaryFileName(const Config&       config,
                                   const std::string & device,
                                   std::string&        bin_filename);

    // Perform live VGPR analysis.
    bool  PerformLiveRegAnalysis(const Config& config);

    // Extract program Control Flow Graph.
    bool  ExtractCFG(const Config& config);

    // Get the AMD GPU metadata from the binary.
    beStatus  ExtractMetadata(const std::string& metadata_filename) const;

    // Extract Resource Usage (statistics) data.
    beStatus  ExtractStatistics(const Config& config);

    // Split ISA text into separate per-kernel ISA fragments and store them into separate files.
    // Puts the names of generated ISA files into output metadata (kcCLICommander::m_outputMetadata).
    bool  SplitISA(const std::string& bin_file, const std::string& isa_text,
                   const std::string& user_isa_filename, const std::string& device,
                   const std::string& kernel, const std::vector<std::string>& kernel_names);

    // Split ISA text into separate per-kernel ISA fragments. The fragments are returned in the
    // "kernelIsaTextMap" map.
    bool  SplitISAText(const std::string& isa_text,
                       const std::vector<std::string>& kernel_names,
                       IsaMap& kernel_isa_text_map) const;

    // Remove unused code from the ISA disassembly.
    bool  ReduceISA(const std::string& bin_file, IsaMap& kernel_isa_texts);

    // Generate RGA CLI session metadata file.
    bool  GenerateSessionMetadata(const Config& config) const;

    // Extract the list of entry points from the source file specified by "fileName".
    static bool  ExtractEntries(const std::string& filename, const Config& config, RgEntryData& entry_data);

    // Delete all temporary files created by RGA.
    void DeleteTempFiles() const;

    // ---- DATA ----

    // Output Metadata
    RgClOutputMetadata  output_metadata_;

    // Alternative compiler paths specified by a user.
    CmpilerPaths   compiler_paths_;

    // Chosen target GPUs.
    std::set<std::string>  targets_;

    // Specifies whether the "-#" option (print commands) is enabled.
    bool  should_print_cmd_;
};

#endif // RGA_RADEONGPUANALYZERCLI_SRC_KC_CLI_COMMANDER_LIGHTNING_H_
