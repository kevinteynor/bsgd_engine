const char* debug_shapes_vertex_shader_code =
	"#version 400\r\n"
	""
	"layout (location=0) in vec3 vertex_position;\r\n"
	"out vec4 color;\r\n"
	""
	"uniform mat4 mvp;\r\n"
	"uniform vec4 uniColor;\r\n"
	""
	"void main()\r\n"
	"{\r\n"
	"	color = uniColor;\r\n"
	"	gl_Position = mvp * vec4(vertex_position, 1);\r\n"
	"}\0";
const char* debug_shapes_fragment_shader_code = 
	"#version 400\r\n"
	""
	"in vec4 color;\r\n"
	"out vec4 FragColor;\r\n"
	""
	"void main()\r\n"
	"{\r\n"
	"	FragColor = color;\r\n"
	"}\0";