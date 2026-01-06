{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "properties": {
    "type": {
      "type": "string",
      "enum": ["file_create", "file_modify", "file_delete"]
    },
    "payload": {
      "type": "object"
    },
    "timestamp": {
      "type": "integer"
    }
  },
  "required": ["type", "timestamp"]
}
