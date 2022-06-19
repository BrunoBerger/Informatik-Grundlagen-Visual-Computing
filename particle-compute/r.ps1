MSBuild.exe .\build\opengl-starter.sln -m
if ($?) {
  build\Debug\opengl-starter.exe
}
