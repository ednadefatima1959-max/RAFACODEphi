# Documentação Técnica do Vectras VM Android

Este diretório centraliza a documentação técnica, operacional e de governança do projeto.

## Princípios desta documentação

1. Representar o estado real do código no repositório.
2. Evitar conteúdo especulativo ou sem referência verificável.
3. Manter linguagem formal, objetiva e auditável.
4. Facilitar manutenção contínua conforme a evolução da base de código.

## Leitura recomendada por ordem

1. [DOC_INDEX.md](../DOC_INDEX.md) — visão rápida dos documentos principais.
2. [ARCHITECTURE.md](ARCHITECTURE.md) — visão arquitetural e componentes.
3. [SOURCE_TRACEABILITY_MATRIX.md](SOURCE_TRACEABILITY_MATRIX.md) — rastreabilidade entre código e documentação.
4. [PERFORMANCE_INTEGRITY.md](PERFORMANCE_INTEGRITY.md) e [BENCHMARKS.md](BENCHMARKS.md) — desempenho e medição.
5. [LEGAL_AND_LICENSES.md](LEGAL_AND_LICENSES.md) — licenças, conformidade e atribuição.

## Organização por assunto

### Arquitetura e implementação

- [ARCHITECTURE.md](ARCHITECTURE.md)
- [INTEGRACAO_RM_QEMU_ANDROIDX.md](INTEGRACAO_RM_QEMU_ANDROIDX.md)
- [API.md](API.md)
- [VECTRA_CORE.md](../VECTRA_CORE.md)

### Operação, desempenho e validação

- [PERFORMANCE_INTEGRITY.md](PERFORMANCE_INTEGRITY.md)
- [BENCHMARKS.md](BENCHMARKS.md)
- [BENCHMARK_MANAGER.md](BENCHMARK_MANAGER.md)
- [navigation/PERFORMANCE_OPERATIONS.md](navigation/PERFORMANCE_OPERATIONS.md)

### Governança documental e contribuição

- [DOCUMENTATION_STANDARDS.md](DOCUMENTATION_STANDARDS.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [ROADMAP.md](ROADMAP.md)
- [DOCUMENTATION_SUMMARY.md](DOCUMENTATION_SUMMARY.md)

### Referências e base acadêmica

- [ABSTRACT.md](ABSTRACT.md)
- [RESUMO.md](RESUMO.md)
- [PREFACE.md](PREFACE.md)
- [BIBLIOGRAPHY.md](BIBLIOGRAPHY.md)
- [GLOSSARY.md](GLOSSARY.md)

### Compliance e propriedade intelectual

- [LEGAL_AND_LICENSES.md](LEGAL_AND_LICENSES.md)
- [IP_MAP.md](IP_MAP.md)
- [SOURCE_TRACEABILITY_MATRIX.md](SOURCE_TRACEABILITY_MATRIX.md)

## Navegação por perfil

- [navigation/INDEX.md](navigation/INDEX.md)
- [navigation/ENTERPRISE_COMPANIES.md](navigation/ENTERPRISE_COMPANIES.md)
- [navigation/SCIENTISTS_RESEARCH.md](navigation/SCIENTISTS_RESEARCH.md)
- [navigation/UNIVERSITIES_ACADEMIC.md](navigation/UNIVERSITIES_ACADEMIC.md)
- [navigation/BENCHMARK_COMPARISONS.md](navigation/BENCHMARK_COMPARISONS.md)

## Política de atualização

- Sempre atualizar o documento técnico quando houver alteração estrutural em módulo, build ou fluxo operacional.
- Registrar mudanças relevantes de documentação no histórico do Git junto ao código associado.
- Priorizar consistência com arquivos fonte (`settings.gradle`, `build.gradle`, `app/build.gradle` e diretórios de módulo).
