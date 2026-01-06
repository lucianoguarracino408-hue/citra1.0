function mapAction(req) {
  if (req.path.includes('/move')) return 'ROBOT_MOVE';
  if (req.path.includes('/stop')) return 'ROBOT_STOP';
  if (req.path.includes('/mode')) return 'SET_OPERATION_MODE';
  return 'UNKNOWN_ACTION';
}
