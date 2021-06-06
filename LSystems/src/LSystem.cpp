#include "LSystem.h"

#include <stack>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Transform.h"
#include "Utils/Log.h"

float random()
{
	return (float)rand() / (float)RAND_MAX;
}

float mutate(float mutationChance, float mutationFactor)
{
	if (random() < mutationChance)
	{
		return (1 - (random() * mutationFactor - 0.5 * mutationFactor));
	}
	return 1;
}

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

Mesh* LSystem::generate(int generation, float lineThickness, float lineThicknessModifier, float lineLengthModifier, float pruneChance, float mutationChance, float mutationFactor, bool lines)
{
	srand(time(nullptr));
	mfGenerate(generation);

	std::string& string = mGenerations[generation];

	std::vector<float>* vertices = new std::vector<float>;
	std::vector<uint32>* elements = new std::vector<uint32>;

	struct State {
		Transform transform;
		float lineLength;
		float lineThickness;
		bool prune;
	};

	glm::vec3 color(0.5, 0.375, 0.25);
	Transform transform;
	std::stack<State> stack;
	float lineLength = 1;
	bool prune = false;

	uint32 currentMaxElement = 0;

	bool first = true;

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
				if (!first)
				{
					if (prune)
						break;
					prune = random() < pruneChance;
					if (prune)
						break;
				}
				first = false;

				glm::mat4 model = transform.getModel();
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));

				std::vector<glm::vec3> points;
				std::vector<uint32> indices;
				if (lines)
				{
					points.push_back({ 0, 0, 0 });
					points.push_back({ 0, instruction.second * lineLength, 0 });
					indices.push_back(0);
					indices.push_back(1);
				}
				else
				{
					Mesh::generateCylinder(lineThickness, lineThickness * lineThicknessModifier, instruction.second * lineLength, points, indices, 10);
				}

				for (glm::vec3& p : points)
				{
					glm::vec4 result = model * rotationMatrix * glm::vec4(p.x, p.y, p.z, 1);
					vertices->push_back(result.x);
					vertices->push_back(result.y);
					vertices->push_back(result.z);
					vertices->push_back(0);
					vertices->push_back(0);
					vertices->push_back(0);
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

				transform.position += transform.front() * instruction.second * lineLength;
					lineLength *= lineLengthModifier * (first ? 1 : mutate(mutationChance, mutationFactor));
				lineThickness *= lineThicknessModifier;
				break;
			}
			case Instruction::MOVE:
				if (prune)
					break;
				prune = random() < pruneChance;
				if (prune)
					break;
				transform.getModel();
				transform.position += transform.front() * instruction.second * lineLength;
				lineLength *= lineLengthModifier * (first ? 1 : mutate(mutationChance, mutationFactor));
				lineThickness *= lineThicknessModifier;
				break;
			case Instruction::ROLL:
			case Instruction::ROTATEZ:
				if (prune)
					break;
				transform.rotation.z += instruction.second * (first ? 1 : mutate(mutationChance, mutationFactor));
				break;
			case Instruction::YAW:
			case Instruction::ROTATEY:
				if (prune)
					break;
				transform.rotation.y += instruction.second * (first ? 1 : mutate(mutationChance, mutationFactor));
				break;
			case Instruction::PITCH:
			case Instruction::ROTATEX:
				if (prune)
					break;
				transform.rotation.x += instruction.second * (first ? 1 : mutate(mutationChance, mutationFactor));
				break;
			case Instruction::PUSH:
				stack.push({ transform, lineLength, lineThickness, prune });
				break;
			case Instruction::POP:
			{
				if (!stack.empty())
				{
					State state = stack.top();
					stack.pop();
					transform = state.transform;
					lineLength = state.lineLength;
					lineThickness = state.lineThickness;
					prune = state.prune;
				}
				
				break;
			}
			default:
				break;
			}
		}
	}

	if (!lines)
	{
		for (uint32 i = 0; i < elements->size(); i += 3)
		{
			uint32 e1 = elements->at(i + 0);
			uint32 e2 = elements->at(i + 1);
			uint32 e3 = elements->at(i + 2);

			uint32 vi1 = 9 * e1;
			uint32 vi2 = 9 * e2;
			uint32 vi3 = 9 * e3;

			glm::vec3 p1 = { vertices->at(vi1 + 0u), vertices->at(vi1 + 1), vertices->at(vi1 + 2) };
			glm::vec3 p2 = { vertices->at(vi2 + 0u), vertices->at(vi2 + 1), vertices->at(vi2 + 2) };
			glm::vec3 p3 = { vertices->at(vi3 + 0u), vertices->at(vi3 + 1), vertices->at(vi3 + 2) };

			glm::vec3 edge1 = p1 - p2;
			glm::vec3 edge2 = p1 - p3;

			glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

			vertices->at(vi1 + 3) = normal.x;
			vertices->at(vi1 + 4) = normal.y;
			vertices->at(vi1 + 5) = normal.z;

			vertices->at(vi2 + 3) = normal.x;
			vertices->at(vi2 + 4) = normal.y;
			vertices->at(vi2 + 5) = normal.z;

			vertices->at(vi3 + 3) = normal.x;
			vertices->at(vi3 + 4) = normal.y;
			vertices->at(vi3 + 5) = normal.z;
		}
	}
	
	Mesh* mesh = new Mesh(new Shader("res/shaders/phong.vert", "res/shaders/phong.frag"), vertices, elements);
	mesh->setAttribute3f("aPosition", 9, 0);
	mesh->setAttribute3f("aNormal", 9, 3);
	mesh->setAttribute3f("aColor", 9, 6);

	return mesh;
}



