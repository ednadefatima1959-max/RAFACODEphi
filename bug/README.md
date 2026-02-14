# bug/

Repositório de diagnósticos, enumeração de falhas, propostas de correção e guias operacionais de patch para o ciclo de bugs do Vectras VM Android.

## Escopo
- Consolidar investigação técnica de bugs, impacto funcional e rastreabilidade de correções.
- Reunir artefatos de análise (sumários, matrizes, guias de implementação e documentos de deploy).
- Servir como camada temática focada em falhas dentro do modelo de documentação do projeto.

## Objetivo
- Facilitar triagem e priorização de falhas a partir de um ponto único.
- Conectar enumeração (`BUGS_ENUMERATION.md`) com remediação (`BUG_FIXES.md` e patches associados).
- Preservar histórico técnico para auditoria, validação e retomada de contexto.

## Relação com `docs/`, `reports/` e raiz
- **Com `docs/`**: `bug/` é especializado em ciclo de bugs; `docs/` permanece como curadoria técnica transversal e base institucional.
- **Com `reports/`**: `reports/` agrega relatórios executivos/operacionais mais amplos; `bug/` mantém foco em defeitos, correções e guias de implementação.
- **Com a raiz**: a raiz (`README.md`, `DOC_INDEX.md`) funciona como camada de navegação global e indexa `bug/` junto aos demais diretórios padronizados.

## Consistência de nomenclatura
Validação alinhada ao padrão aplicado em `engine/`, `tools/` e `docs/`:
- Presença do par obrigatório `README.md` + `FILES_MAP.md` no diretório.
- Arquivos centrais de documentação em `UPPER_SNAKE_CASE.md` (ex.: `BUGS_ENUMERATION.md`, `BUG_FIXES.md`, `SUMARIO_EXECUTIVO.md`).
- Arquivo de síntese rápida fora do padrão Markdown mantido explicitamente (`SIGMA_SUMMARY.txt`) e descrito no mapa local.

## Navegação local
- [README.md](README.md)
- [FILES_MAP.md](FILES_MAP.md)
