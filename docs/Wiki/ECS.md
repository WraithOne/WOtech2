# ECS

An `Entity` is a 64-bit id. The low 32 bits are the slot. The high 32 bits are the generation. Zero is never live.

`World` creates and destroys entities and stores Transform, Camera, MeshRenderer, RigidBody, Collider, AudioSource, Script, Light, and Prefab components. `SpawnPrefab` copies an in-memory prefab. Scenes and prefabs are versioned JSON (`wotech2-scene` / `wotech2-prefab`, version 1).
