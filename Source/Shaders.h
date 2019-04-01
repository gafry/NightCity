#ifndef SHADERS_H
#define SHADERS_H

const char * instancingVertexShader =	"#version 450 core\n"
										"layout(location = 0) in vec3 aPos;\n"
										"layout(location = 1) in vec2 aTexCoord;\n"
										"layout(location = 2) in mat4 model_matrix;\n"

										"out vec2 TexCoord;\n"
										"out float distanceOfCamera;\n"

										"uniform mat4 view;\n"
										"uniform mat4 projection;\n"
										"uniform vec3 cameraPosition;\n"

										"void main()\n"
										"{\n"
										"	distanceOfCamera = distance(cameraPosition, vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]));\n"
										"gl_Position = projection * view * model_matrix * vec4(aPos, 1.0);\n"
										"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
										"}\n";

const char * instancingFragmentShader = "#version 450 core\n"
										"out vec4 FragColor;\n"

										"in vec2 TexCoord;\n"
										"in float distanceOfCamera;\n"
										"vec4 fogColor = vec4(0.5, 0.6, 0.7, 1.0);\n"

										"uniform sampler2D tex;\n"

										"uniform vec3 color;\n"

										"void main()\n"
										"{\n"
										"	float fogAmount = 1.0 - exp(-distanceOfCamera * 0.000007);\n"
										"	FragColor = texture(tex, TexCoord);\n"
										"	if (FragColor.x > 0.20)\n"
										"	{\n"
										"		FragColor = FragColor * vec4(color, 1.0);\n"
										"	}\n"
										"FragColor = mix(FragColor, fogColor, fogAmount);\n"
										"}\n";

const char * ordinaryVertexShader =		"#version 450 core\n"
										"layout(location = 0) in vec3 aPos;\n"
										"layout(location = 1) in vec2 aTexCoord;\n"

										"out vec2 TexCoord;\n"
										"out float distanceOfCamera;\n"

										"uniform mat4 model;\n"
										"uniform mat4 view;\n"
										"uniform mat4 projection;\n"
										"uniform vec3 cameraPosition;\n"

										"void main()\n"
										"{\n"
										"	distanceOfCamera = distance(cameraPosition, vec3(model[3][0], model[3][1], model[3][2]));\n"
										"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
										"	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
										"}\n";

const char * ordinaryFragmentShader =	"#version 450 core\n"
										"out vec4 FragColor;\n"

										"in vec2 TexCoord;\n"
										"in float distanceOfCamera;\n"
										"vec4 fogColor = vec4(0.5, 0.6, 0.7, 1.0);\n"

										"uniform sampler2D tex;\n"

										"uniform vec3 color;\n"

										"void main()\n"
										"{\n"
										"	float fogAmount = 1.0 - exp(-distanceOfCamera * 0.000007);\n"
										"	FragColor = texture(tex, TexCoord);\n"
										"	if (FragColor.x > 0.20)\n"
										"	{\n"
										"		FragColor = FragColor * vec4(color, 1.0);\n"
										"	}\n"
										"	FragColor = mix(FragColor, fogColor, fogAmount);\n"
										"}\n";

const char * skyboxVertexShader =		"#version 450 core\n"
										"layout(location = 0) in vec3 aPos;\n"

										"out vec3 TexCoords;\n"

										"uniform mat4 projection;\n"
										"uniform mat4 view;\n"

										"void main()\n"
										"{\n"
										"	TexCoords = aPos;\n"
										"	vec4 pos = projection * view * vec4(aPos, 1.0);\n"
										"	gl_Position = pos.xyww;\n"
										"}\n";

const char * skyboxFragmentShader =		"#version 450 core\n"
										"out vec4 FragColor;\n"

										"in vec3 TexCoords;\n"

										"uniform samplerCube skybox;\n"

										"void main()\n"
										"{\n"
										"	FragColor = texture(skybox, TexCoords);\n"
										"}\n";

const char * volumetricVertexShader =	"#version 450 core\n"
										"layout(location = 0) in vec2 aPos;\n"
										"layout(location = 1) in vec2 aTexCoords;\n"

										"uniform vec2 lPosition;\n"

										"out vec2 lightPositionOnScreen;\n"
										"out vec2 TexCoords;\n"

										"void main()\n"
										"{\n"
										"	lightPositionOnScreen = lPosition;\n"

										"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
										"	TexCoords = aTexCoords;\n"
										"}\n";

const char * volumtricFragmentShader =	"#version 450 core\n"
										"out vec4 FragColor;\n"

										"in vec2 TexCoords;\n"
										"in vec2 lightPositionOnScreen;\n"

										"uniform sampler2D screenTexture;\n"
										"uniform sampler2D coloredScreenTexture;\n"

										"void main()\n"
										"{\n"
										"	float decay = 0.96815;\n"
										"	float exposure = 0.2;\n"
										"	float density = 0.926;\n"
										"	float weight = 0.58767;\n"
										"	float illuminationDecay = 1.0;\n"
										"	int NUM_SAMPLES = 100;\n"

										"	vec2 tc = TexCoords;\n"

										"	vec2 deltaTexCoord = tc - lightPositionOnScreen.xy;\n"
										"	deltaTexCoord *= 1.0 / float(NUM_SAMPLES) * density;\n"

										"	vec4 color = vec4(0);\n"

										"	for (int i = 0; i < NUM_SAMPLES; i++)\n"
										"	{\n"
										"		tc -= deltaTexCoord;"
										"		vec4 samplee = texture2D(screenTexture, tc) * 0.4;\n"
										"		samplee *= illuminationDecay * weight;\n"
										"		color += samplee;\n"
										"		illuminationDecay *= decay;\n"
										"	}\n"

										"	vec4 realColor = texture2D(coloredScreenTexture, TexCoords);\n"
										"	FragColor = ((vec4((vec3(color.r, color.g, color.b) * exposure), 1)) + (realColor*(1.1)));\n"
										"}\n";

const char * instancingBlackVertexShader =	"#version 450 core\n"
											"layout(location = 0) in vec3 aPos;\n"
											"layout(location = 1) in vec2 aTexCoord;\n"
											"layout(location = 2) in mat4 model_matrix;\n"

											"uniform mat4 view;\n"
											"uniform mat4 projection;\n"

											"void main()\n"
											"{\n"
											"	gl_Position = projection * view * model_matrix * vec4(aPos, 1.0);\n"
											"}\n";

const char * instancingBlackFragmentShader =	"#version 450 core\n"
												"out vec4 FragColor;\n"

												"uniform sampler2D tex;\n"

												"uniform vec3 color;\n"

												"void main()\n"
												"{\n"
												"	FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
												"}\n";

const char * ordinaryBlackVertexShader =	"#version 450 core\n"
											"layout(location = 0) in vec3 aPos;\n"
											"layout(location = 1) in vec2 aTexCoord;\n"

											"uniform mat4 model;\n"
											"uniform mat4 view;\n"
											"uniform mat4 projection;\n"

											"void main()\n"
											"{\n"
											"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
											"}\n";

const char * ordinaryBlackFragmentShader =	"#version 450 core\n"
											"out vec4 FragColor;\n"

											"uniform sampler2D tex;\n"

											"uniform vec3 color;\n"

											"void main()\n"
											"{\n"
											"	FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
											"}\n";

#endif
