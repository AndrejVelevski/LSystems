#axiom
X

#rules
F: FF
X: F[+X]F[-X]+X

#instructions
F: DRAW, 0.1
+: YAW, 20
-: YAW, -20
[: PUSH, 0
]: POP, 0

#settings
generations: 5
lineThickness: 0.01
lineThicknessModifier: 1
lineLengthModifier: 1
pruneChance: 0
mutationChance: 0
mutationFactor: 0