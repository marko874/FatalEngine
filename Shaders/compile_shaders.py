import os

os.system("glslc -fshader-stage=vert Default.vert.glsl -o out/Default.vert.spv")
os.system("glslc -fshader-stage=frag Default.frag.glsl -o out/Default.frag.spv")