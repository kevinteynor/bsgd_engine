const char* vertexShaderCode = 
	"#version 400\r\n"
	"layout (location=0) in vec2 position;"
	"uniform mat2 transform;"
	""
	""
	"void main() {"
	""
	"   gl_Position = vec4(transform * position, 0, 1);"
	""
	""
	"}\0";
const char* fragShaderCode = 
	"#version 400\r\n"
	"in vec3 position;"
	"out vec4 FragColor;"
	""
	"void main() {"
	"   FragColor = vec4(1,0,0,1);"
	""
	"}\0";