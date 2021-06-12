#axiom
Y

#rules
X: X[-FFF][+FFF]FX
Y: YFX[+Y][-Y]

#instructions
F: DRAW, 0.1
+: YAW, 25
-: YAW, -25
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