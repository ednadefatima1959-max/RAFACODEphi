#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
SDK_ROOT="${ANDROID_SDK_ROOT:-/workspace/android-sdk}"
JAVA_HOME_CANDIDATE="${JAVA_HOME:-/usr/lib/jvm/java-17-openjdk-amd64}"
APK_PATH="$ROOT_DIR/app/build/outputs/apk/release/app-release.apk"

log(){ printf '[APK-BUILD] %s\n' "$*"; }

if [[ -x "$JAVA_HOME_CANDIDATE/bin/java" ]]; then
  export JAVA_HOME="$JAVA_HOME_CANDIDATE"
  export PATH="$JAVA_HOME/bin:$PATH"
fi

if [[ ! -d "$SDK_ROOT" ]]; then
  log "ANDROID_SDK_ROOT não encontrado em $SDK_ROOT"
  log "Defina ANDROID_SDK_ROOT para um SDK válido antes de executar este script."
  exit 1
fi

if [[ ! -f "$ROOT_DIR/vectras.jks" ]]; then
  log "Keystore esperado não encontrado: $ROOT_DIR/vectras.jks"
  exit 1
fi

export ANDROID_SDK_ROOT="$SDK_ROOT"
export PATH="$ANDROID_SDK_ROOT/platform-tools:$PATH"

log "Iniciando build release assinado"
(
  cd "$ROOT_DIR"
  ./gradlew :app:assembleRelease --stacktrace
)

if [[ ! -f "$APK_PATH" ]]; then
  log "APK não encontrado em $APK_PATH"
  exit 2
fi

log "Verificando assinatura do APK"
"$ANDROID_SDK_ROOT/build-tools/35.0.0/apksigner" verify --verbose --print-certs "$APK_PATH"

log "Checksum SHA-256"
sha256sum "$APK_PATH"

log "Concluído: $APK_PATH"
