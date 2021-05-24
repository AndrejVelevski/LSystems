#include "TestWindow.h"
#include "Utils/Log.h"

TestWindow::TestWindow(uint16 width, uint16 height, const std::string& title) : Window(width, height, title)
{

}

Mesh* generateRectangle(Shader* shader)
{
	std::vector<float>* vertices = new std::vector<float>{
		0,     0, 1, 1, 1, 0, 0,
		3789,  0, 1, 1, 1, 1, 0,
		0,    64, 1, 1, 1, 0, 1,
		3789, 64, 1, 1, 1, 1, 1
	};

	/*std::vector<float>* vertices = new std::vector<float>{
		0,     0, 0, 0,
		44*10,  0, 1, 0,
		0,    46 * 10, 0, 1,
		44 * 10, 46 * 10, 1, 1
	};*/

	std::vector<uint32>* elements = new std::vector<uint32>{
		0, 1, 2,
		1, 3, 2
	};

	Mesh* mesh = new Mesh(shader, vertices, elements);

	mesh->setAttribute2f("aPosition", 7, 0);
	mesh->setAttribute3f("aColor", 7, 2);
	mesh->setAttribute2f("aTexCoords", 7, 5);

	return mesh;
}

void TestWindow::setup()
{
	mCamera = new OrthographicCamera(0, 0, width(), height());
	mTexture = new Texture("res/textures/tiles.png");
	mFont = new Font("res/fonts/VIVALDII.TTF", 64);
	mMesh = generateRectangle(new Shader("res/shaders/default2d.vert", "res/shaders/default2d.frag"));
	{
		std::vector<float>* vertices = new std::vector<float>;
		std::vector<uint32>* elements = new std::vector<uint32>;

		float xpos = 0;
		float ypos = 0;
		int element = 0;

		std::string str = "Hello world!\nThe quick brown fox\njumps over the lazy dog\n0123456789\n!@#$%^&*()\n-_=+\n[]{};':\",./<>?\|`~/|\\";

		for (char c : str)
		{
			if (c == '\n')
			{
				xpos = 0;
				ypos += mFont->fontSize();
				continue;
			}

			glm::ivec2 bearing = mFont->bearing(c);
			glm::ivec2 size = mFont->size(c);
			glm::vec2 tl = mFont->texCoordsTL(c);
			glm::vec2 tr = mFont->texCoordsTR(c);
			glm::vec2 bl = mFont->texCoordsBL(c);
			glm::vec2 br = mFont->texCoordsBR(c);

			float x = xpos + bearing.x;
			float y = ypos + mFont->fontSize() - bearing.y;
			float w = size.x;
			float h = size.y;

			glm::vec3 color;

			color.x = (float)rand() / RAND_MAX;
			color.y = (float)rand() / RAND_MAX;
			color.z = (float)rand() / RAND_MAX;

			vertices->push_back(x);
			vertices->push_back(y);
			vertices->push_back(color.x);
			vertices->push_back(color.y);
			vertices->push_back(color.z);
			vertices->push_back(tl.x);
			vertices->push_back(tl.y);

			color.x = (float)rand() / RAND_MAX;
			color.y = (float)rand() / RAND_MAX;
			color.z = (float)rand() / RAND_MAX;

			vertices->push_back(x+w);
			vertices->push_back(y);
			vertices->push_back(color.x);
			vertices->push_back(color.y);
			vertices->push_back(color.z);
			vertices->push_back(tr.x);
			vertices->push_back(tr.y);

			color.x = (float)rand() / RAND_MAX;
			color.y = (float)rand() / RAND_MAX;
			color.z = (float)rand() / RAND_MAX;

			vertices->push_back(x);
			vertices->push_back(y+h);
			vertices->push_back(color.x);
			vertices->push_back(color.y);
			vertices->push_back(color.z);
			vertices->push_back(bl.x);
			vertices->push_back(bl.y);

			color.x = (float)rand() / RAND_MAX;
			color.y = (float)rand() / RAND_MAX;
			color.z = (float)rand() / RAND_MAX;

			vertices->push_back(x+w);
			vertices->push_back(y+h);
			vertices->push_back(color.x);
			vertices->push_back(color.y);
			vertices->push_back(color.z);
			vertices->push_back(br.x);
			vertices->push_back(br.y);

			elements->push_back(element + 0);
			elements->push_back(element + 1);
			elements->push_back(element + 2);

			elements->push_back(element + 1);
			elements->push_back(element + 3);
			elements->push_back(element + 2);

			xpos += (mFont->advance(c) >> 6);
			element += 4;
		}

		mTextMesh = new Mesh(new Shader("res/shaders/default2d.vert", "res/shaders/default2d.frag"), vertices, elements);
		mTextMesh->setAttribute2f("aPosition", 7, 0);
		mTextMesh->setAttribute3f("aColor", 7, 2);
		mTextMesh->setAttribute2f("aTexCoords", 7, 5);
		mTextMesh->position.x = 100;
		mTextMesh->position.y = 100;
	}

	//mMesh->position.x = -1000;
	//mMesh->position.y = mHeight / 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.3, 0.3, 0.3, 1.0);
}

void TestWindow::update(float delta)
{
	mMesh->position.x -= delta*100;
}

void TestWindow::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	//mTexture->bind();
	mFont->texture()->bind();
	mMesh->setUniform3f("uColor", { 1, 0, 0 });
	mMesh->setUniform1i("uTexture", 0);
	mMesh->draw(mCamera);

	mFont->texture()->bind();
	mTextMesh->setUniform3f("uColor", { 1, 0, 0 });
	mTextMesh->setUniform1i("uTexture", 0);
	mTextMesh->draw(mCamera);
}
