#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Windows.h>

#include <iostream>

using namespace std;

void framebuffer_size_callbac(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.7f, 0.5f);\n"
"}\n\0";

namespace Geometry
{


	class Shape
	{
	protected:
		/*Color color;*/
	public:
		Shape(/*Color color*/)
		{

		}

		virtual ~Shape()
		{

		}

		virtual double get_area()const = 0;
		virtual double get_perimeter()const = 0;
		virtual void draw()const = 0;
		virtual void info()const
		{
			cout << "�������:\t" << get_area() << endl;
			cout << "��������:\t" << get_perimeter();
			draw();
		}
	};

	class Square :public Shape
	{
		double side;
	public:
		double get_side()const
		{
			return side;
		}
		void set_side(double side)
		{
			if (side <= 0.6)
			{
				side = 0.6;
			}
			this->side = side;
		}

		Square(double side)
		{
			set_side(side);
		}

		~Square()
		{

		}

		double get_area()const
		{
			return side * side;
		}
		double get_perimeter()const
		{
			return side * 4;
		}
		void draw()const
		{
			// ������������� glfw
			glfwInit();

			// ������ ������������ glfw
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// ������� ���� ����������
			GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Square", NULL, NULL);

			// �������� ������� ���� ����������
			if (window == NULL)
			{
				cout << "�� ������� ������� ����" << endl;
				return;
			}

			// ������ �������� ���� ��������
			glfwMakeContextCurrent(window);

			// �������� ������� ������� ���� ���������
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callbac);

			// �������� ���������� �� OpenGL �������
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				cout << "�� ������� ���������������� GLAD" << endl;
				return;
			}

			//----------------- �������������� ��������� ���������---------------

			// ��������� ������
			int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			// �������� �� ������� ������ �������������� ���������� �������
			int success;
			char infolog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
				cout << "ERROR::SHADERS::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
			}

			// ����������� ������
			int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			// �������� �� ������� ������ �������������� ������������ �������
			glGetShaderiv(fragmentShader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
				cout << "ERROR::SHADERS::FRAGMENT::COMPOLATION_FAILED\n" << infolog << endl;
			}

			// ���������� ��������
			int shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			// �������� �� ������� ������ �������������� ���������� ��������
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
				cout << "ERROR::SHADER::PROGRAMM::LINKING_FAILED\n" << infolog << endl;
			}

			// �������� ��������
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// ��������� �������
			float vertices[] =
			{
				0.5f, 0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f
				-0.5f, 0.5f, 0.0f
			};

			unsigned int indices[] =
			{
				0, 1, 3,
				1, 2, 3
			};

			// ��������� ��������� ���������
			unsigned int VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			// ��������� ������ ���������� �������
			glBindVertexArray(VAO);

			// ��������� � ������������� ��������� �����
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// �������� ��������� ������ � ���������� �����
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// ������������� ��������� �������
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// ��������� ������� VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// ��������� ������� VAO
			glBindVertexArray(0);

			// ���� ����������
			while (!glfwWindowShouldClose(window))
			{
				// ��������� �����
				processInput(window);

				// ���������� ����������
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// ������ �������
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				/*glBindVertexArray(0);*/

				// ����� ���������� front � back �������
				glfwSwapBuffers(window);

				// ������������ �������
				glfwPollEvents();
			}

			// ������� ��� �������
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);

			// �������� �������� ��������� ��� GLFW
			glfwTerminate();

		}
		void info()const
		{
			cout << typeid(*this).name() << endl;
			Shape::info();
		}
	};
}

int main(void)
{
	setlocale(LC_ALL, "");

	Geometry::Square square(1.0);
	square.info();

	return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������
// � ��������� ������ �������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// �������� ������� ������� ���� ���������
void framebuffer_size_callbac(GLFWwindow* window, int width, int height)
{
	// ���������� ��� ���� ��������� ������������� ����� �������� ����
	glViewport(0, 0, width, height);
}

