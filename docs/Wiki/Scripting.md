# Scripting

Lua 5.4 runs in `ScriptHost`. The state opens base, string, math, and table only. `dofile`, `loadfile`, and `load` are removed. `os` and `io` are not opened.

Globals: `world.create`, `world.destroy`, `world.set_position`, `world.get_position`, `time.delta`, `audio.play`, `physics.apply_force`, `input`, `net`. A script error is stored on the component and does not crash the editor.
