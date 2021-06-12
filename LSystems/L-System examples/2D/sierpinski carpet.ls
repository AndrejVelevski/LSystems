#axiom
F

#rules
F: FFF[+FFF+FFF+FFF]

#instructions
F: DRAW, 0.1
+: YAW, 90
-: YAW, -90
[: PUSH, 0
]: POP, 0

#settings
generations: 3
lineThickness: 0.01
lineThicknessModifier: 1
lineLengthModifier: 1
pruneChance: 0
mutationChance: 0
mutationFactor: 0