// sb6.h 헤더 파일을 포함시킨다.
#include <sb7.h>
#include <vmath.h>
#include <shader.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> // rand()함수 포함 라이브러리
#include <time.h> // time()함수 포함 라이브러리

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"
#include "NormalModel.h"

// sb6::application을 상속받는다.
class my_application : public sb7::application
{
public:
	// 쉐이더 컴파일한다.
	// basic multiple lighting shader
	GLuint compile_shader(void)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader = sb7::shader::load("multiple_lights.vs.glsl", GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader = sb7::shader::load("multiple_lights.fs.glsl", GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shader_normal(void)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader = sb7::shader::load("multiple_lights_normal.vs.glsl", GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader = sb7::shader::load("multiple_lights_normal.fs.glsl", GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shader_dimension(void)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader = sb7::shader::load("dimension.vs.glsl", GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader = sb7::shader::load("dimension.fs.glsl", GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	GLuint compile_shader_screen(void)
	{
		// 버텍스 쉐이더를 생성하고 컴파일한다.
		GLuint vertex_shader = sb7::shader::load("screen_filtering.vs.glsl", GL_VERTEX_SHADER);

		// 프래그먼트 쉐이더를 생성하고 컴파일한다.
		GLuint fragment_shader = sb7::shader::load("screen_filtering.fs.glsl", GL_FRAGMENT_SHADER);

		// 프로그램을 생성하고 쉐이더를 Attach시키고 링크한다.
		GLuint program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// 이제 프로그램이 쉐이더를 소유하므로 쉐이더를 삭제한다.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	// 애플리케이션 초기화 수행한다.
	virtual void startup()
	{
		srand(time(NULL)); // 매번 다른 시드값 생성

		// 쉐이더 프로그램 컴파일 및 연결
		shader_program = compile_shader();
		shader_program_screen = compile_shader_screen();
		shader_program_normal = compile_shader_normal();
		shader_program_dimension = compile_shader_dimension();

		stbi_set_flip_vertically_on_load(true);

		// 첫 번째 객체 정의 : 박스 --------------------------------------------------
		// 박스 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		float box_s = 1.0f, box_t = 1.0f;
		GLfloat box_positions[] = {
			// 뒷면
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, -0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, -0.25f,
			// 우측면
			0.25f, 0.0f, -0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.0f, 0.25f,

			0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,
			// 정면
			0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, 0.25f,

			-0.25f, 0.0f, 0.25f,
			0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			// 좌측면
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.0f, -0.25f,

			-0.25f, 0.0f, -0.25f,
			-0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			// 바닥면
			-0.25f, 0.0f, 0.25f,
			0.25f, 0.0f, -0.25f,
			0.25f, 0.0f, 0.25f,

			0.25f, 0.0f, -0.25f,
			-0.25f, 0.0f, 0.25f,
			-0.25f, 0.0f, -0.25f,
			// 윗면
			-0.25f, 0.5f, -0.25f,
			0.25f, 0.5f, 0.25f,
			0.25f, 0.5f, -0.25f,

			0.25f, 0.5f, 0.25f,
			-0.25f, 0.5f, -0.25f,
			-0.25f, 0.5f, 0.25f
		};

		GLfloat box_texCoords[] = {
			box_s, box_t,
			0.0f, 0.0f,
			box_s, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			box_s, box_t,
			0.0f, 0.0f,

			0.0f, 0.0f,
			box_s, box_t,
			0.0f, box_t,

			box_s, 0.0f,
			0.0f, box_t,
			0.0f, 0.0f,

			0.0f, box_t,
			box_s, 0.0,
			box_s, box_t,

			0.0f, box_t,
			box_s, 0.0f,
			box_s, box_t,

			box_s, 0.0f,
			0.0f, box_t,
			0.0f, 0.0f
		};

		GLfloat box_normals[] = {
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,

			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		boxModel.init();
		boxModel.setupMesh(36, box_positions, box_texCoords, box_normals);
		boxModel.loadDiffuseMap("container2.png");
		boxModel.loadSpecularMap("container2_specular.png");
		boxModel.loadNormalMap("container2_normal.png");

		boxPositions.push_back(vmath::vec3(-4.5f, 0.5f, -4.5f));
		boxPositions.push_back(vmath::vec3(4.5f, 0.5f, -4.5f));
		boxPositions.push_back(vmath::vec3(4.5f, 0.0f, -4.5f));
		boxPositions.push_back(vmath::vec3(4.5f, -0.5f, -4.5f));

		groundModel.init();
		groundModel.setupMesh(36, box_positions, box_texCoords, box_normals);
		groundModel.loadDiffuseMap("container2.png");
		groundModel.loadSpecularMap("container2_specular.png");
		//groundModel.loadNormalMap("container2_normal.png");

		groundPositions.push_back(vmath::vec3(0.0f, 1.0f, 0.0f)); // 땅
		groundScales.push_back(vmath::vec3(30.0f, 1.0f, 30.0f));
		groundPositions.push_back(vmath::vec3(0.0f, -1.5f, 0.0f)); // 천장
		groundScales.push_back(vmath::vec3(30.0f, 1.0f, 30.0f));
		groundPositions.push_back(vmath::vec3(0.0f, -10.0f, 5.0f)); // 뒤
		groundScales.push_back(vmath::vec3(30.0f, 30.0f, 1.0f));
		groundPositions.push_back(vmath::vec3(0.0f, -10.0f, -5.0f)); // 앞
		groundScales.push_back(vmath::vec3(30.0f, 30.0f, 1.0f));
		groundPositions.push_back(vmath::vec3(5.0f, -10.0f, 0.0f)); // 왼쪽
		groundScales.push_back(vmath::vec3(1.0f, 30.0f, 30.0f));
		groundPositions.push_back(vmath::vec3(-5.0f, -10.0f, 0.0f)); // 오른쪽
		groundScales.push_back(vmath::vec3(1.0f, 30.0f, 30.0f));

		pointLightPos.push_back(vmath::vec3(0.0f, -0.55f, 0.0f));
		pointLightPos.push_back(vmath::vec3(100.25f, (float)sin(0 * 1.f), (float)cos(0 * 1.f) * 0.7f));

		lampModel.init();
		lampModel.loadOBJ("modern_ceiling_lamp.obj");
		lampModel.loadDiffuseMap("modern_ceiling_lamp.jpg");
		lampPositions.push_back(vmath::vec3(0.0f, -0.0f, 0.0f));

		horseModel.init();
		horseModel.loadOBJ("horse.obj");
		horseModel.setDefaultColor(vmath::vec3(94/255.f, 193/255.f, 196/255.f));
		//horseModel.loadDiffuseMap("horse.jpg");

		for (int i = 0; i < 10; i++) {
			float random1 = (rand() * 2 - 32767) / 10000.f;
			float random2 = (rand() * 2 - 32767) / 10000.f;

			if (random1 < -1 || random1 > 1 && random2 < -1 || random2 > 1) 
			horsePositions.push_back(vmath::vec3(random1, 1.0f, random2));
		}

		tableModel.init();
		tableModel.loadOBJ("table.obj");
		tableModel.loadDiffuseMap("table.jpg");
		tablePositions.push_back(vmath::vec3(0.0f, 1.0f, 0.0f));

		marbleModel.init();
		marbleModel.loadOBJ("marble.obj");
		marbleModel.loadDiffuseMap("marble.jpg");
		marblePositions.push_back(vmath::vec3(0.0f, 1.0f, -3.75f));

		signModel.init();
		signModel.loadOBJ("sign.obj");
		signModel.loadDiffuseMap("sign.jpg");
		signPositions.push_back(vmath::vec3(0.0f, 1.0f, -3.0f));

		bedModel.init();
		bedModel.loadOBJ("bed.obj");
		bedModel.loadDiffuseMap("bed.jpg");
		bedPositions.push_back(vmath::vec3(-4.5f, 0.5f, -4.5f));

		appleModel.init();
		appleModel.loadOBJ("apple.obj");
		appleModel.loadDiffuseMap("apple.jpg");
		applePositions.push_back(vmath::vec3(0.0f, 0.5f, 0.0f));
		//applePositions.push_back(vmath::vec3(0.1f, 0.5f, 0.0f));

		// 두 번째 객체 정의 : 피라미드 --------------------------------------------------
		// 피라미드 점들의 위치와 컬러, 텍스처 좌표를 정의한다.
		GLfloat pyramid_vertices[] = {
			1.0f, 0.0f, -1.0f,    // 우측 상단
			-1.0f, 0.0f, -1.0f,   // 좌측 상단
			-1.0f, 0.0f, 1.0f,    // 좌측 하단
			1.0f, 0.0f, 1.0f,     // 우측 하단
			0.0f, 1.0f, 0.0f,      // 상단 꼭지점
			0.0f, -1.0f, 0.0f,      // 하단 꼭지점
		};

		// 삼각형으로 그릴 인덱스를 정의한다.
		GLuint pyramid_indices[] = {
			4, 0, 1,
			4, 1, 2,
			4, 2, 3,
			4, 3, 0,

			5, 1, 0,
			5, 2, 1,
			5, 3, 2,
			5, 0, 3,
		};

		pyramidModel.init();
		pyramidModel.setupMesh(6, pyramid_vertices);
		pyramidModel.setupIndices(24, pyramid_indices);
		pyramidModel.setDefaultColor(vmath::vec3(1, 1, 1));

		//  Post-processing용 스크린 정의 -------------------------------------------------- (49:53)
		GLfloat screenVertices[] = {
			//NDC xy좌표 //텍스처 UV
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		glGenVertexArrays(1, &VAO_screen);
		glGenBuffers(1, &VBO_screen);
		glBindVertexArray(VAO_screen);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_screen);

		glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// framebuffer 설정 -------------------------------------------------- (30:53)
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// color buffer 텍스처 생성 및 연결
		glGenTextures(1, &FBO_texture);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.windowWidth, info.windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);	// 실제 우리가 전달하는 데이터는 없으니까 NULL
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBO_texture, 0); // 프레임버퍼에 연결

		// depth&stencil buffer를 위한 Render Buffer Object 생성 및 연결
		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, info.windowWidth, info.windowHeight); // Depth 24bit, Stencil 8bit
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		// Framebuffer가 정상적으로 만들어 졌는지 확인 (36:42)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			glfwTerminate(); // 프로그램 종료
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // 언바인드
		glfwSetCursorPos(window, info.windowWidth / 2, info.windowHeight / 2);

		glEnable(GL_MULTISAMPLE);
	}

	// 애플리케이션 끝날 때 호출된다.
	virtual void shutdown()
	{
		glDeleteProgram(shader_program);
		glDeleteProgram(shader_program_screen);
		glDeleteProgram(shader_program_normal);
		glDeleteProgram(shader_program_dimension);
	}

	// 렌더링 virtual 함수를 작성해서 오버라이딩한다.
	virtual void render(double currentTime)
	{
		double deltaTime = currentTime - lastTime;
		const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



		// 카메라 매트릭스 계산
		const float cameraSpeed = 0.05f; // adjust accordingly

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		horizontalAngle -= cameraSpeed * deltaTime * float(info.windowWidth / 2 - xpos);
		verticalAngle += cameraSpeed * deltaTime * float(info.windowHeight / 2 - ypos);

		vmath::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		vmath::vec3 right = vmath::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		cameraUp = vmath::cross(right, direction);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += direction * deltaTime * cameraSpeed * 50;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= direction * deltaTime * cameraSpeed * 50;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraUp;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= right * deltaTime * cameraSpeed * 50;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += right * deltaTime * cameraSpeed * 50;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			isPressF = true;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
			if (isPressF) {
				toggleSpotLight = !toggleSpotLight;
				isPressF = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			isPressR = true;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
			if (isPressR) {
				togglePointLight = !togglePointLight;
				isPressR = false;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			uMode = 0;
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			uMode = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			uMode = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			uMode = 3;

		if (cameraPos[2] > 4.5f) {
			cameraPos[2] = 4.4f;
		}
		if (cameraPos[2] < -4.5f) {
			cameraPos[2] = -4.4f;
		}
		if (cameraPos[0] > 4.5f) {
			cameraPos[0] = 4.4f;
		}
		if (cameraPos[0] < -4.5f) {
			cameraPos[0] = -4.4f;
		}
		if (cameraPos[1] != 0.0f) {
			if (cameraPos[0] < -4.0 && cameraPos[2] < -4.0) {
				cameraPos[1] = -1.0f;
			}
			else {
				cameraPos[1] = 0.0f;
			}
			
		}
		

		vmath::mat4 lookAt = vmath::lookat(cameraPos, cameraPos + direction, cameraUp);
		vmath::mat4 projM = vmath::perspective(fov, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

		// Render-To-Texture Framebuffer 바인딩 ---------------------------------------- (39:29)
			// FBO 바인딩
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);

		// FBO에 연결된 버퍼들의 값을 지우고, 뎁스 테스팅 활성화
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// =======================================================

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		//glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);


		// 라이팅 설정 ---------------------------------------
		vmath::vec3 lightColor(1.0f, 1.0f, 1.0f);
		vmath::vec3 viewPos = cameraPos;

		// 박스 그리기 ---------------------------------------
		vmath::mat4 transM = vmath::translate(vmath::vec3((float)sin(currentTime * 0.5f), 0.0f, (float)cos(currentTime * 0.5f) * 0.7f));
		float angle = currentTime * 100;
		vmath::mat4 rotateM = vmath::rotate(angle, 0.0f, 1.0f, 0.0f);

		glUseProgram(shader_program_normal);

		glUniformMatrix4fv(glGetUniformLocation(shader_program_normal, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program_normal, "view"), 1, GL_FALSE, lookAt);

		glUniform3fv(glGetUniformLocation(shader_program_normal, "viewPos"), 1, viewPos);

		glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.direction"), -1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		//glUniform3f(glGetUniformLocation(shader_program_normal, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		
		//for (int i = 0; i < pointLightPos.size(); i++) {
		//	char valueName[20] = "pointLights";
		//	strcat_s(valueName, std::to_string(i).c_str());
		//	strcat_s(valueName, "]");
		//}

		glUniform3f(glGetUniformLocation(shader_program_normal, "LightPosition_worldspace"), pointLightPos[0][0], pointLightPos[0][1], pointLightPos[0][2]);
		glUniform3fv(glGetUniformLocation(shader_program_normal, "pointLights[0].position"), 1, pointLightPos[0]);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_program_normal, "pointLights[0].c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program_normal, "pointLights[0].c2"), 0.032f);
		if (togglePointLight) {
			glUniform3f(glGetUniformLocation(shader_program_normal, "LightPosition_worldspace"), pointLightPos[1][0], pointLightPos[1][1], pointLightPos[1][2]);
			glUniform3fv(glGetUniformLocation(shader_program_normal, "pointLights[0].position"), 1, pointLightPos[1]);
		}

		glUniform3fv(glGetUniformLocation(shader_program_normal, "pointLights[1].position"), 1, pointLightPos[1]);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_program_normal, "pointLights[1].c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program_normal, "pointLights[1].c2"), 0.032f);

		glUniform3fv(glGetUniformLocation(shader_program_normal, "spotLight.position"), 1, cameraPos);
		glUniform3fv(glGetUniformLocation(shader_program_normal, "spotLight.direction"), 1, direction);
		glUniform1f(glGetUniformLocation(shader_program_normal, "spotLight.cutOff"), (float)cos(vmath::radians(12.5)));
		glUniform1f(glGetUniformLocation(shader_program_normal, "spotLight.outerCutOff"), (float)cos(vmath::radians(15.5)));
		glUniform1f(glGetUniformLocation(shader_program_normal, "spotLight.c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program_normal, "spotLight.c2"), 0.032f);
		glUniform3f(glGetUniformLocation(shader_program_normal, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		if (toggleSpotLight) {
			glUniform3f(glGetUniformLocation(shader_program_normal, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shader_program_normal, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(glGetUniformLocation(shader_program_normal, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(shader_program_normal, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		}


		for (int i = 0; i < boxPositions.size(); i++)
		{
			float angle = sin(currentTime * 0 / 180) * 180.f * 0;
			vmath::mat4 model = vmath::translate(boxPositions[i]) *
				vmath::rotate(angle, 1.0f, 0.5f, 0.3f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program_normal, "model"), 1, GL_FALSE, model);
			boxModel.draw(shader_program_normal);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////

		glUseProgram(shader_program);

		glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, projM);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, GL_FALSE, lookAt);

		glUniform3fv(glGetUniformLocation(shader_program, "viewPos"), 1, viewPos);

		glUniform3f(glGetUniformLocation(shader_program, "dirLight.direction"), -1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shader_program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);
		//glUniform3f(glGetUniformLocation(shader_program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		//glUniform3f(glGetUniformLocation(shader_program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		//glUniform3f(glGetUniformLocation(shader_program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		//for (int i = 0; i < pointLightPos.size(); i++) {
		//	char valueName[20] = "pointLights";
		//	strcat_s(valueName, std::to_string(i).c_str());
		//	strcat_s(valueName, "]");
		//}

		glUniform3f(glGetUniformLocation(shader_program, "LightPosition_worldspace"), pointLightPos[0][0], pointLightPos[0][1], pointLightPos[0][2]);
		glUniform3fv(glGetUniformLocation(shader_program, "pointLights[0].position"), 1, pointLightPos[0]);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[0].c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[0].c2"), 0.032f);
		if (togglePointLight) {
			glUniform3f(glGetUniformLocation(shader_program, "LightPosition_worldspace"), pointLightPos[1][0], pointLightPos[1][1], pointLightPos[1][2]);
			glUniform3fv(glGetUniformLocation(shader_program, "pointLights[0].position"), 1, pointLightPos[1]);
		}

		glUniform3fv(glGetUniformLocation(shader_program, "pointLights[1].position"), 1, pointLightPos[1]);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader_program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[1].c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program, "pointLights[1].c2"), 0.032f);

		glUniform3fv(glGetUniformLocation(shader_program, "spotLight.position"), 1, cameraPos);
		glUniform3fv(glGetUniformLocation(shader_program, "spotLight.direction"), 1, direction);
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.cutOff"), (float)cos(vmath::radians(12.5)));
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.outerCutOff"), (float)cos(vmath::radians(15.5)));
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.c1"), 0.09f);
		glUniform1f(glGetUniformLocation(shader_program, "spotLight.c2"), 0.032f);
		glUniform3f(glGetUniformLocation(shader_program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		if (toggleSpotLight) {
			glUniform3f(glGetUniformLocation(shader_program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shader_program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform3f(glGetUniformLocation(shader_program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(shader_program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		}

		// 램프 ---------------------------------------------
		for (int i = 0; i < lampPositions.size(); i++)
		{
			vmath::mat4 model = vmath::translate(lampPositions[i]) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			lampModel.draw(shader_program);
		}

		// 테이블 ---------------------------------------------
		for (int i = 0; i < tablePositions.size(); i++)
		{
			vmath::mat4 model = vmath::translate(tablePositions[i]) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			tableModel.draw(shader_program);
		}

		// 조각상 ------
		for (int i = 0; i < marblePositions.size(); i++)
		{
			vmath::mat4 model = vmath::translate(marblePositions[i]) *
				vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(3.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			marbleModel.draw(shader_program);
		}

		// 표지판 ------
		for (int i = 0; i < signPositions.size(); i++)
		{
			vmath::mat4 model = vmath::translate(signPositions[i]) *
				vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			signModel.draw(shader_program);
		}

		// 침대 ------
		for (int i = 0; i < bedPositions.size(); i++)
		{
			vmath::mat4 model = vmath::translate(bedPositions[i]) *
				vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(0.1f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			bedModel.draw(shader_program);
		}

		// 사과 ---------------------------------------------
		for (int i = 0; i < applePositions.size(); i++)
		{
			float angle = currentTime * 50;
			vmath::mat4 model = vmath::translate(applePositions[i]) *
				//vmath::rotate(angle, 0.0f, 1.0f, 0.0f) *
				vmath::rotate(180.0f, 1.0f, 0.0f, 0.0f) *
				vmath::scale(1.5f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			appleModel.draw(shader_program);
		}

		// 말 ---------------------------------------------
		for (int i = 0; i < horsePositions.size(); i++)
		{
			float angle = sin(currentTime * i / 180) * 360.f;
			vmath::mat4 model = vmath::translate(horsePositions[i]) *
				vmath::rotate(angle, 0.0f, 1.0f, 0.0f) *
				vmath::rotate(180.f, 1.0f, 0.0f, 0.0f) *
				vmath::scale((i%7) * 1.0f);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, model);
			horseModel.draw(shader_program);
		}

		// 땅 그리기 ---------------------------------------------
		for (int i = 0; i < groundPositions.size(); i++) {
			float scaleFactor = 0.05f;
			vmath::mat4 transform = vmath::translate(groundPositions[i]) *
				vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f) *
				vmath::scale(groundScales[i]);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, transform);

			groundModel.draw(shader_program);
		}

		// 피라미드 그리기 ---------------------------------------
		for (int i = 0; i < 2; i++)
		{
			float scaleFactor = 0.05f;
			vmath::mat4 transform = vmath::translate(pointLightPos[i]) *
				vmath::rotate(angle * 0.5f, 0.0f, 1.0f, 0.0f) *
				vmath::scale(scaleFactor, scaleFactor, scaleFactor);
			glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, transform);

			pyramidModel.draw(shader_program);
		}

		// 윈도우 Framebuffer 바인딩 ---------------------------------------- (42:02)
	// 기본 Framebuffer로 되돌리기
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);

		

		// 버퍼들의 값 지우기
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shader_program_dimension);
		//glUniformMatrix4fv(glGetUniformLocation(shader_program_dimension, "transform"), 1, GL_FALSE, projM* lookAt* rotateM* vmath::scale(1.3f)* vmath::translate(0.0f, -0.5f, 0.0f));
		//glUniform1i(glGetUniformLocation(shader_program_dimension, "texture1"), 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, FBO_texture);
		//glBindVertexArray(VAO_screen);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// Post Processing
		glUseProgram(shader_program_screen);
		glUniform1i(glGetUniformLocation(shader_program_screen, "screenTexture"), 0);
		glUniform1i(glGetUniformLocation(shader_program_screen, "uMode"), uMode);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBO_texture);
		glBindVertexArray(VAO_screen);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glfwSetCursorPos(window, info.windowWidth / 2, info.windowHeight / 2);
		lastTime = currentTime;
	}

	void onResize(int w, int h)
	{
		sb7::application::onResize(w, h);

		if (glViewport != NULL)
			glViewport(0, 0, info.windowWidth, info.windowHeight);
	}

#define MAX_FOV 120.f
#define MIN_FOV 10.f
	virtual void onMouseWheel(int pos)
	{
		if (pos > 0)
			fov = vmath::min(MAX_FOV, fov + 1.0f);
		else
			fov = vmath::max(MIN_FOV, fov - 1.0f);
	}

	virtual void init() {
		sb7::application::init();

		info.samples = 8;
	}

private:
	GLuint shader_program;
	GLuint shader_program_screen;
	GLuint shader_program_normal;
	GLuint shader_program_dimension;

	Model pyramidModel, groundModel, lampModel, tableModel, appleModel, horseModel, marbleModel, signModel, bedModel;
	NormalModel boxModel;

	std::vector<vmath::vec3> boxPositions;
	
	std::vector<vmath::vec3> groundPositions;
	std::vector<vmath::vec3> groundScales;

	std::vector<vmath::vec3> pointLightPos;

	std::vector<vmath::vec3> lampPositions;

	std::vector<vmath::vec3> tablePositions;

	std::vector<vmath::vec3> applePositions;

	std::vector<vmath::vec3> horsePositions;

	std::vector<vmath::vec3> marblePositions;

	std::vector<vmath::vec3> signPositions;

	std::vector<vmath::vec3> bedPositions;

	// Frame Buffer Object (24:18)
	GLuint FBO, FBO_texture;

	// In, Post Processing
	GLuint VAO_screen, VBO_screen;

	int uMode = 0;
	double lastTime = 0.0f;

	bool toggleSpotLight = true;
	bool isPressF = false;
	bool togglePointLight = true;
	bool isPressR = false;

	vmath::vec3 cameraPos = vmath::vec3(0.0f, 0.0f, 3.0f);
	vmath::vec3 cameraUp = vmath::vec3(0.0f, 1.0f, 0.0f);

	float horizontalAngle = -M_PI * 2;
	float verticalAngle = M_PI;

	// Get a handle for our "MVP" uniform
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ModelView3x3MatrixID;

	float fov = 50.f;
};


// DECLARE_MAIN의 하나뿐인 인스턴스
DECLARE_MAIN(my_application)