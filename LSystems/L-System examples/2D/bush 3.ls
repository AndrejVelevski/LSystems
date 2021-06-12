#axiom
F

#rules
F: F[+FF][-FF]F[-F][+F]F

#instructions
F: DRAW, 0.1
+: YAW, 36
-: YAW, -36
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