#!/usr/bin/env python3
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD_FILE = ROOT / "app" / "build.gradle"
SRC_ROOT = ROOT / "app" / "src"
REPORT = ROOT / "reports" / "external-dependency-hotspots.md"

DEP_RE = re.compile(r"^\s*(implementation|testImplementation|androidTestImplementation|annotationProcessor)\s+['\"]([^'\"]+)['\"]")
IMPORT_RE = re.compile(r"^\s*import\s+([a-zA-Z0-9_.]+)")

PACKAGE_HINTS = {
    "androidx.appcompat": ["androidx.appcompat"],
    "com.google.android.material": ["com.google.android.material"],
    "androidx.annotation": ["androidx.annotation"],
    "androidx.core": ["androidx.core"],
    "androidx.drawerlayout": ["androidx.drawerlayout"],
    "androidx.preference": ["androidx.preference"],
    "androidx.swiperefreshlayout": ["androidx.swiperefreshlayout"],
    "androidx.viewpager": ["androidx.viewpager"],
    "com.google.code.gson": ["com.google.gson"],
    "com.squareup.okhttp3": ["okhttp3", "okio"],
    "androidx.window": ["androidx.window"],
    "org.apache.commons": ["org.apache.commons"],
    "androidx.activity": ["androidx.activity"],
    "androidx.constraintlayout": ["androidx.constraintlayout"],
    "androidx.documentfile": ["androidx.documentfile"],
    "androidx.work": ["androidx.work"],
    "com.github.bumptech.glide": ["com.bumptech.glide"],
    "org.robolectric": ["org.robolectric"],
    "org.mockito": ["org.mockito"],
    "androidx.test": ["androidx.test"],
}

REFACTOR_NOTES = {
    "com.google.code.gson:gson": "Reduzir alocações evitando parse completo para objetos grandes; priorizar streaming com JsonReader em caminhos críticos.",
    "com.squareup.okhttp3:okhttp": "Reutilizar singleton de OkHttpClient e pools, evitando novos clients por request para diminuir GC e overhead de conexão.",
    "com.github.bumptech.glide:glide": "Fixar tamanhos alvo, habilitar downsampling e recycle de targets para reduzir picos de heap/GC em listas.",
    "androidx.work:work-runtime": "Consolidar jobs periódicos e evitar enfileiramento redundante; usar constraints mínimas para reduzir wakeups.",
    "org.apache.commons:commons-compress": "Substituir fluxos bufferizados pequenos por buffers fixos maiores em I/O pesado para reduzir churn de objetos.",
}


def parse_dependencies() -> list[tuple[str, str]]:
    deps: list[tuple[str, str]] = []
    for line in BUILD_FILE.read_text(encoding="utf-8").splitlines():
        m = DEP_RE.match(line)
        if not m:
            continue
        cfg, coord = m.groups()
        if ":" not in coord:
            continue
        deps.append((cfg, coord))
    return deps


def collect_imports() -> dict[Path, list[str]]:
    imports_by_file: dict[Path, list[str]] = {}
    for file in SRC_ROOT.rglob("*"):
        if file.suffix not in {".kt", ".java"}:
            continue
        imports: list[str] = []
        try:
            for line in file.read_text(encoding="utf-8", errors="ignore").splitlines():
                m = IMPORT_RE.match(line)
                if m:
                    imports.append(m.group(1))
        except OSError:
            continue
        if imports:
            imports_by_file[file] = imports
    return imports_by_file


def find_matches(coord: str, imports_by_file: dict[Path, list[str]]) -> list[Path]:
    group, artifact, *_ = (coord.split(":") + [""])[:3]
    key = f"{group}:{artifact}"
    hints = PACKAGE_HINTS.get(group, [])
    if not hints:
        hints = [group]

    matched: list[Path] = []
    for file, imports in imports_by_file.items():
        if any(any(imp.startswith(prefix) for prefix in hints) for imp in imports):
            matched.append(file.relative_to(ROOT))
    return sorted(matched)


def main() -> None:
    deps = parse_dependencies()
    imports_by_file = collect_imports()
    REPORT.parent.mkdir(parents=True, exist_ok=True)

    lines: list[str] = []
    lines.append("# External Dependency Hotspots (Performance/GC)")
    lines.append("")
    lines.append("Relatório gerado automaticamente a partir de `app/build.gradle` + imports em `app/src`. Foco: pontos para refatoração visando reduzir GC, overhead e fricção de runtime.")
    lines.append("")
    lines.append("## Dependências externas detectadas")
    lines.append("")
    for cfg, coord in deps:
        lines.append(f"- `{cfg}` → `{coord}`")

    lines.append("")
    lines.append("## Hotspots por dependência")
    lines.append("")

    for cfg, coord in deps:
        files = find_matches(coord, imports_by_file)
        lines.append(f"### `{coord}` ({cfg})")
        note = ""
        group, artifact, *_ = (coord.split(":") + [""])[:3]
        note = REFACTOR_NOTES.get(f"{group}:{artifact}", "Avaliar remoção gradual com módulo autoral equivalente, priorizando caminhos críticos de CPU/memória.")
        lines.append(f"- Oportunidade de refatoração: {note}")
        if files:
            lines.append(f"- Arquivos impactados ({len(files)}):")
            for f in files[:25]:
                lines.append(f"  - `{f.as_posix()}`")
            if len(files) > 25:
                lines.append(f"  - `... +{len(files) - 25} arquivos`")
        else:
            lines.append("- Arquivos impactados: nenhum import direto encontrado no código-fonte atual.")
        lines.append("")

    REPORT.write_text("\n".join(lines).rstrip() + "\n", encoding="utf-8")
    print(f"Report written: {REPORT}")


if __name__ == "__main__":
    main()
