# Audio

Audio stays on the existing XAudio2 `AudioEngine` and `AudioSource`. The `AudioSource` component stores the file, volume, loop, and play flag. Lua `audio.play` sets that flag. Playback uses the existing engine; it is not replaced.
