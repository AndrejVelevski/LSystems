#axiom
|FA

#rules
A: F+F+F[vF+FvF+vvvF+F+F]+A

#instructions
F: DRAW, 0.1
[: PUSH, 0
]: POP, 0
v: PITCH, 30
+: YAW, 30
|: PITCH, -90

#settings
generations: 20
lineThickness: 0.03
lineThicknessModifier: 0.97
lineLengthModifier: 1
pruneChance: 0
mutationChance: 0.2
mutationFactor: 0.2
