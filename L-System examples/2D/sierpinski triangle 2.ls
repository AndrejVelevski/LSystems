#axiom
YF

#rules
X: YF+XF+Y
Y: XF-YF-X

#instructions
F: DRAW, 0.1
+: YAW, 60
-: YAW, -60

#settings
generations: 5
lineThickness: 0.01
lineThicknessModifier: 1
lineLengthModifier: 1
pruneChance: 0
mutationChance: 0
mutationFactor: 0