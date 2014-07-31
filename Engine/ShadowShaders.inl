const char* shadow_vertex_shader_code = 
	"#version 400\r\n"
	"layout(location=0) in vec3 vertexPosition;\r\n"
	""
	"uniform mat4 mvp;\r\n"
	""
	"void main() {\r\n"
	"	gl_Position = mvp * vec4(vertexPosition, 1);\r\n"
	"}\0";

const char* shadow_fragment_shader_code = 
	"#version 400\r\n"
	""
	"out vec4 fragmentdepth;\r\n"
	""
	"void main(){\r\n"
	"	fragmentdepth = vec4(gl_FragCoord.z);\r\n"
	"}\0";