function sanitize(body) {
  if (!body) return null;

  const clone = { ...body };
  delete clone.password;
  delete clone.token;
  delete clone.secret;

  return clone;
}
