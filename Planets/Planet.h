//
//  Planet.h
//  Planets
//
//  Created by Mihai Iachimovschi on 09.06.2013.
//  Copyright (c) 2013 Mihai Iachimovschi. All rights reserved.
//

#ifndef __Planets__Planet__
#define __Planets__Planet__

#include "vbomesh.h"
#include "cTexture.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtx/transform2.hpp"
using glm::mat4;
using glm::vec3;

class Planet
{
private:
  // These three values are all relative to sun.
  float radius;
  float distance;
  float velocity;
  
  VBOMesh *sphere;
  cTexture texture;
  mat4 model;
public:
  Planet();
  Planet(float rad, float dist, const char* tex);
  Planet(float rad, float dist, VBOMesh obj, const char * tex);
  ~Planet();
  mat4 Update(float angle);
  void Render();
};

#endif

/*
 Planet       Radius   Distance from sun
 Sun          695500                   0
 
 Mercury        2440            57910000
 Venus          6052           108200000
 Earth          6371           149600000
 
 Mars           3396           227900000
 Jupiter       69911           778500000
 Saturn        58232          1433000000
 
 Uranus        25362          2877000000
 Neptune       24622          4503000000
 Pluto          1153          5906376272
 
 Reference       1.0                 1.9
*/