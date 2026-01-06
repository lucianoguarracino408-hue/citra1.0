import { v4 as uuidv4 } from 'uuid';

app.use('/api', (req, res, next) => {
  const start = process.hrtime.bigint();
  const traceId = req.headers['x-trace-id'] || uuidv4();

  req.traceId = traceId;
  res.setHeader('X-Trace-Id', traceId);

  const auditEntry = {
    traceId,
    timestamp: new Date().toISOString(),
    userId: req.user?.id ?? 'anonymous',
    role: req.user?.role ?? 'none',
    ip: req.ip,
    method: req.method,
    path: req.originalUrl,
    action: mapAction(req), // ej: MOVE_FORWARD, STOP, SET_MODE
    payload: sanitize(req.body),
    decision: 'pending',
    statusCode: null,
    latencyMs: null
  };

  res.on('finish', () => {
    const end = process.hrtime.bigint();
    auditEntry.latencyMs = Number(end - start) / 1e6;
    auditEntry.statusCode = res.statusCode;
    auditEntry.decision = res.statusCode < 400 ? 'allowed' : 'denied';

    // Nunca bloquear el flujo principal
    try {
      AuditLog.insertAsync(auditEntry);
      // o enqueueAudit(auditEntry)
    } catch (err) {
      console.error('Audit logger failure', err);
    }
  });

  next();
});
