# External Dependency Hotspots (Performance/GC)

Relatório gerado automaticamente a partir de `app/build.gradle` + imports em `app/src`. Foco: pontos para refatoração visando reduzir GC, overhead e fricção de runtime.

## Dependências externas detectadas

- `implementation` → `androidx.appcompat:appcompat:1.7.1`
- `implementation` → `com.google.android.material:material:1.13.0`
- `implementation` → `androidx.annotation:annotation:1.9.1`
- `implementation` → `androidx.core:core-ktx:1.13.1`
- `implementation` → `androidx.drawerlayout:drawerlayout:1.2.0`
- `implementation` → `androidx.preference:preference-ktx:1.2.1`
- `implementation` → `androidx.swiperefreshlayout:swiperefreshlayout:1.1.0`
- `implementation` → `androidx.viewpager:viewpager:1.1.0`
- `implementation` → `com.google.code.gson:gson:2.13.2`
- `implementation` → `com.squareup.okhttp3:okhttp:4.12.0`
- `implementation` → `androidx.window:window:1.5.1`
- `implementation` → `org.apache.commons:commons-compress:1.28.0`
- `implementation` → `androidx.activity:activity-ktx:1.9.2`
- `implementation` → `androidx.constraintlayout:constraintlayout:2.2.1`
- `implementation` → `androidx.documentfile:documentfile:1.1.0`
- `implementation` → `androidx.work:work-runtime:2.9.1`
- `implementation` → `com.github.bumptech.glide:glide:4.16.0`
- `annotationProcessor` → `com.github.bumptech.glide:compiler:4.16.0`
- `testImplementation` → `junit:junit:4.13.2`
- `testImplementation` → `org.robolectric:robolectric:4.14.1`
- `testImplementation` → `androidx.test:core:1.6.1`
- `testImplementation` → `org.mockito:mockito-core:5.12.0`
- `testImplementation` → `org.mockito:mockito-inline:5.2.0`
- `androidTestImplementation` → `androidx.test.ext:junit:1.3.0`
- `androidTestImplementation` → `androidx.test.espresso:espresso-core:3.7.0`

## Hotspots por dependência

### `androidx.appcompat:appcompat:1.7.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (47):
  - `app/src/main/java/android/androidVNC/Utils.java`
  - `app/src/main/java/android/androidVNC/VncCanvas.java`
  - `app/src/main/java/android/androidVNC/VncCanvasActivity.java`
  - `app/src/main/java/com/vectras/qemu/MainSettingsManager.java`
  - `app/src/main/java/com/vectras/qemu/MainVNCActivity.java`
  - `app/src/main/java/com/vectras/vm/AboutActivity.java`
  - `app/src/main/java/com/vectras/vm/CqcmActivity.java`
  - `app/src/main/java/com/vectras/vm/DataExplorerActivity.java`
  - `app/src/main/java/com/vectras/vm/ExportRomActivity.java`
  - `app/src/main/java/com/vectras/vm/Fragment/CreateImageDialogFragment.java`
  - `app/src/main/java/com/vectras/vm/ImagePrvActivity.java`
  - `app/src/main/java/com/vectras/vm/Minitools.java`
  - `app/src/main/java/com/vectras/vm/QemuParamsEditorActivity.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/RomReceiverActivity.java`
  - `app/src/main/java/com/vectras/vm/SetArchActivity.java`
  - `app/src/main/java/com/vectras/vm/SplashActivity.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/VMManager.java`
  - `app/src/main/java/com/vectras/vm/WebViewActivity.java`
  - `app/src/main/java/com/vectras/vm/benchmark/BenchmarkActivity.java`
  - `app/src/main/java/com/vectras/vm/crashtracker/LastCrashActivity.java`
  - `app/src/main/java/com/vectras/vm/localization/LanguageModulesFragment.kt`
  - `app/src/main/java/com/vectras/vm/main/MainActivity.java`
  - `app/src/main/java/com/vectras/vm/main/core/MainStartVM.java`
  - `... +22 arquivos`

### `com.google.android.material:material:1.13.0` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (28):
  - `app/src/main/java/com/termux/app/ExtraKeysView.java`
  - `app/src/main/java/com/termux/shared/termux/extrakeys/ExtraKeysView.java`
  - `app/src/main/java/com/vectras/qemu/MainSettingsManager.java`
  - `app/src/main/java/com/vectras/vm/AboutActivity.java`
  - `app/src/main/java/com/vectras/vm/ExportRomActivity.java`
  - `app/src/main/java/com/vectras/vm/Fragment/CreateImageDialogFragment.java`
  - `app/src/main/java/com/vectras/vm/ImagePrvActivity.java`
  - `app/src/main/java/com/vectras/vm/Minitools.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/VMManager.java`
  - `app/src/main/java/com/vectras/vm/VectrasApp.java`
  - `app/src/main/java/com/vectras/vm/benchmark/BenchmarkActivity.java`
  - `app/src/main/java/com/vectras/vm/main/MainActivity.java`
  - `app/src/main/java/com/vectras/vm/main/core/MainStartVM.java`
  - `app/src/main/java/com/vectras/vm/main/core/RomOptionsDialog.java`
  - `app/src/main/java/com/vectras/vm/main/monitor/SystemMonitorFragment.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreFragment.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreFragment.java`
  - `app/src/main/java/com/vectras/vm/main/vms/VmsFragment.java`
  - `app/src/main/java/com/vectras/vm/rafaelia/RafaeliaBenchManager.kt`
  - `app/src/main/java/com/vectras/vm/settings/LanguageModulesActivity.kt`
  - `app/src/main/java/com/vectras/vm/settings/ThemeActivity.java`
  - `app/src/main/java/com/vectras/vm/setupwizard/SetupWizard2Activity.java`
  - `app/src/main/java/com/vectras/vm/tools/ProfessionalToolsActivity.java`
  - `app/src/main/java/com/vectras/vm/utils/DialogUtils.java`
  - `... +3 arquivos`

### `androidx.annotation:annotation:1.9.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (61):
  - `app/src/main/java/android/androidVNC/VncCursorView.java`
  - `app/src/main/java/com/termux/app/ExtraKeysInfos.java`
  - `app/src/main/java/com/termux/app/TermuxActivity.java`
  - `app/src/main/java/com/termux/app/TermuxOpenReceiver.java`
  - `app/src/main/java/com/termux/app/TermuxPreferences.java`
  - `app/src/main/java/com/termux/shared/termux/extrakeys/ExtraKeyButton.java`
  - `app/src/main/java/com/termux/shared/termux/extrakeys/ExtraKeysInfo.java`
  - `app/src/main/java/com/termux/shared/termux/extrakeys/ExtraKeysView.java`
  - `app/src/main/java/com/termux/shared/termux/extrakeys/SpecialButton.java`
  - `app/src/main/java/com/vectras/qemu/MainSettingsManager.java`
  - `app/src/main/java/com/vectras/qemu/MainVNCActivity.java`
  - `app/src/main/java/com/vectras/qemu/SettingsFragment.java`
  - `app/src/main/java/com/vectras/vm/AboutActivity.java`
  - `app/src/main/java/com/vectras/vm/Fragment/CreateImageDialogFragment.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/RomReceiverActivity.java`
  - `app/src/main/java/com/vectras/vm/VMManager.java`
  - `app/src/main/java/com/vectras/vm/VectrasApp.java`
  - `app/src/main/java/com/vectras/vm/adapters/GithubUserAdapter.java`
  - `app/src/main/java/com/vectras/vm/core/HardwareProfileBridge.java`
  - `app/src/main/java/com/vectras/vm/core/ProcessLaunch.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadCoordinator.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadItemState.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadPathResolver.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadStateReconciler.java`
  - `... +36 arquivos`

### `androidx.core:core-ktx:1.13.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (15):
  - `app/src/main/java/com/k2/zoomimageview/ZoomImageView.kt`
  - `app/src/main/java/com/vectras/qemu/utils/FileUtils.java`
  - `app/src/main/java/com/vectras/vm/DataExplorerActivity.java`
  - `app/src/main/java/com/vectras/vm/MainService.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadWorker.java`
  - `app/src/main/java/com/vectras/vm/main/MainActivity.java`
  - `app/src/main/java/com/vectras/vm/utils/FileUtils.java`
  - `app/src/main/java/com/vectras/vm/utils/NotificationUtils.java`
  - `app/src/main/java/com/vectras/vm/utils/PermissionUtils.java`
  - `app/src/main/java/com/vectras/vm/utils/UIUtils.java`
  - `app/src/main/java/com/vectras/vm/x11/LoriePreferences.java`
  - `app/src/main/java/com/vectras/vm/x11/X11Activity.java`
  - `app/src/main/java/com/vectras/vm/x11/input/TouchInputHandler.java`

### `androidx.drawerlayout:drawerlayout:1.2.0` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (3):
  - `app/src/main/java/com/termux/app/ExtraKeysView.java`
  - `app/src/main/java/com/termux/app/TermuxActivity.java`
  - `app/src/main/java/com/termux/app/TermuxViewClient.java`

### `androidx.preference:preference-ktx:1.2.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (9):
  - `app/src/main/java/com/vectras/qemu/MainSettingsManager.java`
  - `app/src/main/java/com/vectras/qemu/SettingsFragment.java`
  - `app/src/main/java/com/vectras/vm/SplashActivity.java`
  - `app/src/main/java/com/vectras/vm/rafaelia/RafaeliaBenchManager.kt`
  - `app/src/main/java/com/vectras/vm/rafaelia/RafaeliaConfig.kt`
  - `app/src/main/java/com/vectras/vm/rafaelia/RafaeliaSettings.kt`
  - `app/src/main/java/com/vectras/vm/setupwizard/FirstRunPermissionOrchestrator.kt`
  - `app/src/main/java/com/vectras/vm/setupwizard/SetupWizard2Activity.java`
  - `app/src/main/java/com/vectras/vm/x11/LoriePreferences.java`

### `androidx.swiperefreshlayout:swiperefreshlayout:1.1.0` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.

### `androidx.viewpager:viewpager:1.1.0` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (4):
  - `app/src/main/java/com/termux/app/TermuxActivity.java`
  - `app/src/main/java/com/vectras/vm/x11/X11Activity.java`
  - `app/src/main/java/com/vectras/vm/x11/utils/TermuxX11ExtraKeys.java`
  - `app/src/main/java/com/vectras/vm/x11/utils/X11ToolbarViewPager.java`

### `com.google.code.gson:gson:2.13.2` (implementation)
- Oportunidade de refatoração: Reduzir alocações evitando parse completo para objetos grandes; priorizar streaming com JsonReader em caminhos críticos.
- Arquivos impactados (10):
  - `app/src/main/java/com/vectras/vm/CqcmActivity.java`
  - `app/src/main/java/com/vectras/vm/ExportRomActivity.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/VMManager.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/DataRoms.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreFragment.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreFragment.java`
  - `app/src/main/java/com/vectras/vm/setupwizard/SetupWizard2Activity.java`
  - `app/src/main/java/com/vectras/vm/utils/JSONUtils.java`
  - `app/src/test/java/com/vectras/vm/VMManagerRestoreVMsJsonAppendTest.java`

### `com.squareup.okhttp3:okhttp:4.12.0` (implementation)
- Oportunidade de refatoração: Reutilizar singleton de OkHttpClient e pools, evitando novos clients por request para diminuir GC e overhead de conexão.
- Arquivos impactados (2):
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadWorker.java`

### `androidx.window:window:1.5.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.

### `org.apache.commons:commons-compress:1.28.0` (implementation)
- Oportunidade de refatoração: Substituir fluxos bufferizados pequenos por buffers fixos maiores em I/O pesado para reduzir churn de objetos.
- Arquivos impactados (1):
  - `app/src/main/java/com/vectras/vm/utils/TarUtils.java`

### `androidx.activity:activity-ktx:1.9.2` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (20):
  - `app/src/main/java/com/vectras/vm/ExportRomActivity.java`
  - `app/src/main/java/com/vectras/vm/Minitools.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/WebViewActivity.java`
  - `app/src/main/java/com/vectras/vm/benchmark/BenchmarkActivity.java`
  - `app/src/main/java/com/vectras/vm/crashtracker/LastCrashActivity.java`
  - `app/src/main/java/com/vectras/vm/main/MainActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/ExternalVNCSettingsActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/ImportExportSettingsActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/LanguageModulesActivity.kt`
  - `app/src/main/java/com/vectras/vm/settings/ThemeActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/UpdaterActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/VNCSettingsActivity.java`
  - `app/src/main/java/com/vectras/vm/settings/X11DisplaySettingsActivity.java`
  - `app/src/main/java/com/vectras/vm/setupwizard/SetupWizard2Activity.java`
  - `app/src/main/java/com/vectras/vm/tools/ProfessionalToolsActivity.java`
  - `app/src/main/java/com/vectras/vm/utils/PermissionUtils.java`
  - `app/src/main/java/com/vectras/vm/utils/UIUtils.java`
  - `app/src/main/java/com/vectras/vm/x11/X11Activity.java`

### `androidx.constraintlayout:constraintlayout:2.2.1` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.

### `androidx.documentfile:documentfile:1.1.0` (implementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (3):
  - `app/src/main/java/com/vectras/qemu/utils/FileInstaller.java`
  - `app/src/main/java/com/vectras/qemu/utils/FileUtils.java`
  - `app/src/main/java/com/vectras/vm/utils/PermissionUtils.java`

### `androidx.work:work-runtime:2.9.1` (implementation)
- Oportunidade de refatoração: Consolidar jobs periódicos e evitar enfileiramento redundante; usar constraints mínimas para reduzir wakeups.
- Arquivos impactados (6):
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadCoordinator.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadStateReconciler.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadViewModel.java`
  - `app/src/main/java/com/vectras/vm/download/DownloadWorker.java`
  - `app/src/main/java/com/vectras/vm/importer/ImportSessionWorker.java`

### `com.github.bumptech.glide:glide:4.16.0` (implementation)
- Oportunidade de refatoração: Fixar tamanhos alvo, habilitar downsampling e recycle de targets para reduzir picos de heap/GC em listas.
- Arquivos impactados (10):
  - `app/src/main/java/com/vectras/vm/ImagePrvActivity.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/main/core/MainStartVM.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreHomeAdapterSearch.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreHomeAdpater.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreHomeAdapter.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreHomeAdapterSearch.java`
  - `app/src/main/java/com/vectras/vm/main/vms/VmsHomeAdapter.java`
  - `app/src/main/java/com/vectras/vm/utils/NotificationUtils.java`

### `com.github.bumptech.glide:compiler:4.16.0` (annotationProcessor)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (10):
  - `app/src/main/java/com/vectras/vm/ImagePrvActivity.java`
  - `app/src/main/java/com/vectras/vm/RomInfo.java`
  - `app/src/main/java/com/vectras/vm/VMCreatorActivity.java`
  - `app/src/main/java/com/vectras/vm/main/core/MainStartVM.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreHomeAdapterSearch.java`
  - `app/src/main/java/com/vectras/vm/main/romstore/RomStoreHomeAdpater.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreHomeAdapter.java`
  - `app/src/main/java/com/vectras/vm/main/softwarestore/SoftwareStoreHomeAdapterSearch.java`
  - `app/src/main/java/com/vectras/vm/main/vms/VmsHomeAdapter.java`
  - `app/src/main/java/com/vectras/vm/utils/NotificationUtils.java`

### `junit:junit:4.13.2` (testImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.

### `org.robolectric:robolectric:4.14.1` (testImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (10):
  - `app/src/test/java/android/androidVNC/VncCanvasHoverMouseTest.java`
  - `app/src/test/java/com/termux/app/BackgroundJobTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMEnvNullCdromPathTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMVncPasswordCliRegressionTest.java`
  - `app/src/test/java/com/vectras/vm/benchmark/BenchmarkManagerTest.java`
  - `app/src/test/java/com/vectras/vm/crashtracker/CrashHandlerTest.java`
  - `app/src/test/java/com/vectras/vm/setupwizard/SetupFeatureCoreBootstrapValidationTest.java`
  - `app/src/test/java/com/vectras/vm/setupwizard/SetupWizard2ActivityUrlSanitizationTest.java`
  - `app/src/test/java/com/vectras/vm/utils/FileUtilsGetPathExternalStorageTest.java`
  - `app/src/test/java/com/vectras/vm/utils/FileUtilsOpenModeTest.java`

### `androidx.test:core:1.6.1` (testImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (2):
  - `app/src/test/java/com/vectras/vm/localization/LocaleManagerTest.kt`
  - `app/src/test/java/com/vectras/vm/utils/FileUtilsGetPathExternalStorageTest.java`

### `org.mockito:mockito-core:5.12.0` (testImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (4):
  - `app/src/test/java/com/termux/app/BackgroundJobTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMEnvNullCdromPathTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMVncPasswordCliRegressionTest.java`
  - `app/src/test/java/com/vectras/vm/core/ProotCommandBuilderTest.java`

### `org.mockito:mockito-inline:5.2.0` (testImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados (4):
  - `app/src/test/java/com/termux/app/BackgroundJobTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMEnvNullCdromPathTest.java`
  - `app/src/test/java/com/vectras/vm/StartVMVncPasswordCliRegressionTest.java`
  - `app/src/test/java/com/vectras/vm/core/ProotCommandBuilderTest.java`

### `androidx.test.ext:junit:1.3.0` (androidTestImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.

### `androidx.test.espresso:espresso-core:3.7.0` (androidTestImplementation)
- Oportunidade de refatoração: Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.
- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.
