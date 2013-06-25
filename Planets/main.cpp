/* ----------------------------------*/
/* GENERAL HEADER FILES AND DEFINES  */
/* ----------------------------------*/

#include <iostream>

/* --------------------------------------------------*/
/* HEADER FILES AND DEFINES SPECIFIC FOR THIS SAMPLE */
/* --------------------------------------------------*/

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "cTexture.h"
#include "cTimer/cTimer.h"
#include "glslprogram.h"
#include "vbomesh.h"
#include "defines.h"
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtx/transform2.hpp"
#include "Planet.h"
using glm::mat4;
using glm::vec3;
using namespace std;
float velocity;


/* ----------------------------------------------------------------------------------------------------------------------*/
/* ------------------------------------------------- SAMPLE CODE BELOW --------------------------------------------------*/
/* ----------------------------------------------------------------------------------------------------------------------*/
bool glsl120fallback=false;
GLSLProgram prog;
VBOMesh *sphere;
mat4 model;
mat4 view;
mat4 projection;
float angle;
cTimer fpsTimer, translationTimer;
int fpsCount=0;
cTexture planetTexture[5];
Planet *Planets[10];
static vec3 g_camTarget(0.0f, 0.4f, 0.0f);

void init(void)
{
	if(glsl120fallback==false)
	{
		if( ! prog.compileShaderFromFile("shader/texture.vs",GLSLShader::VERTEX) )
		{
			printf("Vertex shader failed to compile!\n%s",
             prog.log().c_str());
			exit(1);
		}
		if( ! prog.compileShaderFromFile("shader/texture.fs",GLSLShader::FRAGMENT))
		{
			printf("Fragment shader failed to compile!\n%s",
             prog.log().c_str());
			exit(1);
		}
	}
	else
	{
		if( ! prog.compileShaderFromFile("shader/120/texture.vs",GLSLShader::VERTEX) )
		{
			printf("Vertex shader failed to compile!\n%s",
             prog.log().c_str());
			exit(1);
		}
		if( ! prog.compileShaderFromFile("shader/120/texture.fs",GLSLShader::FRAGMENT))
		{
			printf("Fragment shader failed to compile!\n%s",
             prog.log().c_str());
			exit(1);
		}
	}
  
	//bind attributes in case of glsl 1.20 fallback
	if(glsl120fallback)
	{
		glBindAttribLocation(prog.getHandle(), 0, "VertexPosition");
		glBindAttribLocation(prog.getHandle(), 1, "VertexNormal");
		glBindAttribLocation(prog.getHandle(), 2, "VertexTexCoord");
	}
  
  
  if( ! prog.link() )
  {
    printf("Shader program failed to link!\n%s",
           prog.log().c_str());
    exit(1);
  }
  
  velocity = 1;
  
  prog.use();
  
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  sphere = new VBOMesh("mesh/sphere.obj");
	
  view = glm::lookAt(vec3(1.5f,0.25f,2.0f), g_camTarget, vec3(0.0f,1.0f,0.0f));
  projection = mat4(1.0f);
  angle = (float)( TO_RADIANS(100.0) );
  
  prog.setUniform("Light.Intensity", vec3(0.9f,0.9f,0.9f) );
	
  glActiveTexture(GL_TEXTURE0);

  // Initialize the planets.
  Planets[0] = new Planet(0.8f, 0.0f, "texture/sun.jpg");
  Planets[1] = new Planet(0.03f, 0.86f, "texture/mercury.jpg");
  Planets[2] = new Planet(0.08f, 0.89f, "texture/venus.jpg");
  Planets[3] = new Planet(0.09f, 0.90f, "texture/earth.jpg");
  
  Planets[4] = new Planet(0.04f, 0.95f, "texture/mars.jpg");
  Planets[5] = new Planet(0.28f, 1.3f, "texture/jupiter.jpg");
  Planets[6] = new Planet(0.22f, 1.6f, "texture/saturn.jpg");
  
  Planets[7] = new Planet(0.12f, 1.9f, "texture/uranus.jpg");
  Planets[8] = new Planet(0.1f, 2.1f, "texture/neptune.jpg");
  Planets[9] = new Planet(0.015f, 2.3f, "texture/pluto.jpg");
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  prog.setUniform("Tex1", 0);
}

void update(int)
{
  translationTimer.AdvanceTime();
  angle -= translationTimer.GetElapsedTime() * velocity;
  translationTimer.Reset();
  
  glutTimerFunc(1,update,0);
	glutPostRedisplay();
}

void setMatrices()
{
  view = glm::lookAt(vec3(0.0f, 0.0f, 3.0f), g_camTarget, vec3(0.0f, 1.0f, 0.0f));
  mat4 mv = view * model;
  prog.setUniform("ModelViewMatrix", mv);
  prog.setUniform("NormalMatrix",
                  mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
  prog.setUniform("MVP", projection * mv);
}

void render(void)
{
	//calc fps
	fpsTimer.AdvanceTime();
	fpsCount++;
	if(fpsTimer.GetElapsedTime()>1)
	{
		char tempBuf[512];
		sprintf(tempBuf,"(%d fps) Planets sample",fpsCount);
		
		glutSetWindowTitle(tempBuf);
		fpsTimer.Reset();
		fpsCount=0;
	}
  //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //prog.setUniform("Light.Position", view * vec4(20.0f * cos(angle),1.0f,20.0f * sin(angle),1.0f) );
  prog.setUniform("Light.Position", view * vec4(0.0f, 0.0f, 0.0f, 1.0f) );
  prog.setUniform("Material.Ks", 0.15f, 0.15f, 0.15f);     // Specular
	prog.setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);        // Diffuse
  prog.setUniform("Material.Ka", 0.3f, 0.3f, 0.3f);        // Ambient
  prog.setUniform("Material.Shininess", 0.8f);
  
  //render sun in center
  prog.setUniform("Sun", true);
  model = Planets[0]->Update(angle);
  setMatrices();
	Planets[0]->Render();
  prog.setUniform("Sun", false);
  
  for (int i = 1; i < 10; i++)
  {
    model = Planets[i]->Update(angle);
    setMatrices();
    Planets[i]->Render();
  }
  
	glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0,0,w,h);
  //float c = 2.0f;
  projection = glm::perspective(100.0f,(float)w/(float)h,0.3f,100.0f);
}
void keyboard (unsigned char key, int x, int y)
{
  //cout << key << " " << x << " " << y;
  switch (key) {
    case 'w': g_camTarget.z -= 0.4f; break;
    case 's': g_camTarget.z += 0.4f; break;
    case 'd': g_camTarget.x += 0.4f; break;
    case 'a': g_camTarget.x -= 0.4f; break;
    case 'e': g_camTarget.y -= 0.4f; break;
    case 'q': g_camTarget.y += 0.4f; break;
      
    case 'm': velocity += 0.5f; break;
    case 'n': velocity -= 0.5f; break;
      
    default:
      break;
  }
	glutPostRedisplay ();
}

void getGlVersion(int *major, int *minor)
{
  const char *verstr = (const char *) glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf(verstr,"%d.%d", major, minor) != 2))
  {
    *major = *minor = 0;
    fprintf(stderr, "Invalid GL_VERSION format!!!\n");
  }
}

void getGlslVersion(int *major, int *minor)
{
  int gl_major, gl_minor;
  getGlVersion(&gl_major, &gl_minor);
  *major = *minor = 0;
  if(gl_major == 1)
  {
    /* GL v1.x can provide GLSL v1.00 only as an extension */
    const char *extstr = (const char *) glGetString(GL_EXTENSIONS);
    if ((extstr != NULL) &&
        (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
    {
      *major = 1;
      *minor = 0;
    }
  }
  else if (gl_major >= 2)
  {
    /* GL v2.0 and greater must parse the version string */
    const char *verstr =
    (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    if((verstr == NULL) ||
       (sscanf(verstr, "%d.%d", major, minor) != 2))
    {
      *major = *minor = 0;
      fprintf(stderr,
              "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
    }
  }
}
int main(int argc, char* argv[])
{
	//GLUT INIT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Planets");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1,update,0);
  
  cout << "GL VENDOR:" << glGetString(GL_VENDOR) << endl;
  cout << "GL RENDERER:" << glGetString(GL_RENDERER) << endl;
  cout << "GL VERSION(string):" << glGetString(GL_VERSION) << endl;
  cout << "GLSL VERSION:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  
  GLint glslmajor,glslminor;
  getGlslVersion(&glslmajor,&glslminor);
  
  //check if support is below GSLS 1.20
  if(glslmajor<=1 && glslminor<20)
  {
    cout << "This sample requires at least GLSL 1.20 support... Press key to exit.";
    getchar();
    return 0;
  }
  //check if we are between GSLS 1.20 and GSLS 2
  else if(glslmajor==1 && glslminor>=20)
  {
    cout << "Falling back to GLSL 1.20 shaders";
    glsl120fallback=true;
  }
  //check if we have GLSL 2
  else if(glslmajor==2)
  {
    cout << "Falling back to GLSL 1.20 shaders";
    glsl120fallback=true;
  }
  //check if we have GLSL 3 and below 3.30
  else if(glslmajor==3 && glslminor<30)
  {
    cout << "Falling back to GLSL 1.20 shaders";
    glsl120fallback=true;
  }
  
  
  //INIT GL RESOURCES
  init();
	
  //ENTER LOOP
  glutMainLoop();
  getchar();
  return 0;
}