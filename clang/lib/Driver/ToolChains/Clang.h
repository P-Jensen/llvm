//===--- Clang.h - Clang Tool and ToolChain Implementations ====-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CLANG_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CLANG_H

#include "MSVC.h"
#include "clang/Basic/DebugInfoOptions.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/Types.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Option/Option.h"
#include "llvm/Support/raw_ostream.h"

namespace clang {
class ObjCRuntime;
namespace driver {

namespace tools {

/// Clang compiler tool.
class LLVM_LIBRARY_VISIBILITY Clang : public Tool {
  // Indicates whether this instance has integrated backend using
  // internal LLVM infrastructure.
  bool HasBackend;

public:
  static const char *getBaseInputName(const llvm::opt::ArgList &Args,
                                      const InputInfo &Input);
  static const char *getBaseInputStem(const llvm::opt::ArgList &Args,
                                      const InputInfoList &Inputs);
  static const char *getDependencyFileName(const llvm::opt::ArgList &Args,
                                           const InputInfoList &Inputs);

private:
  void AddPreprocessingOptions(Compilation &C, const JobAction &JA,
                               const Driver &D, const llvm::opt::ArgList &Args,
                               llvm::opt::ArgStringList &CmdArgs,
                               const InputInfo &Output,
                               const InputInfoList &Inputs) const;

  void RenderTargetOptions(const llvm::Triple &EffectiveTriple,
                           const llvm::opt::ArgList &Args, bool KernelOrKext,
                           llvm::opt::ArgStringList &CmdArgs) const;

  void AddAArch64TargetArgs(const llvm::opt::ArgList &Args,
                            llvm::opt::ArgStringList &CmdArgs) const;
  void AddARMTargetArgs(const llvm::Triple &Triple,
                        const llvm::opt::ArgList &Args,
                        llvm::opt::ArgStringList &CmdArgs,
                        bool KernelOrKext) const;
  void AddARM64TargetArgs(const llvm::opt::ArgList &Args,
                          llvm::opt::ArgStringList &CmdArgs) const;
  void AddMIPSTargetArgs(const llvm::opt::ArgList &Args,
                         llvm::opt::ArgStringList &CmdArgs) const;
  void AddPPCTargetArgs(const llvm::opt::ArgList &Args,
                        llvm::opt::ArgStringList &CmdArgs) const;
  void AddR600TargetArgs(const llvm::opt::ArgList &Args,
                         llvm::opt::ArgStringList &CmdArgs) const;
  void AddRISCVTargetArgs(const llvm::opt::ArgList &Args,
                          llvm::opt::ArgStringList &CmdArgs) const;
  void AddSparcTargetArgs(const llvm::opt::ArgList &Args,
                          llvm::opt::ArgStringList &CmdArgs) const;
  void AddSystemZTargetArgs(const llvm::opt::ArgList &Args,
                            llvm::opt::ArgStringList &CmdArgs) const;
  void AddX86TargetArgs(const llvm::opt::ArgList &Args,
                        llvm::opt::ArgStringList &CmdArgs) const;
  void AddHexagonTargetArgs(const llvm::opt::ArgList &Args,
                            llvm::opt::ArgStringList &CmdArgs) const;
  void AddLanaiTargetArgs(const llvm::opt::ArgList &Args,
                          llvm::opt::ArgStringList &CmdArgs) const;
  void AddWebAssemblyTargetArgs(const llvm::opt::ArgList &Args,
                                llvm::opt::ArgStringList &CmdArgs) const;
  void AddVETargetArgs(const llvm::opt::ArgList &Args,
                       llvm::opt::ArgStringList &CmdArgs) const;

  enum RewriteKind { RK_None, RK_Fragile, RK_NonFragile };

  ObjCRuntime AddObjCRuntimeArgs(const llvm::opt::ArgList &args,
                                 const InputInfoList &inputs,
                                 llvm::opt::ArgStringList &cmdArgs,
                                 RewriteKind rewrite) const;

  void AddClangCLArgs(const llvm::opt::ArgList &Args, types::ID InputType,
                      llvm::opt::ArgStringList &CmdArgs,
                      codegenoptions::DebugInfoKind *DebugInfoKind,
                      bool *EmitCodeView) const;

  void ConstructHostCompilerJob(Compilation &C, const JobAction &JA,
                                const InputInfo &Output,
                                const InputInfoList &Inputs,
                                const llvm::opt::ArgList &TCArgs) const;

  mutable std::unique_ptr<llvm::raw_fd_ostream> CompilationDatabase = nullptr;
  void DumpCompilationDatabase(Compilation &C, StringRef Filename,
                               StringRef Target,
                               const InputInfo &Output, const InputInfo &Input,
                               const llvm::opt::ArgList &Args) const;

  void DumpCompilationDatabaseFragmentToDir(
      StringRef Dir, Compilation &C, StringRef Target, const InputInfo &Output,
      const InputInfo &Input, const llvm::opt::ArgList &Args) const;

public:
  Clang(const ToolChain &TC, bool HasIntegratedBackend = true);
  ~Clang() override;

  bool hasGoodDiagnostics() const override { return true; }
  bool hasIntegratedAssembler() const override { return true; }
  bool hasIntegratedBackend() const override { return HasBackend; }
  bool hasIntegratedCPP() const override { return true; }
  bool canEmitIR() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Clang integrated assembler tool.
class LLVM_LIBRARY_VISIBILITY ClangAs : public Tool {
public:
  ClangAs(const ToolChain &TC)
      : Tool("clang::as", "clang integrated assembler", TC) {}
  void AddMIPSTargetArgs(const llvm::opt::ArgList &Args,
                         llvm::opt::ArgStringList &CmdArgs) const;
  void AddX86TargetArgs(const llvm::opt::ArgList &Args,
                        llvm::opt::ArgStringList &CmdArgs) const;
  void AddRISCVTargetArgs(const llvm::opt::ArgList &Args,
                          llvm::opt::ArgStringList &CmdArgs) const;
  bool hasGoodDiagnostics() const override { return true; }
  bool hasIntegratedAssembler() const override { return false; }
  bool hasIntegratedCPP() const override { return false; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Offload bundler tool.
class LLVM_LIBRARY_VISIBILITY OffloadBundler final : public Tool {
public:
  OffloadBundler(const ToolChain &TC)
      : Tool("offload bundler", "clang-offload-bundler", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
  void ConstructJobMultipleOutputs(Compilation &C, const JobAction &JA,
                                   const InputInfoList &Outputs,
                                   const InputInfoList &Inputs,
                                   const llvm::opt::ArgList &TCArgs,
                                   const char *LinkingOutput) const override;
};

/// Offload wrapper tool.
class LLVM_LIBRARY_VISIBILITY OffloadWrapper final : public Tool {
public:
  OffloadWrapper(const ToolChain &TC)
      : Tool("offload wrapper", "clang-offload-wrapper", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Offload binary tool.
class LLVM_LIBRARY_VISIBILITY OffloadPackager final : public Tool {
public:
  OffloadPackager(const ToolChain &TC)
      : Tool("Offload::Packager", "clang-offload-packager", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Offload deps tool.
class LLVM_LIBRARY_VISIBILITY OffloadDeps final : public Tool {
  void constructJob(Compilation &C, const JobAction &JA,
                    ArrayRef<InputInfo> Outputs, ArrayRef<InputInfo> Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const;

public:
  OffloadDeps(const ToolChain &TC)
      : Tool("offload deps", "clang-offload-deps", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
  void ConstructJobMultipleOutputs(Compilation &C, const JobAction &JA,
                                   const InputInfoList &Outputs,
                                   const InputInfoList &Inputs,
                                   const llvm::opt::ArgList &TCArgs,
                                   const char *LinkingOutput) const override;
};

/// SPIR-V translator tool.
class LLVM_LIBRARY_VISIBILITY SPIRVTranslator final : public Tool {
public:
  SPIRVTranslator(const ToolChain &TC)
      : Tool("SPIR-V translator", "llvm-spirv", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// SPIR Checking tool.
class LLVM_LIBRARY_VISIBILITY SPIRCheck final : public Tool {
public:
  SPIRCheck(const ToolChain &TC)
      : Tool("SPIR Checker", "llvm-no-spir-kernel", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// SYCL post-link device code processing tool.
class LLVM_LIBRARY_VISIBILITY SYCLPostLink final : public Tool {
public:
  SYCLPostLink(const ToolChain &TC)
      : Tool("SYCL post link", "sycl-post-link", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool hasGoodDiagnostics() const override { return true; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// File table transformation tool.
class LLVM_LIBRARY_VISIBILITY FileTableTform final : public Tool {
public:
  FileTableTform(const ToolChain &TC)
      : Tool("File table transformation", "file-table-tform", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool hasGoodDiagnostics() const override { return true; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Append Footer tool
class LLVM_LIBRARY_VISIBILITY AppendFooter final : public Tool {
public:
  AppendFooter(const ToolChain &TC)
      : Tool("Append Footer to source", "append-file", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool hasGoodDiagnostics() const override { return true; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// SPIR-V to LLVM-IR wrapper tool
class LLVM_LIBRARY_VISIBILITY SpirvToIrWrapper final : public Tool {
public:
  SpirvToIrWrapper(const ToolChain &TC)
      : Tool("Convert SPIR-V to LLVM-IR if needed", "spirv-to-ir-wrapper", TC) {
  }

  bool hasIntegratedCPP() const override { return false; }
  bool hasGoodDiagnostics() const override { return true; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

/// Linker wrapper tool.
class LLVM_LIBRARY_VISIBILITY LinkerWrapper final : public Tool {
  const Tool *Linker;

public:
  LinkerWrapper(const ToolChain &TC, const Tool *Linker)
      : Tool("Offload::Linker", "linker", TC), Linker(Linker) {}

  bool hasIntegratedCPP() const override { return false; }
  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

} // end namespace tools

} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_CLANG_H
