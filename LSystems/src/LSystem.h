#pragma once

#include "Utils/Types.h"

#include <string>
#include <vector>
#include <unordered_map>

class LSystem
{
public:
	enum Instruction {
		DRAW,
		MOVE,
		TURN,
		ROTATEZ, YAW,
		ROTATEY, PITCH,
		ROTATEX, ROLL,
		PUSH,
		POP
	};

public:
	LSystem();
	LSystem(const std::string& axiom,
		    const std::unordered_map<char, std::string>& rules,
		    const std::unordered_map<char, std::pair<Instruction, float>>& instructions);

	void generate(int generation, std::vector<float>* vertices, std::vector<uint32>* elements);

private:
	void mfGenerate(int generation);

	std::string mAxiom;
	std::unordered_map<char, std::string> mRules;
	std::unordered_map<char, std::pair<Instruction, float>> mInstructions;
	std::vector<std::string> mGenerations;
};

