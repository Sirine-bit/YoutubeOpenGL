#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include "style.hpp"
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <string>
#include <io.h>


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(size * aPos.x, size * aPos.y, size * aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";



int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Get primary monitor
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	// Get monitor resolution
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	// Create window with monitor's resolution
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Sup'Engine", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0,800, 800);



	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//change the fonts of login form
	io.Fonts->AddFontDefault();
	ImFont* mainfont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.5f);
	if (mainfont) {
		io.Fonts->Build();
	}
	else {
		std::cerr << "Erreur: Impossible de charger la police !" << std::endl;
	}
	


	// Apply the style
	SetImGuiStyle();
	
	// Variables to be changed in the ImGUI window
	bool logged = false;
	const int MAX_INPUT_LENGTH = 256;
	char inputUsername[MAX_INPUT_LENGTH] = "";
	char inputPassword[MAX_INPUT_LENGTH] = "";
	std::string username = "test";
	std::string password = "123456";
	
	
	
	bool drawTriangle = true;
	float size = 1.0f;
	//int x = 0;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	//bool show_demo_window = false;
	//bool show_another_window = false;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//MY project variables !
	bool paused = false;
	float fps = 112.125f;
	const char* clothModes[] = { "Pin Upper Corner", "Pin Upper Edge" };
	int clothMode = 0;
	const char* drawModes[] = { "Draw Particles", "Draw Strings", "Draw Cloth" };
	int drawMode = 2;
	float hookCoefficient = 100.0f;
	float shearingCoefficient = 50.0f;
	float bendingCoefficient = 30.0f;
	float damping = 12.0f;
	float friction = 8.0f;
	float groundFriction = 5.0f;
	float sphereFriction = 3.0f;
	float windVelocity = 0.0f;
	ImVec4 lightColor = ImVec4(0.702f, 0.498f, 0.333f,1.0f);
	const char* clothTextures[] = { "The Starry Night", "Allegory of Sight", "Rubens Wallpaper" };
	int clothTextureIndex = 0;
	const char* groundTextures[] = { "Witcher 3 Wallpaper", "God of War Ragnarok Wallpaper", "Genshin Impact Wallpaper" };
	int groundTextureIndex = 0;
	const char* sphereTextures[] = { "Witcher 3 Wallpaper", "God of War Ragnarok Wallpaper", "Genshin Impact Wallpaper" };
	int sphereTextureIndex = 0;
	const char* windTextures[] = { "Witcher 3 Wallpaper", "God of War Ragnarok Wallpaper", "Genshin Impact Wallpaper" };
	int windTextureIndex = 0;
	bool mouseGrabActive = false;

	// Exporting variables to shaders
	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);
	//glUniform1f(glGetUniformLocation(shaderProgram, "sizeDec"), x);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		// Only draw the triangle if the ImGUI checkbox is ticked
		if (drawTriangle)
			// Draw the triangle using the GL_TRIANGLES primitive
			glDrawArrays(GL_TRIANGLES, 0, 3);

		//// ImGUI window creation (first window)
		//ImGui::Begin("esm el window");
		//// Text that appears in the window
		//ImGui::Text("Hello");
		//// Checkbox that appears in the window
		//ImGui::Checkbox("Draw Triangle", &drawTriangle);
		//// Slider that appears in the window
		//ImGui::SliderFloat("Size", &size, -3.0f, 2.0);
		//std::cout << size << std::endl;
		//// int Slider that appears in the window
		//ImGui::SliderInt("Size", &x, -5, 2);
		//// Fancy color editor that appears in the window
		//ImGui::ColorEdit4("Color", color);
		//// Ends the window
		//ImGui::End();
		////(second window)
		//ImGui::Begin("haha");
		//// Text that appears in the window
		//ImGui::Text("Hello");
		//// Checkbox that appears in the window
		//ImGui::Checkbox("Draw Triangle", &drawTriangle);
		//// Slider that appears in the window
		//ImGui::SliderFloat("Size", &size, -3.0f, 2.0);
		//// int Slider that appears in the window
		//ImGui::SliderInt("SizeDec", &x, -5, 2);
		//// Fancy color editor that appears in the window
		//ImGui::ColorEdit4("Color", color);
		//// Ends the window
		//ImGui::End();
		//(third window)
		/*if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			static float f = 0.0f;
			static int counter = 0;

			//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);

			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			//	counter++;
			//ImGui::SameLine();
			//ImGui::Text("counter = %d", counter);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			//ImGui::End();
		}	

		// Export variables to shader
		glUseProgram(shaderProgram);
		glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);
		glUniform1f(glGetUniformLocation(shaderProgram, "sizeDec"), x);	 */


		
		ImGui::SetNextWindowSize(ImVec2(500, 500));
		if (!logged && ImGui::Begin("Login", nullptr, ImGuiWindowFlags_NoCollapse)) {
			ImGui::PushFont(mainfont);
			ImGui::Text("Input Username:");
			ImGui::InputText("##usernameinput", inputUsername, MAX_INPUT_LENGTH);

			ImGui::Text("Input Password:");
			ImGui::InputText("##passwordinput", inputPassword, MAX_INPUT_LENGTH,
				ImGuiInputTextFlags_Password);
			
			ImGui::SetCursorPos(ImVec2(150, 150));
			if (ImGui::Button("Login", ImVec2(80, 25))) {
				if (strcmp(inputUsername, username.c_str()) == 0 &&
					strcmp(inputPassword, password.c_str()) == 0) {
					logged = true;
				}
			}
			ImGui::PopFont();
			ImGui::End();
		}




		//My project window !
		if (logged) {
			ImVec2 topLeftCorner(0.0f, 0.0f); // Position at (0, 0)
			ImGui::SetNextWindowPos(topLeftCorner, ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(500, 900));
			ImGui::Begin("Cloth Simulation Control Panel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

			// Simulation
			ImGui::Text("Simulate with %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			ImGui::Checkbox("Pause", &paused);

			// Cloth
			ImGui::Text("Cloth");
			ImGui::Combo("Mode", &clothMode, clothModes, IM_ARRAYSIZE(clothModes));
			ImGui::Combo("Draw Mode", &drawMode, drawModes, IM_ARRAYSIZE(drawModes));
			ImGui::SliderFloat("Hook Coefficient", &hookCoefficient, 0.0f, 200.0f);
			ImGui::SliderFloat("Shearing Coefficient", &shearingCoefficient, 0.0f, 100.0f);
			ImGui::SliderFloat("Bending Coefficient", &bendingCoefficient, 0.0f, 60.0f);
			ImGui::DragFloat("Damping", &damping, 0.1f, 0.0f, 20.0f);
			ImGui::DragFloat("Friction", &friction, 0.1f, 0.0f, 20.0f);
			ImGui::DragFloat("Ground Friction", &groundFriction, 0.1f, 0.0f, 10.0f);
			ImGui::DragFloat("Sphere Friction", &sphereFriction, 0.1f, 0.0f, 10.0f);
			ImGui::Combo("Cloth Texture", &clothTextureIndex, clothTextures, IM_ARRAYSIZE(clothTextures));
			ImGui::Combo("Ground Texture", &groundTextureIndex, groundTextures, IM_ARRAYSIZE(groundTextures));
			ImGui::Combo("Sphere Texture", &sphereTextureIndex, sphereTextures, IM_ARRAYSIZE(sphereTextures));

			// Wind
			ImGui::Text("Wind");
			ImGui::DragFloat("Velocity", &windVelocity, 0.1f, 0.0f, 10.0f);
			ImGui::Combo("Wind Texture", &windTextureIndex, windTextures, IM_ARRAYSIZE(windTextures));
			ImGui::SetCursorPos(ImVec2(200, 430));
			if (ImGui::Button("Stop Wind", ImVec2(80, 25))) {
				windVelocity = 0.0f;
			}

			// Light
			ImGui::Text("Light");
			ImGui::ColorEdit3("Color", (float*)&lightColor ,ImGuiColorEditFlags_NoSmallPreview| ImGuiColorEditFlags_NoLabel);

			// Cloth Rotation
			ImGui::Text("Cloth Rotation");
			if (ImGui::Button("Rotate Clockwise")) {
				// Rotate cloth clockwise
			}
			if (ImGui::Button("Rotate Counter-Clockwise")) {
				// Rotate cloth counter-clockwise
			}

			// Mouse Grab
			ImGui::Text("Mouse Grab");
			ImGui::Checkbox("Activate Grab Mode", &mouseGrabActive);
			if (mouseGrabActive) {
				// Handle mouse grab functionality
			}

			ImGui::End();
		}
		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
