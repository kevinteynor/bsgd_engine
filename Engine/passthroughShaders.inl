const char* passthrough_vertex_code =
	"#version 400\r\n"
	""
	"layout (location=0) in vec3 vertex_position;"
	"layout (location=1) in vec4 mat_color;"
	"out vec4 color;"
	""
	"uniform mat4 mvp;"
	""
	"void main()"
	"{"
	"	color = mat_color;"
	"	gl_Position = mvp * vec4(vertex_position, 1);"
	"}\0";
const char* passthrough_fragment_code = 
	"#version 400\r\n"
	""
	"in vec4 color;"
	"out vec4 FragColor;"
	""
	"void main()"
	"{"
	"	FragColor = color;"
	"}\0";