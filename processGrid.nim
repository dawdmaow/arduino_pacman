type
  Pos = tuple[x, y: int]

let
  width = 28
  height = 31
  lines = readLines("level_layout.txt", 31)

var
  walls, points, ghostSpawn, superPoints, gates: seq[Pos]
  teleport1, teleport2, playerSpawn: Pos

for y, line in lines:
  for x, symbol in line:
    case symbol
    of '#': walls.add (x, y)
    of '.': points.add (x, y)
    of 'G': ghostSpawn.add (x, y)
    of 't': teleport1 = (x, y)
    of 'T': teleport2 = (x, y)
    of 'P': playerSpawn = (x, y)
    of '^': superPoints.add (x, y)
    of '-': gates.add (x, y)
    of ' ': discard
    else: quit "Unknown symbol: " & symbol

var s = ""
s.add "#include <Arduino.h>\n\n"
s.add "constexpr byte gridWidth = " & $width & ";\n"
s.add "constexpr byte gridHeight = " & $height & ";\n"
s.add "constexpr byte playerSpawn[2] = {" & $playerSpawn.x & ", " & $playerSpawn.y & "};\n"
s.add "constexpr byte teleport1[2] = {" & $teleport1.x & ", " & $teleport1.y & "};\n"
s.add "constexpr byte teleport2[2] = {" & $teleport2.x & ", " & $teleport2.y & "};\n"
s.add "constexpr byte walls[" & $walls.len & "][2] = {\n"
for i, wall in walls:
  s.add "  {" & $wall.x & ", " & $wall.y & "}"
  if i != walls.len - 1: s.add ","
  s.add "\n"
s.add "};\n"
s.add "constexpr byte default_points[" & $points.len & "][2] = {\n"
for i, point in points:
  s.add "  {" & $point.x & ", " & $point.y & "}"
  if i != points.len - 1: s.add ","
  s.add "\n"
s.add "};\n"
s.add "constexpr byte default_superPoints[" & $superPoints.len & "][2] = {\n"
for i, superPoint in superPoints:
  s.add "  {" & $superPoint.x & ", " & $superPoint.y & "}"
  if i != superPoints.len - 1: s.add ","
  s.add "\n"
s.add "};\n"
s.add "constexpr byte ghostSpawn[" & $ghostSpawn.len & "][2] = {\n"
for i, ghost in ghostSpawn:
  s.add "  {" & $ghost.x & ", " & $ghost.y & "}"
  if i != ghostSpawn.len - 1: s.add ","
  s.add "\n"
s.add "};\n"
s.add "constexpr byte gates[" & $gates.len & "][2] = {\n"
for i, gate in gates:
  s.add "  {" & $gate.x & ", " & $gate.y & "}"
  if i != gates.len - 1: s.add ","
  s.add "\n"
s.add "};\n"

writeFile("src/default_level_layout.cpp", s)
