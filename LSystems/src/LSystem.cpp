#include "LSystem.h"

#include <stack>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Transform.h"
#include "Renderer/Mesh.h"
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

	glm::vec3 color(1, 1, 1);
	Transform transform;
	std::stack<Transform> stack;
	std::stack<float> stackScale;
	float scale = 1;
	float scaleModifier = 0.7;

	uint32 currentMaxElement = 0;

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
				glm::mat4 model = transform.getModel();
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

				std::vector<glm::vec3> points;
				std::vector<uint32> indices;
				Mesh::generateCylinder(0.1*scale, 0.1*scale*scaleModifier, instruction.second*scale, points, indices, 10);

				for (glm::vec3& p : points)
				{
					glm::vec4 result = model * rotationMatrix * glm::vec4(p.x, p.y, p.z, 1);
					vertices->push_back(result.x);
					vertices->push_back(result.y);
					vertices->push_back(result.z);
					vertices->push_back(color.r);
					vertices->push_back(color.g);
					vertices->push_back(color.b);
				}

				uint32 startElement = currentMaxElement;
				for (uint32 i : indices)
				{
					uint32 newElement = startElement + i;
					elements->push_back(newElement);

					if (newElement >= currentMaxElement)
						currentMaxElement = newElement+1;
				}

				transform.position += transform.front() * instruction.second*scale;
				scale *= scaleModifier;

				break;
			}
			case Instruction::MOVE:
				transform.getModel();
				transform.position += transform.front() * instruction.second;
				scale *= scaleModifier;
				break;
			case Instruction::ROLL:
			case Instruction::ROTATEZ:
				transform.rotation.z += instruction.second;
				break;
			case Instruction::YAW:
			case Instruction::ROTATEY:
				transform.rotation.y += instruction.second;
				break;
			case Instruction::PITCH:
			case Instruction::ROTATEX:
				transform.rotation.x += instruction.second;
				break;
			case Instruction::PUSH:
				stack.push(transform);
				stackScale.push(scale);
				break;
			case Instruction::POP:
				transform = stack.top();
				stack.pop();
				scale = stackScale.top();
				stackScale.pop();
				break;
			case Instruction::SCALE:
				transform.scale *= instruction.second;
				break;
			default:
				break;
			}
		}
	}
}



