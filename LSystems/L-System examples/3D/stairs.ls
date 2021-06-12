#axiom
A

#rules
A: G+F+G+F+^HvG[vH]+F[vH]+G[vH]+[F]+RA

#instructions
F: DRAW, 0.25
f: MOVE, 0.25
G: DRAW, 1
H: DRAW, 0.125
[: PUSH, 0
]: POP, 0
v: PITCH, 90
^: PITCH, -90
+: YAW, 90
-: YAW, -90
R: YAW, 5

#settings
generations: 100
lineThickness: 0.03
lineThicknessModifier: 1
lineLengthModifier: 1
pruneChance: 0
mutationChance: 0
mutationFactor: 0