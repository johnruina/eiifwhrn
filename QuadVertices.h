
#ifndef QUAD_VERTICES
#define QUAD_VERTICES

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};
	
float quadVerticesNoTexCoords[] = { 
	// positions  
	-1.0f,  1.0f, 
	-1.0f, -1.0f,
	 1.0f, -1.0f, 

	-1.0f,  1.0f, 
	 1.0f, -1.0f, 
	 1.0f,  1.0f, 
};

#endif