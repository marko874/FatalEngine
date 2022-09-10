import os

os.system("glslc -fshader-stage=vert Default.vert.glsl -o out/Default.vert.spv")
os.system("glslc -fshader-stage=frag Default.frag.glsl -o out/Default.frag.spv")

os.system("glslc -fshader-stage=vert Model.vert.glsl -o out/Model.vert.spv")
os.system("glslc -fshader-stage=frag Model.frag.glsl -o out/Model.frag.spv")