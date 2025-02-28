; REQUIRES: object-emission

; RUN: llvm-as -opaque-pointers=0 < %s -o %t.bc
; RUN: llvm-spirv %t.bc -o %t.spv
; RUN: llvm-spirv -r %t.spv -o - | llvm-dis -o %t.ll

; RUN: llc -mtriple=%triple -O0 -filetype=obj < %t.ll > %t
; RUN: llvm-dwarfdump %t | FileCheck %s

; Also test that the null streamer doesn't crash with debug info.
; RUN: llc -mtriple=%triple -O0 -filetype=null < %t.ll

target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-n8:16:32:64"
target triple = "spir64-unknown-unknown"

; generated from the following source compiled to bitcode with clang -g -O1
; static int i;
; int main() {
;   (void)&i;
; }

; CHECK: debug_info contents
; CHECK: DW_TAG_variable

source_filename = "test/DebugInfo/Generic/global.ll"

; Function Attrs: nounwind readnone uwtable
define i32 @main() #0 !dbg !9 {
entry:
  ret i32 0, !dbg !12
}

attributes #0 = { nounwind readnone uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!7, !8}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !1, producer: "clang version 3.4 ", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !2, globals: !3, imports: !2)
!1 = !DIFile(filename: "global.cpp", directory: "/tmp")
!2 = !{}
!3 = !{!4}
!4 = !DIGlobalVariableExpression(var: !5, expr: !DIExpression())
!5 = !DIGlobalVariable(name: "i", linkageName: "_ZL1i", scope: null, file: !1, line: 1, type: !6, isLocal: true, isDefinition: true)
!6 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!7 = !{i32 2, !"Dwarf Version", i32 3}
!8 = !{i32 1, !"Debug Info Version", i32 3}
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 2, type: !10, isLocal: false, isDefinition: true, scopeLine: 2, virtualIndex: 6, flags: DIFlagPrototyped, isOptimized: true, unit: !0, retainedNodes: !2)
!10 = !DISubroutineType(types: !11)
!11 = !{!6}
!12 = !DILocation(line: 4, scope: !9)

