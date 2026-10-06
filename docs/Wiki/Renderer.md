# Renderer

glTF 2.0 import uses cgltf. Materials are metallic-roughness. `RenderPipeline::RenderFeatureFrame` creates an offscreen `R16G16B16A16_FLOAT` target, a shadow map, an SSAO pass over depth, and a Reinhard tonemap into an 8-bit target.

`Assets/HDRI/sample_studio.hdr` is a generated Radiance sample, not a Poly Haven download. Production HDRIs come from https://polyhaven.com/hdris. The feature frame allocates the HDR target, shadow map, SSAO kernel pass, and tonemap. A full irradiance and prefiltered specular convolution is the next renderer step on top of that sample.
