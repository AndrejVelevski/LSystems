#pragma once

#include "Utils/Types.h"
#include "Renderer/Mesh.h"

#include <string>
#include <vector>
#include <unordered_map>

class LSystem
{
public:
	enum Instruction {
		DRAW,
		MOVE,
		ROTATEZ, ROLL,
		ROTATEY, YAW,
		ROTATEX, PITCH,
		PUSH,
		POP,
	};

public:
	LSystem();
	LSystem(const std::string& axiom,
		    const std::unordered_map<char, std::string>& rules,
		    const std::unordered_map<char, std::pair<Instruction, float>>& instructions);

	Mesh* generate(int generation, float lineThickness, float lineThicknessModifier, float lineLengthModifier, float pruneChance, float mutationChance, float mutationFactor, bool lines);

private:
	void mfGenerate(int generation);

	std::string mAxiom;
	std::unordered_map<char, std::string> mRules;
	std::unordered_map<char, std::pair<Instruction, float>> mInstructions;
	std::vector<std::string> mGenerations;
};

