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
			cout << "Площадь:\t" << get_area() << endl;
			cout << "Периметр:\t" << get_perimeter();
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
			// Инициализация glfw
			glfwInit();

			// Задаем конфигурацию glfw
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Создаем окно приложения
			GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Square", NULL, NULL);

			// Проверка наличия окна приложения
			if (window == NULL)
			{
				cout << "Не удалось создать окно" << endl;
				return;
			}

			// Делаем контекст окна основным
			glfwMakeContextCurrent(window);

			// Корекция размера области окна просмотра
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callbac);

			// Загрузка указателей на OpenGL функции
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				cout << "Не удалось инициализировать GLAD" << endl;
				return;
			}

			//----------------- Компилирование шейдерной программы---------------

			// Вершинный шейдер
			int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);

			// Проверка на наличие ошибок компилирования вершинного шейдера
			int success;
			char infolog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
				cout << "ERROR::SHADERS::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
			}

			// Фрагментный шейдер
			int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			// Проверка на наличие ошибок компилиравания фрагментного шейдера
			glGetShaderiv(fragmentShader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
				cout << "ERROR::SHADERS::FRAGMENT::COMPOLATION_FAILED\n" << infolog << endl;
			}

			// Связывание шейдеров
			int shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			// Проверка на наличие ошибок компилирования связывания шейдеров
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
				cout << "ERROR::SHADER::PROGRAMM::LINKING_FAILED\n" << infolog << endl;
			}

			// Удаление шейдеров
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Указываем вершины
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

			// Настройка вершинных атрибутов
			unsigned int VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			// Связываем объект вершинного массива
			glBindVertexArray(VAO);

			// Связываем и устанавливаем вершинный буфер
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// Копируем индексный массив в элементный буфер
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// Конфигурируем вершинный атрибут
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Выполняем отвязку VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Выполняем отвязку VAO
			glBindVertexArray(0);

			// Цикл рендеринга
			while (!glfwWindowShouldClose(window))
			{
				// Обработка ввода
				processInput(window);

				// Выполнение рендеринга
				glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// Рисуем квадрат
				glUseProgram(shaderProgram);
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
				/*glBindVertexArray(0);*/

				// Обмен содержимым front и back буферов
				glfwSwapBuffers(window);

				// Отслеживание событий
				glfwPollEvents();
			}

			// Удаляем все ресурсы
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);

			// Удаление ресурсов выделеных для GLFW
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

// Обработка всех событий ввода: запрос GLFW о нажатии/отпускании клавиш
// и обработка данных событий
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Корекция размера области окна просмотра
void framebuffer_size_callbac(GLFWwindow* window, int width, int height)
{
	// Убеждаемся что окно просмотра соответствует новым размерам окна
	glViewport(0, 0, width, height);
}

