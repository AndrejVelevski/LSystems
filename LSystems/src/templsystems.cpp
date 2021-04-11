/*mLSystem = LSystem( //Koch's curve
		"F",
		{
			{'F', "F-F++F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 60}},
			{'-', {LSystem::TURN, -60}}
		}
	);*/

	/*mLSystem = LSystem( //Koch's snowflake
		"F++F++F",
		{
			{'F', "F-F++F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 60}},
			{'-', {LSystem::TURN, -60}}
		}
	);*/

	/*mLSystem = LSystem( //Unnamed
		"F+F+F",
		{
			{'F', "F-F+F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 120}},
			{'-', {LSystem::TURN, -120}}
		}
	);*/

	/*mLSystem = LSystem( //Ice fractals
		"F+F+F+F",
		{
			{'F', "FF+F++F+F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}}
		}
	);*/

	/*mLSystem = LSystem( //Dragon curve
		"FX",
		{
			{'X', "X+YF+"},
			{'Y', "-FX-Y"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Gosper's curve
		"XF",
		{
			{'X', "X+YF++YF-FX--FXFX-YF+"},
			{'Y', "-FX+YFYF++YF+FX--FX-Y"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 60}},
			{'-', {LSystem::TURN, -60}}
		}
	);*/

	/*mLSystem = LSystem( //Serpinski curve
		"F+XF+F+XF",
		{
			{'X', "XF-F+F-XF+F+XF-F+F-X"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Gilbert's curve
		"X",
		{
			{'X', "-YF+XFX+FY-"},
			{'Y', "+XF-YFY-FX+"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Unnamed
		"F+F+F+F",
		{
			{'F', "FF+F+F+F+FF"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}}
		}
	);*/

	/*mLSystem = LSystem( //Generalizations of the Koch's curve 1
		"F+F+F+F",
		{
			{'F', "F+F-F-FF+F+F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Generalizations of the Koch's curve 2
		"F+F+F+F",
		{
			{'F', "F+F-F-FFF+F+F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Generalizations of the Koch's curve 3
		"F+F+F+F",
		{
			{'F', "F-FF+FF+F+F-F-FF+F+F-F-FF-FF+F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Quadratic Snowflake
		"F",
		{
			{'F', "F-F+F+F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Unnamed
		"YF",
		{
			{'X', "YF+XF+Y"},
			{'Y', "XF-YF-X"},
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 60}},
			{'-', {LSystem::TURN, -60}}
		}
	);*/

	/*mLSystem = LSystem( //Unnamed
		"F+F+F+F",
		{
			{'F', "F+F-F+F+F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Unnamed
		"F+F+F+F",
		{
			{'F', "FF+F+F+F+F+F-F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //A bush 1
		"Y",
		{
			{'X', "X[-FFF][+FFF]FX"},
			{'Y', "YFX[+Y][-Y]"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 25}},
			{'-', {LSystem::TURN, -25}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}}
		}
	);*/

	/*mLSystem = LSystem( //A bush 2
		"F",
		{
			{'F', "FF+[+F-F-F]-[-F+F+F]"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 22.5}},
			{'-', {LSystem::TURN, -22.5}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}}
		}
	);*/

	/*mLSystem = LSystem( //A bush 3
		"F",
		{
			{'F', "F[+FF][-FF]F[-F][+F]F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 36}},
			{'-', {LSystem::TURN, -36}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}}
		}
	);*/

	/*mLSystem = LSystem( //A bush 4
		"X",
		{
			{'F', "FF"},
			{'X', "F[+X]F[-X]+X"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 20}},
			{'-', {LSystem::TURN, -20}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}}
		}
	);*/

	/*mLSystem = LSystem( //A bush 5
		"F",
		{
			{'F', "F[+F]F[-F]F"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::TURN, 25}},
			{'-', {LSystem::TURN, -25}},
			{'[', {LSystem::PUSH, 0}},
			{']', {LSystem::POP, 0}}
		}
	);*/

	/*mLSystem = LSystem( //Islands and lakes
		"F+F+F+F",
		{
			{'F', "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF"},
			{'f', "ffffff"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'f', {LSystem::MOVE, 1}},
			{'+', {LSystem::TURN, 90}},
			{'-', {LSystem::TURN, -90}}
		}
	);*/

	/*mLSystem = LSystem( //Islands and lakes
		"A",
		{
			{'A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//"},
			{'B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//"},
			{'C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//"},
			{'D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//"}
		},
		{
			{'F', {LSystem::DRAW, 1}},
			{'+', {LSystem::YAW, 90}},
			{'-', {LSystem::YAW, -90}},
			{'&', {LSystem::PITCH, 90}},
			{'^', {LSystem::PITCH, -90}},
			{'\\', {LSystem::ROLL, 90}},
			{'/', {LSystem::ROLL, -90}},
			{'|', {LSystem::YAW, 180}}
		}
	);*/