# Segurança Operacional

## Princípios
- **Fail-safe não bloqueante:** observabilidade não pode travar execução.
- **Menor privilégio:** evitar permissões legadas em Android moderno.
- **Contenção de recurso:** quotas de memória/linhas/bytes em logs.

## Mitigações implementadas
| Vetor | Mitigação |
|---|---|
| Deadlock stdout/stderr | Drenagem paralela (`ProcessOutputDrainer`) |
| DoS local por flood de logs | Rate limit + drop contabilizado + modo degradado |
| Crescimento ilimitado de memória | Ring buffer com limites rígidos |
| Processo órfão/zumbi | Supervisor com timeout + TERM/KILL escalonado |
| Storage legado inseguro | Scoped Storage + SAF em Android 10+ |

## Logging seguro
- Logs extensivos são limitados por token bucket.
- Em saturação, o sistema troca para saída resumida (`DEGRADED`) e registra auditoria.

## Storage e permissões
- Android 10+ prioriza armazenamento interno e SAF.
- Android legado mantém fallback de `WRITE_EXTERNAL_STORAGE`.


## Política de uso de keystore (`vectras.jks`)
- **Classificação:** material criptográfico sensível para assinatura de **release**.
- **Ambiente permitido:** cofre seguro e segredos de CI; proibido manter chave privada em repositório Git.
- **Acesso mínimo:** princípio de menor privilégio; acesso somente para pipeline de release e mantenedores autorizados.
- **Rotação:** obrigatória no máximo a cada 90 dias, ou imediata em caso de suspeita de vazamento.
- **Resposta a incidente:**
  1. Revogar credenciais e descontinuar uso da chave comprometida.
  2. Gerar novo keystore/alias e atualizar segredos do CI.
  3. Auditar histórico de artefatos assinados e publicar comunicado de segurança.
  4. Executar varredura de repositório e histórico para remoção de segredos expostos.

## Controles automatizados
- O CI executa `tools/security/block_sensitive_artifacts.sh` para bloquear inclusão de novos `*.jks`, `*.keystore` e padrões de credenciais sem exceção documentada em `.ci/sensitive-allowlist.txt`.
