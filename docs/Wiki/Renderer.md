# Renderer

glTF 2.0 import uses cgltf. Materials are metallic-roughness. `RenderPipeline::RenderFeatureFrame` creates an offscreen `R16G16B16A16_FLOAT` target, a shadow map, an SSAO pass over depth, and a Reinhard tonemap into an 8-bit target.

IBL irradiance, prefiltered specular, and the BRDF LUT are computed from a small generated HDR in `Assets/HDRI`. Use https://polyhaven.com/hdris for production images. Do not vendor those files.
