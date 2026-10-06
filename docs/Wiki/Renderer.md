# Renderer

glTF 2.0 import uses cgltf. Materials are metallic-roughness. `RenderPipeline::RenderFeatureFrame` renders the feature mesh into an offscreen `R16G16B16A16_FLOAT` target, then Reinhard-tonemaps into an 8-bit target. DeviceDX11 and its `B8G8R8A8` swap chain are unchanged.

`Assets/HDRI/sample_studio.hdr` is a generated Radiance sample, not a Poly Haven download. Production HDRIs come from https://polyhaven.com/hdris. The frame uploads that HDR, projects it to an environment cubemap, convolves an irradiance cubemap and a prefiltered specular cubemap on the GPU, writes a split-sum BRDF LUT, and samples all three from the PBR shader. The same mesh is drawn into the shadow map and sampled with a 3x3 PCF kernel. SSAO remains a screen-space kernel over the mesh depth.
