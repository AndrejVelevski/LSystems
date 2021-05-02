#include "TestWindow.h"
#include "Utils/Log.h"

TestWindow::TestWindow(uint16 width, uint16 height, const std::string& title) : Window(width, height, title)
{

}

void TestWindow::setup()
{
	mCamera = new OrthographicCamera(0, mWidth, 0, mHeight);
	Shader* shader = new Shader("res/shaders/default2d.vs", "res/shaders/default2d.fs");
	mMesh = generateDavidStar(shader);
	mMesh->position.x = mWidth / 2;
	mMesh->position.y = mHeight / 2;

	mMesh->setAttribute2f("aPosition", 2, 0);

	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void TestWindow::update(float delta)
{
}

void TestWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	mMesh->draw(mCamera, Mesh::LINE_LOOP);
}

Mesh* TestWindow::generateDavidStar(Shader* shader)
{
	std::vector<float>* vertices = new std::vector<float>;

	double outerRadius = 200;
	double innerRadius = outerRadius / (sqrt(15)/3);
	//double innerRadius = 154;
	double rotation = -90;
	int points = 16;

	for (int i = 0; i <points; i += 2)
	{
		double outerAngle1 = (i * 360) / points + rotation;
		double innerAngle = ((i + 1) * 360) / points + rotation;
		double outerAngle2 = ((i + 2) * 360) / points + rotation;

		double x1 = outerRadius * cos(glm::radians(outerAngle1));
		double y1 = outerRadius * sin(glm::radians(outerAngle1));
		double x2 = innerRadius * cos(glm::radians(innerAngle));
		double y2 = innerRadius * sin(glm::radians(innerAngle));
		double x3 = outerRadius * cos(glm::radians(outerAngle2));
		double y3 = outerRadius * sin(glm::radians(outerAngle2));

		vertices->push_back(x1);
		vertices->push_back(y1);

		vertices->push_back(x2);
		vertices->push_back(y2);

		vertices->push_back(x2);
		vertices->push_back(y2);

		vertices->push_back(x3);
		vertices->push_back(y3);
	}	

	return new Mesh(shader, vertices, nullptr);
}
