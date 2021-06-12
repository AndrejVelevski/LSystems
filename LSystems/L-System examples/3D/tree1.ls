#axiom
|FA

#rules
A: [vFA][^FA][<FA][>FA]

#instructions
F: DRAW, 1
[: PUSH, 0
]: POP, 0
v: PITCH, 30
^: PITCH, -30
<: ROLL, 30
>: ROLL, -30
|: PITCH, -90

#settings
generations: 6
lineThickness: 0.1
lineThicknessModifier: 0.7
lineLengthModifier: 0.8
pruneChance: 0.2
mutationChance: 0.2
mutationFactor: 0.2