# Build Verification Report — 2026-04-03 (fix run)

## Canonical build command

`./tools/gradle_with_jdk21.sh clean :app:assembleDebug --stacktrace`

## Build result

- Status: **SUCCESS**
- APK generated: `app/build/outputs/apk/debug/app-debug.apk`
- Metadata: `app/build/outputs/apk/debug/output-metadata.json`

## Additional validations

- `./tools/gradle_with_jdk21.sh :app:compileDebugJavaWithJavac --stacktrace` → **SUCCESS**
- `./tools/gradle_with_jdk21.sh :app:lintDebug --stacktrace` → **SUCCESS**
- `./tools/gradle_with_jdk21.sh :app:testDebugUnitTest --stacktrace` → **FAILED**

## Unit test failure summary

Compilation errors in test sources (not in main app build):
- `RamInfo.ensureMinimumVmMemoryMb(int)` missing in tests.
- `CrashHandlerTest` contains invalid override.
- `VncCanvasHoverMouseTest` uses mismatched `MotionEvent.obtain(...)` signature.

Main deliverable build (`assembleDebug`) remains successful.
