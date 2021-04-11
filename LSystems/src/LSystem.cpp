#include "LSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>
#include <sstream>

#include "Utils/Log.h"

LSystem::LSystem()
{

}

LSystem::LSystem(const std::string& axiom,
				 const std::unordered_map<char, std::string>& rules,
			     const std::unordered_map<char, std::pair<Instruction, float>>& instructions)
{
	mAxiom = axiom;
	mRules = rules;
	mInstructions = instructions;
	mGenerations.push_back(axiom);
}

void LSystem::mfGenerate(int generation)
{
	while (mGenerations.size() <= generation)
	{
		int i = mGenerations.size() - 1;
		std::string& current = mGenerations[i];

		std::stringstream ss;

		for (char c : current)
		{
			bool foundRule = false;

			for (auto it = mRules.begin(); it != mRules.end(); ++it)
			{
				if (it->first == c)
				{
					ss << it->second;
					foundRule = true;
					break;
				}
			}

			if (!foundRule)
				ss << c;
		}

		mGenerations.push_back(ss.str());
	}
}

void LSystem::generate(int generation, std::vector<float>* vertices, std::vector<uint32>* elements)
{
	mfGenerate(generation);

	std::string& string = mGenerations[generation];

	glm::vec3 pos(0, 0, 0);
	glm::vec3 col(1, 1, 1);

	glm::vec3 dir(0, 0, 0);
	std::stack<glm::vec3> posStack;
	std::stack<glm::vec3> dirStack;

	int currentindex = 0;

	for (char c : string)
	{
		std::pair<Instruction, float> instruction;
		bool foundInstruction = false;

		for (auto it = mInstructions.begin(); it != mInstructions.end(); ++it)
		{
			if (it->first == c)
			{
				instruction = it->second;
				foundInstruction = true;
				break;
			}
		}

		if (foundInstruction)
		{
			switch (instruction.first)
			{
			case Instruction::DRAW:
			{
				vertices->push_back(pos.x);
				vertices->push_back(pos.y);
				vertices->push_back(pos.z);

				vertices->push_back(col.r);
				vertices->push_back(col.g);
				vertices->push_back(col.b);

				elements->push_back(currentindex++);

				//pos.x += cos(glm::radians(dir));
				//pos.y += sin(glm::radians(dir));

				/*U.x = (U.x * cos(yaw) + U.y * sin(yaw));
				U.y = (U.y * cos(yaw) - U.x * sin(yaw));

				L.x = (L.x * cos(pitch) - L.z * sin(pitch));
				L.z = (L.x * sin(pitch) + L.z * cos(pitch));

				H.y = (H.y * cos(roll) - H.z * sin(roll));
				H.z = (H.y * sin(roll) + H.z * cos(roll));

				glm::dvec3 sum = U * L * H;
				pos.x += sum.x;
				pos.y += sum.y;
				pos.z += sum.z;*/


				pos.x += cos(glm::radians(dir.z)) * cos(glm::radians(dir.x));
				pos.y += sin(glm::radians(dir.x));
				pos.z += sin(glm::radians(dir.z)) * cos(glm::radians(dir.x));


				vertices->push_back(pos.x);
				vertices->push_back(pos.y);
				vertices->push_back(pos.z);

				vertices->push_back(col.r);
				vertices->push_back(col.g);
				vertices->push_back(col.b);

				elements->push_back(currentindex++);
				break;
			}
			case Instruction::MOVE:
				break;
			case Instruction::YAW:
			case Instruction::ROTATEZ:
				dir.z += instruction.second;
				break;
			case Instruction::ROLL:
			case Instruction::ROTATEY:
				dir.y += instruction.second;
				break;
			case Instruction::PITCH:
			case Instruction::ROTATEX:
				dir.x += instruction.second;
				break;
			case Instruction::TURN:
				dir += instruction.second;
				break;
			case Instruction::PUSH:
				posStack.push(pos);
				dirStack.push(dir);
				break;
			case Instruction::POP:
				pos = posStack.top();
				dir = dirStack.top();
				posStack.pop();
				dirStack.pop();
				break;
			default:
				break;
			}
		}
	}
}



