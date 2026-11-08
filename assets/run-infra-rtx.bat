cd /d "%~dp0"
if exist bin\d3d9.dll.rtx_off ren bin\d3d9.dll.rtx_off d3d9.dll
if exist bin\winmm.dll.rtx_off ren bin\winmm.dll.rtx_off winmm.dll
START infra.exe -console -insecure -steam -novid -disable_d3d9_hacks -limitvsconst -softparticlesdefaultoff -disallowhwmorph -no_compressed_verts -nogamepadui +mat_phong 1 %*