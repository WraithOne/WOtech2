# Editor command API

The editor listens on `127.0.0.1:27182` (`WOtech::kEditorCommandPort`). Each request is one JSON object followed by a newline. The response is one JSON object followed by a newline. The same commands are available as a file of one JSON object per line, executed by `EditorCommandServer::ExecuteFile`.

The socket server and the file runner both call `EditorCommandServer::Execute`, which calls `EditorApi`. Do not duplicate those operations.

## Commands

| cmd | fields | result |
| --- | --- | --- |
| `new_scene` |  | empty world |
| `open_scene` | `path` | loads a `wotech2-scene` version 1 file |
| `save_scene` | `path` | writes the current world |
| `create_entity` | `name` | `{ "ok": true, "id": "<decimal>" }` |
| `destroy_entity` | `id` |  |
| `add_component` | `id`, `type` | types: `transform`, `camera`, `meshRenderer`, `rigidBody`, `collider`, `audioSource`, `script`, `light`, `prefab` |
| `remove_component` | `id`, `type` |  |
| `set_transform` | `id`, `position`, `rotation`, `scale` | arrays of 3 numbers, radians |
| `set_component` | `id`, `type`, `fields` |  |
| `import_gltf` | `path` | records a glTF 2.0 mesh entity |
| `spawn_prefab` | `path` | `wotech2-prefab` version 1 |
| `play` / `stop` |  | play mode flag |
| `export_game` | `path` | writes `scene.json` and `export.json` |
| `screenshot` | `path` | writes a 1x1 PPM if no swap-chain readback is attached |
| `query_scene` |  | `{ "ok": true, "scene": { ... } }` |

Entity ids are decimal strings so they survive JSON numbers.

Example:

```json
{"cmd":"create_entity","name":"block"}
```
