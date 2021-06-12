#axiom
|F

#rules
F: F<+[vvFvF^F]>-[^FvvF]

#instructions
F: DRAW, 1
[: PUSH, 0
]: POP, 0
v: PITCH, 20
^: PITCH, -20
v: YAW, 40
^: YAW, -40
<: ROLL, 40
>: ROLL, -40
|: PITCH, -90

#settings
generations: 4
lineThickness: 0.1
lineThicknessModifier: 0.7
lineLengthModifier: 0.7
pruneChance: 0.5
mutationChance: 0.2
mutationFactor: 0.2